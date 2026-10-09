// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/project_tools/string_processor.h"
#include <regex>
#include <string>
#include <sstream>

namespace nau
{
    int StringProcessor::countMatches(const std::string& content, const std::string& pattern)
    {
        std::regex regex(pattern);
        std::sregex_iterator it(content.begin(), content.end(), regex);
        std::sregex_iterator end;

        int count = 0;

        while (it != end)
        {
            ++count;
            ++it;
        }

        return count;
    }

    void StringProcessor::toLower(std::string& string)
    {
        std::transform(string.begin(), string.end(), string.begin(),
        [](unsigned char c)
        {
            return std::tolower(c);
        });
    }

    void StringProcessor::trim(std::string& string)
    {
        string.erase(string.begin(), std::find_if(string.begin(), string.end(),
        [](unsigned char ch)
        {
            return !std::isspace(ch);
        }));
    }

    void StringProcessor::replace(std::string& string, char from, char to)
    {
        std::replace(string.begin(), string.end(), from, to);
    }

    bool StringProcessor::split(const std::string& string, const std::string& delimiter, std::vector<std::string> &out)
    {
        std::size_t start = 0;
        std::size_t end = string.find(delimiter);

        while (end != std::string::npos)
        {
            std::string substring = string.substr(start, end - start);
            trim(substring);
            out.push_back(substring);

            start = end + delimiter.length();
            end = string.find(delimiter, start);
        }

        std::string substring = string.substr(start, end - start);

        trim(substring);
        out.push_back(substring);

        return out.size() > 0;
    }

    std::string StringProcessor::modifyExpandedValue(const std::string& content, const std::vector<std::string>& modifiers)
    {
        std::string result = content;

        for (const std::string& modifier : modifiers)
        {
            if (modifier == "lower")
            {
                toLower(result);
            }
            else if (modifier == "no_space")
            {
                replace(result, ' ', '_');
            }
            else if (modifier == "cmake_path")
            {
                replace(result, '\\', '/');
            }
        }

        return result;
    }

    std::string StringProcessor::processRegexMatches(const std::string& pattern, const std::string& content, std::map<std::string, std::string>& projectProperties, int& count)
    {
        count = countMatches(content, pattern);

        std::regex regex(pattern);
        std::smatch matches;
        std::string result = content;

        while (std::regex_search(result, matches, regex))
        {
            std::string value = matches[1].str();
            std::istringstream stringStream(value);
            std::string option, modifier;
            std::vector<std::string> modifiers;

            std::getline(stringStream, option, ':');

            if (std::getline(stringStream, modifier, ','))
            {
                split(modifier, ",", modifiers);
            }

            if (projectProperties.count(option) == 0)
            {
                throw std::runtime_error("Invalid option: " + option);
            }

            std::string property = projectProperties[option];

            value = modifyExpandedValue(property, modifiers);

            std::string prefix = matches.prefix().str();
            std::string suffix = matches.suffix().str();

            result = prefix + value + suffix;

            count--;
        }

        return result;
    }
}