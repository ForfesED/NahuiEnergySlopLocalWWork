// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/io/fs_path.h"

namespace nau::io
{
    FsPath::FsPath() = default;

    FsPath::FsPath(const FsPath& other) :
        m_path(other.m_path)
    {
        makePreferredPathStringInplace(m_path);
    }

    FsPath::FsPath(FsPath&& other) :
        m_path(std::move(other.m_path))
    {
        makePreferredPathStringInplace(m_path);
    }

    FsPath::FsPath(StringType&& str) :
        m_path(std::move(str))
    {
        makePreferredPathStringInplace(m_path);
    }

    FsPath& FsPath::operator=(const FsPath& other)
    {
        m_path = other.m_path;
        return *this;
    }

    FsPath& FsPath::operator=(FsPath&& other)
    {
        m_path = std::move(other.m_path);
        other.m_path.clear();
        return *this;
    }

    bool FsPath::operator==(const FsPath& other) const
    {
        if (isEmpty() && other.isEmpty())
        {
            return true;
        }

        if (m_path == "/" || other.m_path == "/")
        {
            return m_path == other.m_path;
        }

        auto elements = splitElements();
        auto otherElements = other.splitElements();

        auto iter1 = elements.begin();
        auto iter2 = otherElements.begin();

        for (; iter1 != elements.end() && iter2 != otherElements.end(); ++iter1, ++iter2)
        {
            if (*iter1 != *iter2)
            {
                return false;
            }
        }

        return iter1 == elements.end() && iter2 == elements.end();
    }

    FsPath& FsPath::append(const FsPath& other)
    {
        NAU_ASSERT(!other.isAbsolute());

        return append(other.m_path);
    }

    FsPath& FsPath::appendInternal(std::string_view name)
    {
        NAU_ASSERT(!FsPath{std::string{name}}.isAbsolute());

        scope_on_leave
        {
            makePreferredPathStringInplace(m_path);
        };

        while (name.starts_with("/"))
        {
            name = name.substr(1);
        }

        while (name.ends_with("/"))
        {
            name = name.substr(0, name.size() - 1);
        }

        if (!name.empty())
        {
            if (m_path.empty())
            {
                m_path.assign(name.data(), name.size());
            }
            else if (m_path.ends_with("/"))
            {
                m_path = std::format("{}{}", m_path, name);
            }
            else
            {
                m_path = std::format("{}/{}", m_path, name);
            }
        }

        return *this;
    }

    FsPath& FsPath::concatInternal(std::string_view str)
    {
        scope_on_leave
        {
            makePreferredPathStringInplace(m_path);
        };

        if (!str.empty())
        {
            m_path.append(str.data(), str.size());
        }

        return *this;
    }

    FsPath FsPath::replace_extension(const FsPath& replacement) const
    {
        const size_t separatorIndex = m_path.rfind('/');
        const size_t dotIndex = m_path.find_last_of('.');

        if (separatorIndex == std::string::npos || separatorIndex < dotIndex)
        {
            return std::format("{}.{}", m_path.substr(0, dotIndex), replacement.getCStr());
        }

        return std::format("{}.{}", m_path, replacement.getCStr());
    }

    FsPath FsPath::getRelativePath(const FsPath& base) const
    {
        auto elements = splitElements();
        auto baseElements = base.splitElements();

        auto elementIter = elements.begin();
        auto otherElementIter = baseElements.begin();

        for (; elementIter != elements.end() && otherElementIter != baseElements.end(); ++elementIter, ++otherElementIter)
        {
            if (*elementIter != *otherElementIter)
            {
                return FsPath{};
            }
        }

        FsPath relativePath;
        for (; elementIter != elements.end(); ++elementIter)
        {
            relativePath.append(*elementIter);
        }

        return relativePath;
    }

    std::string FsPath::getRootPath() const
    {
        if (m_path.empty())
        {
            return {};
        }

        const auto separatorIndex = m_path.starts_with("/") ? m_path.find('/', 1) : m_path.find('/');

        if (separatorIndex == 0)
        {
            return std::string{"/"};
        }
        else if (separatorIndex == std::string::npos)
        {
            return {};
        }

        return m_path.substr(0, separatorIndex);
    }

