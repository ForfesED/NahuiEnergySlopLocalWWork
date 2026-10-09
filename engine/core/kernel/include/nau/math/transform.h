// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/scene/transform.h


#pragma once
#include "nau/math/math.h"

namespace nau::math
{
    class Transform;

    Transform lerpTransform(const Transform& A, const Transform& B, float alpha);
    Transform slerpTransform(const Transform& A, const Transform& B, float alpha);

    class alignas(Quat) Transform
    {
        Quat m_quat = {0, 0, 0, 1};
        Vector3 m_translation = {0, 0, 0};
        Vector3 m_scale = {1.f, 1.f, 1.f};

        NAU_CLASS_FIELDS(
            CLASS_NAMED_FIELD(m_quat, "rotation"),
            CLASS_NAMED_FIELD(m_translation, "translation"),
            CLASS_NAMED_FIELD(m_scale, "scale")
        )

    public:
        static const Transform& identity()
        {
            static Transform identity = { };
            return identity;
        }

#pragma region NaNChecks
        NAU_FORCE_INLINE bool isTranslationNaN() const
        {
            return FloatInVec(m_translation.get128()).hasInfOrNaN();
        };
        NAU_FORCE_INLINE bool isRotateNaN() const
        {
            return FloatInVec(m_quat.get128()).hasInfOrNaN();
        };
        NAU_FORCE_INLINE bool isScaleNaN() const
        {
            return FloatInVec(m_scale.get128()).hasInfOrNaN();
        };
        NAU_FORCE_INLINE bool containsNaN() const
        {
            return isTranslationNaN() || isRotateNaN() || isScaleNaN();
        };

        NAU_FORCE_INLINE bool isRotationNormalized() const
        {
            NAU_ASSERT(!isRotateNaN());
            return (dot(m_quat, m_quat) - FloatInVec(1)).abs() > FloatInVec(0);
        }

        inline bool isValid() const
        {
            return !(containsNaN() || !isRotationNormalized());
        }
#pragma endregion

#pragma region Constructors

        NAU_FORCE_INLINE Transform() = default;

        NAU_FORCE_INLINE explicit Transform(const Vector3& inTranslation) :
            m_quat(),
            m_translation(inTranslation),
            m_scale(1.f, 1.f, 1.f){};

        NAU_FORCE_INLINE explicit Transform(const Quat& inRotation) :
            m_quat(inRotation),
            m_translation(0.f, 0.f, 0.f),
            m_scale(1.f, 1.f, 1.f){};

        NAU_FORCE_INLINE Transform(const Quat& inRotation, const Vector3& inTranslation, const Vector3& inScale = Vector3(1.f, 1.f, 1.f)) :
            m_quat(inRotation),
            m_translation(inTranslation),
            m_scale(inScale){};
        NAU_FORCE_INLINE explicit Transform(const Matrix4& inMatrix)
        {
            setFromMatrix(inMatrix);
        };

#pragma endregion

#pragma region MatrixOperations

        /*
         * TRS Matrix form.
         */
        NAU_FORCE_INLINE Matrix4 toMatrixWithScale() const
        {
            auto output = Matrix4::rotation(m_quat);

            output = {
                output.getCol0() * FloatInVec(m_scale.getX()),
                output.getCol1() * FloatInVec(m_scale.getY()),
                output.getCol2() * FloatInVec(m_scale.getZ()),
                Vector4(m_translation, 1)};

            return output;
            // True equation:
            // return Matrix4::translation(m_translation) * Matrix4::rotation(m_quat) * Matrix4::scale(m_scale);
        };

        /*
         * TR Matrix form.
         */
        NAU_FORCE_INLINE Matrix4 toMatrixNoScale() const
        {
            auto output = Matrix4::rotation(m_quat);
            output.setCol3(Vector4(m_translation, 1));
            return output;
            // True equation:
            // return Matrix4::translation(m_translation) * Matrix4::rotation(m_quat);
        };

#pragma endregion

#pragma region LerpSlerp

        NAU_FORCE_INLINE Transform lerpTransform(const Transform& other, float alpha) const
        {
            return nau::math::lerpTransform(*this, other, alpha);
        };

        NAU_FORCE_INLINE Transform slerpTransform(const Transform& other, float alpha) const
        {
            return nau::math::slerpTransform(*this, other, alpha);
        };

#pragma endregion

