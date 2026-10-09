// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <iostream>
#include <string>
#include "nau/shared/api.h"

namespace nau
{
    class SHARED_API NauVersion
    {
        enum Component
        {
            major = 0,
            minor = 1,
            patch = 2,
            build = 3,
            count = 4
        };

    public:
        NauVersion() noexcept = default;
        ~NauVersion() noexcept = default;
        inline NauVersion(const NauVersion& other) noexcept
        {
            *this = other;
        }
        inline NauVersion(NauVersion&& other) noexcept
        {
            *this = std::move(other);
        }
        inline NauVersion& operator=(const NauVersion& other) noexcept
        {
            if(this != &other)
            {
                std::copy_n(other.m_versions, count, m_versions);
            }
            return *this;
        }
        inline NauVersion& operator=(NauVersion&& other) noexcept
        {
            if(this != &other)
            {
                std::swap(m_versions, other.m_versions);
            }
            return *this;
        }
        inline NauVersion(const std::string& version)
        {
            fromString(version);
        }
      
        inline operator std::string() const
        {
            return toString();
        }

        int getMajorVersion() const;
        int getMinorVersion() const;
        int getPatchVersion() const;
        int getBuildVersion() const;
        unsigned int getHash() const;
      
        int size() const;

        std::string toString() const;
        NauVersion& fromString(const std::string& version);

        inline auto operator<=>(const NauVersion& other) const
        {
            auto& [major, minor, patch, build] = m_versions;
            auto& [otherMajor, otherMinor, otherPatch, otherBuild] = other.m_versions;
            return std::tie(major, minor, patch, build) <=> std::tie(otherMajor, otherMinor, otherPatch, otherBuild);
        }

        bool operator==(const NauVersion& other) const = default;

        inline friend std::ostream& operator<<(std::ostream& os, const NauVersion& version)
        {
            os << version.toString();
            return os;
        }

    private:
        int m_versions[count] = {0, 0, 0, 0};
        int m_size = 0;
    };
}  // namespace nau