    FsPath FsPath::getParentPath() const
    {
        if (m_path.empty())
        {
            return {};
        }

        const auto separatorIndex = m_path.rfind('/');

        if (separatorIndex == 0)
        {
            return std::string{"/"};
        }
        else if (separatorIndex == std::string::npos)
        {
            return {};
        }

        return m_path.substr(0, separatorIndex);
    }

    std::string_view FsPath::getName() const
    {
        const auto separatorIndex = m_path.find_last_of('/');
        if (separatorIndex == std::string_view::npos)
        {
            return m_path;
        }

        const size_t nameStartIndex = separatorIndex + 1;
        return std::string_view{m_path.data() + nameStartIndex, m_path.size() - nameStartIndex};
    }

    std::string_view FsPath::getExtension() const
    {
        auto fileName = getName();
        if (fileName.empty() || fileName == "." || fileName == "..")
        {
            return {};
        }

        const auto dotIndex = fileName.rfind('.');
        if (dotIndex == std::string_view::npos || dotIndex == 0)
        {
            return {};
        }

        return fileName.substr(dotIndex, fileName.size() - dotIndex);
    }

    std::string_view FsPath::getStem() const
    {
        auto fileName = getName();
        if (fileName.empty() || fileName == "." || fileName == "..")
        {
            return fileName;
        }

        const auto dotIndex = fileName.rfind('.');
        if (dotIndex == std::string_view::npos || dotIndex == 0)
        {
            return fileName;
        }

        return fileName.substr(0, dotIndex);
    }

    std::string FsPath::getString() const
    {
        return m_path;
    }

    const char* FsPath::getCStr() const
    {
        return m_path.c_str();
    }

    bool FsPath::isAbsolute() const
    {
        return m_path.starts_with("/");
    }

    bool FsPath::isRelative() const
    {
        return !isEmpty() && !isAbsolute();
    }

    bool FsPath::isEmpty() const
    {
        return m_path.empty();
    }

    FsPath& FsPath::makeAbsolute()
    {
        scope_on_leave
        {
            makePreferredPathStringInplace(m_path);
        };

        if (m_path.empty())
        {
            m_path.assign("/");
        }
        else if (!m_path.starts_with('/'))
        {
            m_path.insert(m_path.begin(), '/');
        }

        return *this;
    }

    size_t FsPath::getHashCode() const
    {
        return m_path.empty() ? 0 : std::hash<std::string>{}(m_path);
    }

    Result<> parse(std::string_view str, FsPath& path)
    {
        path = FsPath{str};
        return ResultSuccess;
    }

    std::string toString(const FsPath& path)
    {
        return path.getString();
    }

    std::string makePreferredPathString(std::string_view pathString)
    {
        std::string result{pathString};
        makePreferredPathStringInplace(result);
        return result;
    }

    void makePreferredPathStringInplace(std::string& pathStr)
    {
        // remove all leading spaces
        while (!pathStr.empty() && pathStr.starts_with(' '))
        {
            pathStr.erase(pathStr.begin());
        }

        // remove all trailing spaces
        while (!pathStr.empty() && pathStr.ends_with(' '))
        {
            pathStr.resize(pathStr.size() - 1);
        }

        if (pathStr.empty())
        {
            return;
        }

        // replace all backslashes with forward slashes
        std::replace(pathStr.begin(), pathStr.end(), '\\', '/');

        // remove all empty (double) slashes
        size_t p = 0;
        while ((p = pathStr.find("//", p)) != std::string::npos)
        {
            pathStr.replace(p, 2, "/", 1);
        }

        // remove trailing slash (checks root only case)
        if (pathStr.size() > 1 && pathStr.ends_with('/'))
        {
            pathStr.resize(pathStr.size() - 1);
        }
    }
}  // namespace nau::io