    private:
        NAU_FORCE_INLINE static Transform decomposeWithScale(Matrix4 matrix, Vector3 scale)
        {
            Transform output;

            // Removing negative scale from matrix
            auto invScale = (divPerElem(Vector3{1, 1, 1}, Vector3{length(matrix[0]), length(matrix[1]), length(matrix[2])}));
            auto mask = absPerElem(scale) > Vector3{MATH_SMALL_NUMBER};
            invScale = select(Vector3::zero(), invScale, mask);
            Vector3 signVector = copySignPerElem(invScale, scale);
            matrix[0] *= signVector.getX();
            matrix[1] *= signVector.getY();
            matrix[2] *= signVector.getZ();

            // Decomposing
            Vector3 newTranslation = {0, 0, 0};
            Quat newRotation = {0, 0, 0, 1};
            Vector3 newScale = {1, 1, 1};
            decompose(matrix, newTranslation, newRotation, newScale);
            newRotation = newRotation * FloatInVec(1 / length(newRotation));

            output.setRotation(newRotation);
            output.setTranslation(newTranslation);
            output.setScale(scale);
            return output;
        };

    public:
#pragma region Operators

        NAU_FORCE_INLINE Transform operator*(const Transform& other) const
        {
            Transform output;

            output.setScale(m_scale * FloatInVec(other.m_scale.get128()));

            if(!(bool(m_scale > Vector3::zero())) || !(bool(other.m_scale > Vector3::zero())))
            {
                // Then some parts of scale are 0 or less
                // decompose usage is required to save rotation
                auto matrix = toMatrixWithScale() * other.toMatrixWithScale();
                return decomposeWithScale(matrix, output.m_scale);
            }

            output.setRotation(normalize(m_quat * other.m_quat));
            output.setTranslation(transformVector(other.m_translation) + m_translation);
            return output;
        }

        NAU_FORCE_INLINE void operator*=(const Transform& other)
        {
            setScale(m_scale * FloatInVec(other.m_scale.get128()));
            setRotation(m_quat * other.m_quat);
            setTranslation(other.m_translation + other.transformVector(m_translation));
        };

        NAU_FORCE_INLINE Quat operator*(const Quat& quat)
        {
            return transformRotation(quat);
        }

        NAU_FORCE_INLINE Point3 operator*(const Point3& quat)
        {
            return transformPoint(quat);
        }

        NAU_FORCE_INLINE Vector3 operator*(const Vector3& quat)
        {
            return transformVector(quat);
        }

#pragma endregion

#pragma region ApplyTransform

        NAU_FORCE_INLINE Vector3 transformVector(const Vector3& v) const
        {
            return rotate(m_quat, v * FloatInVec(m_scale.get128()));
        };
        NAU_FORCE_INLINE Point3 transformPoint(const Point3& p) const
        {
            return rotate(m_quat, scale(p, m_scale)) + m_translation;
        };
        NAU_FORCE_INLINE Quat transformRotation(const Quat& q) const
        {
            return m_quat * q;
        };

#pragma endregion

#pragma region Getters

        NAU_FORCE_INLINE const Vector3& getTranslation() const
        {
            return m_translation;
        }
        NAU_FORCE_INLINE const Vector3& getScale() const
        {
            return m_scale;
        }
        NAU_FORCE_INLINE const Quat& getRotation() const
        {
            return m_quat;
        }
        NAU_FORCE_INLINE Vector3& getTranslation()
        {
            return m_translation;
        }
        NAU_FORCE_INLINE Vector3& getScale()
        {
            return m_scale;
        }
        NAU_FORCE_INLINE Quat& getRotation()
        {
            return m_quat;
        }
        NAU_FORCE_INLINE Matrix4 getMatrix() const
        {
            return toMatrixWithScale();
        }

        NAU_FORCE_INLINE Transform inverse() const
        {
            Transform output;

            if((absPerElem(m_scale) < Vector3{MATH_SMALL_NUMBER}))
            {
                return identity();
            }

            output.setScale(divPerElem({1, 1, 1}, m_scale));
            auto mask = absPerElem(m_scale) > Vector3{MATH_SMALL_NUMBER};
            output.setScale(select(Vector3::zero(), output.m_scale, mask));
            output.m_scale.setW(0);
            output.setRotation(conj(m_quat));
            output.setTranslation(-output.transformVector(m_translation));
            output.m_translation.setW(0);
            return output;
        };

        /*
         * Returns Transform T, such than Other * T = this
         */
        NAU_FORCE_INLINE Transform getRelativeTransformInverse(const Transform& other) const
        {
            if(!(bool(m_scale > Vector3::zero())) || !(bool(other.m_scale > Vector3::zero())))
            {
                // Then some parts of scale are 0 or less
                // Decompose usage is required

                auto invScale = (divPerElem({1, 1, 1}, other.m_scale));
                auto mask = absPerElem(other.m_scale) > Vector3{MATH_SMALL_NUMBER};
                invScale = (select(Vector3::zero(), invScale, mask));
                invScale.setW(0);
                auto desiredScale = mulPerElem(invScale, m_scale);

                auto matrix = nau::math::inverse(other.toMatrixWithScale()) * toMatrixWithScale();
                return decomposeWithScale(matrix, desiredScale);
            }

            Transform output;

            auto invScale = divPerElem({1, 1, 1}, other.m_scale);
            auto mask = absPerElem(other.m_scale) > Vector3{MATH_SMALL_NUMBER};
            invScale = select(Vector3::zero(), invScale, mask);
            invScale.setW(0);
            auto invRot = conj(other.m_quat);

            output.setScale(m_scale * FloatInVec(invScale.get128()));
            output.setRotation(invRot * m_quat);
            auto diffT = m_translation - other.m_translation;
            output.setTranslation(mulPerElem(rotate(invRot, diffT), invScale));

            return output;
        };

