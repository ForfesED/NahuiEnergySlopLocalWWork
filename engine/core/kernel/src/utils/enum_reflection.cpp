// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/utils/enum/enum_reflection.h"

#include "nau/diag/logging.h"
#include "nau/string/string_utils.h"

namespace nau::nau_detail
{
    namespace
    {
        /**
         parse value
             "EnumValue = XXX" -> "EnumValue"
      */
        std::string_view parseSingleEnumEntry(std::string_view enumStr)
        {
            const auto pos = enumStr.find_first_of('=');
            if (pos == std::string_view::npos)
            {
                return nau::strings::trim(enumStr);
            }

            return nau::strings::trim(enumStr.substr(0, pos));
        }
    }  // namespace

    void EnumTraitsHelper::parseEnumDefinition(std::string_view enumDefinitionString, [[maybe_unused]] size_t itemCount, std::string_view* result)
    {
        NAU_FATAL(result);
        NAU_FATAL(itemCount > 0);
        NAU_FATAL(!enumDefinitionString.empty());

        size_t index = 0;

        for (auto singleEnumString : strings::split(enumDefinitionString, std::string_view{","}))
        {
            NAU_FATAL(index < itemCount);
            result[index++] = parseSingleEnumEntry(singleEnumString);
        }
    }

    std::string_view EnumTraitsHelper::toString(IEnumRuntimeInfo& enumInfo, int value)
    {
        const auto intValues = enumInfo.getIntValues();
        size_t index = 0;
        for (size_t count = intValues.size(); index < count; ++index)
        {
            if (intValues[index] == static_cast<int>(value))
            {
                break;
            }
        }

        if (index == intValues.size())
        {
            NAU_FAILURE("Invalid enum ({}) int value ({})", enumInfo.getName(), value);
            return {};
        }

        const auto strValues = enumInfo.getStringValues();
        NAU_FATAL(index < strValues.size(), "Invalid internal enum runtime info");

        return strValues[index];
    }

    Result<int> EnumTraitsHelper::parse(IEnumRuntimeInfo& enumInfo, std::string_view str)
    {
        const auto strValues = enumInfo.getStringValues();
        size_t index = 0;
        for (size_t count = strValues.size(); index < count; ++index)
        {
            if (strings::icaseEqual(str, strValues[index]))
            {
                break;
            }
        }

        if (index == strValues.size())
        {
            return NauMakeError("Invalid enum value ({})", str);
        }

        const auto intValues = enumInfo.getIntValues();
        NAU_FATAL(index < intValues.size(), "Invalid internal enum runtime info ({})", str);

        return intValues[index];
    }

}  // namespace nau::nau_detail
