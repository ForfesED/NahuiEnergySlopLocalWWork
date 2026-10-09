// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/playback/animation_skeleton.h"

#include "animation_helper.h"

#include <ozz/base/span.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/base/containers/vector.h>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>

namespace nau::animation
{
    void SkeletalAnimation::apply(int frame, AnimationState& animationState) const
    {
        if(!animationState.target)
        {
            return;
        }

        if(SkeletonComponent* skeletonComponent = getAnimatableTarget<SkeletonComponent>(animationState))
        {
            const ozz::animation::Skeleton& skeleton = skeletonComponent->getSkeleton();
            const int numJoints = skeleton.num_joints();
            NAU_ASSERT(numJoints == ozzAnimation.num_tracks());

            SkeletalAnimRuntimeData& d = skeletonComponent->getAnimRuntimeDataMutable();

            auto& track = d.tracks[animationState.animInstanceName];

            track.blendMethod = animationState.blendMethod;
            track.weight = !animationState.isStopped ? animationState.weight : .0f;

            if (track.locals.size() < skeleton.num_soa_joints())
            {
                track.locals.resize(skeleton.num_soa_joints());
            }
            if (track.animSamplingContext.max_tracks() < numJoints)
            {
                track.animSamplingContext.Resize(numJoints);
            }

            ozz::animation::SamplingJob sampling_job;
            sampling_job.animation = &ozzAnimation;
            sampling_job.context = &track.animSamplingContext;
            sampling_job.ratio = frame / getDurationInFrames(); // [0, 1]
            sampling_job.output = ozz::make_span(track.locals);
            if (!sampling_job.Run()) {
                NAU_ASSERT(false);
            }
        }
    }

    float SkeletalAnimation::getDurationInFrames() const
    {
        return ozzAnimation.duration() * 60.0f; // 60.0f is default fake animation frameRate
    }

    void SkeletalAnimationMixer::blendAnimations(const IAnimatable::Ptr& target)
    {
        if (SkeletonComponent* skeletonComponent = getAnimatableTarget<SkeletonComponent>(target))
        {
            SkeletalAnimRuntimeData& d = skeletonComponent->getAnimRuntimeDataMutable();

            ozz::vector<ozz::animation::BlendingJob::Layer> layers;
            ozz::vector<ozz::animation::BlendingJob::Layer> additiveLayers;

            ozz::animation::BlendingJob::Layer* curLayer = nullptr;
            for (const auto& track : d.tracks)
            {
                switch(track.second.blendMethod) // blendMethod is set in SkeletalAnimation::apply
                {
                case AnimationBlendMethod::Mix:
                    curLayer = &layers.emplace_back();
                    break;
                case AnimationBlendMethod::Additive:
                    curLayer = &additiveLayers.emplace_back();
                    break;
                default:
                    break;
                }

                if (!curLayer)
                {
                    NAU_ASSERT(false);
                    continue;
                }
                curLayer->weight = track.second.weight; // weight is set in SkeletalAnimation::apply
                curLayer->transform = make_span(track.second.locals);
                //curLayer->joint_weights // <- can be used for per-bone masking for animation, not yet supported
            }

            ozz::animation::BlendingJob blend_job;
            blend_job.threshold = 0.05f; // todo: tunable param per skeleton? 
            blend_job.layers = ozz::make_span(layers);
            blend_job.additive_layers = ozz::make_span(additiveLayers);
            blend_job.rest_pose = skeletonComponent->getSkeleton().joint_rest_poses();
            blend_job.output = ozz::make_span(d.locals);

            if (!blend_job.Run()) {
                NAU_ASSERT(false);
            }
        }
    }

    void SkeletalAnimationMixer::computeFinalTransforms(const IAnimatable::Ptr& target)
    {
        if (SkeletonComponent* skeletonComponent = getAnimatableTarget<SkeletonComponent>(target))
        {
            SkeletalAnimRuntimeData& d = skeletonComponent->getAnimRuntimeDataMutable();

            ozz::animation::LocalToModelJob ltm_job;
            ltm_job.skeleton = &skeletonComponent->getSkeleton();
            ltm_job.input = ozz::make_span(d.locals);
            ltm_job.output = ozz::make_span(skeletonComponent->getModelSpaceJointMatricesMutable());
            if (!ltm_job.Run()) {
                NAU_ASSERT(false);
            }
        }
    }

}  // namespace nau::animation