        /*
         * Returns Transform T, such than this * T = other
         */
        NAU_FORCE_INLINE Transform getRelativeTransform(const Transform& other) const
        {
            return other.getRelativeTransformInverse(*this);
        };

#pragma endregion

#pragma region Setters

        NAU_FORCE_INLINE void setTranslation(float tx, float ty, float tz)
        {
            m_translation = {tx, ty, tz};
        }
        NAU_FORCE_INLINE void setTranslation(const Vector3& newTranslation)
        {
            m_translation = newTranslation;
        }
        NAU_FORCE_INLINE void setRotation(const Quat& newRotation)
        {
            m_quat = newRotation;
        }
        NAU_FORCE_INLINE void setScale(float sx, float sy, float sz)
        {
            m_scale = {sx, sy, sz};
        }
        NAU_FORCE_INLINE void setScale(const Vector3& newScale)
        {
            m_scale = newScale;
        }
        NAU_FORCE_INLINE void setComponents(const Vector3& newTranslation, const Quat& newRotation, const Vector3& newScale)
        {
            m_translation = newTranslation;
            m_scale = newScale;
            m_quat = newRotation;
        }
        NAU_FORCE_INLINE void setFromMatrix(const Matrix4& inMatrix)
        {
            Vector3 newTranslation;
            Quat newRotation;
            Vector3 newScale;
            decompose(inMatrix, newTranslation, newRotation, newScale);
            setComponents(newTranslation, newRotation, newScale);
        };

#pragma endregion

#pragma region Modifications

        NAU_FORCE_INLINE void addRotation(const Quat& deltaRotation)
        {
            m_quat = deltaRotation * m_quat;
        }

        NAU_FORCE_INLINE void addTranslation(const Vector3& deltaTranslation)
        {
            m_translation += deltaTranslation;
        }

        NAU_FORCE_INLINE void addScale(const Vector3& deltaScale)
        {
            m_scale = mulPerElem(deltaScale, m_scale);
        }

#pragma endregion

#pragma region Comparators
        NAU_FORCE_INLINE bool similar(const Transform& other, float tolerance = MATH_SMALL_NUMBER) const
        {
            return m_scale.similar(other.m_scale, tolerance) &&
                   m_translation.similar(other.m_translation, tolerance) &&
                   m_quat.similar(other.m_quat, tolerance);
        };
        bool operator==(const Transform& other) const
        {
            return m_scale == other.m_scale &&
                   m_translation == other.m_translation &&
                   m_quat == other.m_quat;
        };
#pragma endregion
    };

    //inline const Transform Transform::Identity = {};

    NAU_FORCE_INLINE Transform lerpTransform(const Transform& A, const Transform& B, float alpha)
    {
        return {
            normalize(lerp(alpha, A.getRotation(), B.getRotation())),
            Vectormath::lerp(A.getTranslation(), B.getTranslation(), alpha),
            Vectormath::lerp(A.getScale(), B.getScale(), alpha)};
    };

    NAU_FORCE_INLINE Transform slerpTransform(const Transform& A, const Transform& B, float alpha)
    {
        return {
            slerp(alpha, A.getRotation(), B.getRotation()),
            Vectormath::lerp(A.getTranslation(), B.getTranslation(), alpha),
            Vectormath::lerp(A.getScale(), B.getScale(), alpha)};
    };

}  // namespace nau::math

template <>
struct fmt::formatter<nau::math::Transform> : fmt::formatter<const char*>
{
    auto format(nau::math::Transform const& transform, fmt::format_context& ctx) const
    {
        auto rotation = transform.getRotation().toEuler();
        return format_to(ctx.out(),
                         "translate: ({}, {}, {})\n"
                         "rotation: ({}, {}, {})\n"
                         "scale: ({}, {}, {})",
                         float(transform.getTranslation().getX()), float(transform.getTranslation().getY()), float(transform.getTranslation().getZ()),
                         float(rotation.getX()), float(rotation.getY()), float(rotation.getZ()),
                         float(transform.getScale().getX()), float(transform.getScale().getY()), float(transform.getScale().getZ()));
    }
};
