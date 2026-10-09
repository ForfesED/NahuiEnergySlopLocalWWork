// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/shared/version.h"

#include <format>
#include <sstream>
#include <vector>

namespace nau
{
    int NauVersion::getMajorVersion() const
    {
        return m_versions[major];
    }

    int NauVersion::getMinorVersion() const
    {
        return m_versions[minor];
    }

    int NauVersion::getPatchVersion() const
    {
        return m_versions[patch];
    }

    int NauVersion::getBuildVersion() const
    {
        return m_versions[build];
    }

    unsigned int NauVersion::getHash() const
    {
        std::stringstream ss;
        ss << m_versions[0] << m_versions[1] << m_versions[2] << m_versions[3];
        return std::stoul(ss.str());
    }

    int NauVersion::size() const
    {
        return m_size;
    }

    std::string NauVersion::toString() const
    {
        std::stringstream ss;

        for(int i = 0; i < m_size; ++i)
        {
            ss << m_versions[i];

            if(i != m_size - 1)
            {
                ss << ".";
            }
        }

        return ss.str();
    }

    NauVersion& NauVersion::fromString(const std::string& version)
    {
        if(version.empty())
        {
            throw std::invalid_argument("Empty version string");
        }

        // Split string via . as delimiter
        std::stringstream ss(version);
        std::string token;
        std::vector<std::string> tokens;

        while(std::getline(ss, token, '.'))
        {
            tokens.push_back(token);
        }
      
        // For understanding does version has patches or builds identifiers
        m_size = static_cast<int>(tokens.size());
      
        // Set versions
        for(int i = 0; i < m_size; ++i)
        {
            m_versions[i] = std::stoi(tokens[i]);
        }

        return *this;
    }
}  // namespace nau