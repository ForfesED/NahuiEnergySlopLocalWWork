// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <cstdint>

#include "nau/3d/dag_resId.h"
#include "nau/3d/dag_resPtr.h"
#include "nau/3d/dag_resizableTex.h"
#include "nau/shaders/shader_globals.h"

namespace nau::render
{

    class DeferredRT
    {
    public:
        enum class StereoMode
        {
            MonoOrMultipass,
            SideBySideHorizontal,
            SideBySideVertical,
        };

        static constexpr int MAX_NUM_MRT = 4;

        DeferredRT(const char* name, int w, int h, StereoMode stereoMode, unsigned msaaFlag, int numRT, const unsigned texFmt[MAX_NUM_MRT], uint32_t depthFmt);

        void close();
        void setRt();

        uint32_t recreateDepth(uint32_t fmt);  // returns true if 32 bit depth buffer was created

        ~DeferredRT()
        {
            close();
        }
        void changeResolution(int w, int h);
        inline int getWidth() const
        {
            return m_width;
        }
        inline int getHeight() const
        {
            return m_height;
        }
        inline Texture* getDepth() const
        {
            return m_depth.get();
        }
        inline TEXTUREID getDepthId() const
        {
            return m_depth->getTID();
        }
        inline const TexPtr& getDepthAll() const
        {
            return m_depth;
        }
        inline Texture* getRt(uint32_t idx) const
        {
            return m_mrts[idx].get();
        }
        inline TEXTUREID getRtId(uint32_t idx) const
        {
            return m_mrts[idx]->getTID();
        }
        inline const TexPtr& getRtAll(uint32_t idx) const
        {
            return m_mrts[idx];
        }
        inline uint32_t getRtNum() const
        {
            return m_numRt;
        }

        inline d3d::SamplerHandle getDefaultSampler() const
        {
            return m_defaultSampler;
        }

    protected:
        uint32_t recreateDepthInternal(uint32_t fmt);

        math::IVector2 calcCreationSize() const;

        StereoMode m_stereoMode = StereoMode::MonoOrMultipass;
        int m_numRt = 0, m_width = 0, m_height = 0;
        nau::string m_name;

        d3d::SamplerHandle m_defaultSampler;
        ResizableResPtrTex m_mrts[MAX_NUM_MRT] = {};
        ResizableResPtrTex m_depth;

        bool m_useResolvedDepth = false;
    };
}