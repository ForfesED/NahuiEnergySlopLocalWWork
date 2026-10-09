// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


// clang-format off
#include <rpc.h>

#include "nau/platform/windows/utils/uid.h"
#include "nau/platform/windows/diag/win_error.h"
// clang-format on

namespace nau
{
    Uid Uid::generate()
    {
        GUID uid;
        const RPC_STATUS result = UuidCreateSequential(&uid);
        if (result == RPC_S_OK || result == RPC_S_UUID_LOCAL_ONLY)
        {
            return uid;
        }

        [[maybe_unused]] const auto errCode = diag::getAndResetLastErrorCode();
        [[maybe_unused]] const std::string errMessage = diag::getWinErrorMessageA(errCode);
        NAU_FAILURE("Fail to generate Unique ID. Error({}):{}", errCode, errMessage);

        return Uid{};
    }

    Result<Uid> Uid::parseString(std::string_view str)
    {
        using RpcChar = unsigned char;
        std::array<RpcChar, 64> rpcString;

        if (str.empty() || (rpcString.size() - 1) < str.length())
        {
            return NauMakeError("Invalid input string");
        }

        strncpy(reinterpret_cast<char*>(rpcString.data()), str.data(), str.size());
        rpcString[str.size()] = 0ui8;

        GUID uid;
        const RPC_STATUS parseResult = ::UuidFromStringA(rpcString.data(), &uid);
        if (parseResult == S_OK)
        {
            return Uid{uid};
        }

        if (parseResult == RPC_S_INVALID_STRING_UUID)
        {
            return NauMakeError("Invalid UID string ({})", str);
        }
        else if (const auto err = ::GetLastError(); err != 0)
        {
            NauMakeErrorT(diag::WinCodeError)(err);
        }

        return NauMakeError("Error while parse UID string ({})", str);
    }

    Result<> parse(std::string_view str, Uid& uid)
    {
        const auto parseResult = Uid::parseString(str);
        if (!parseResult)
        {
            uid = Uid{};
            return parseResult.getError();
        }

        uid = *parseResult;
        return ResultSuccess;
    }

    std::string toString(const Uid& uid)
    {
        // https://learn.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-uuidtostringa
        RPC_CSTR str = nullptr;
        scope_on_leave
        {
            if (str != nullptr)
            {
                RpcStringFreeA(&str);
            }
        };

        [[maybe_unused]] const RPC_STATUS status = UuidToStringA(&uid.m_data, &str);
        if (status == S_OK)
        {
            return std::string{reinterpret_cast<char*>(str)};
        }

        return {};
    }

    Uid::Uid() noexcept
    {
        memset(&m_data, 0, sizeof(GUID));
    }

    Uid::Uid(GUID data) noexcept :
        m_data(data)
    {
    }

    size_t Uid::getHashCode() const
    {
        [[maybe_unused]] RPC_STATUS status = S_OK;
        GUID mutableData = m_data;
        const unsigned short hash = UuidHash(&mutableData, &status);
        NAU_ASSERT(status == S_OK);

        return static_cast<size_t>(hash);
    }

    Uid::operator bool() const noexcept
    {
        const bool isNull = m_data.Data1 == 0 &&
                            m_data.Data2 == 0 &&
                            m_data.Data3 == 0 &&
                            m_data.Data4[0] == 0 &&
                            m_data.Data4[1] == 0 &&
                            m_data.Data4[2] == 0 &&
                            m_data.Data4[3] == 0 &&
                            m_data.Data4[4] == 0 &&
                            m_data.Data4[5] == 0 &&
                            m_data.Data4[6] == 0 &&
                            m_data.Data4[7] == 0;

        return !isNull;
    }

    bool operator<(const Uid& uid1, const Uid& uid2) noexcept
    {
#if 0
        [[maybe_unused]] RPC_STATUS status = RPC_S_OK;
        const auto cmpRes = UuidCompare(&uid1.m_data, &uid2.m_data, &status);
        NAU_ASSERT(status == RPC_S_OK);

        return cmpRes < 0;
#else
        return memcmp(&uid1.m_data, &uid2.m_data, sizeof(GUID)) < 0;
#endif
    }

    bool operator==(const Uid& uid1, const Uid& uid2) noexcept
    {
        return uid1.m_data == uid2.m_data;
    }

    bool operator!=(const Uid& uid1, const Uid& uid2) noexcept
    {
        return uid1.m_data != uid2.m_data;
    }
}  // namespace nau
