// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// lua_toolkit/lua_utils.h


#pragma once
#include <string_view>

#include "lua_toolkit/lua_headers.h"
#include "lua_toolkit/lua_toolkit_config.h"
#include "nau/utils/preprocessor.h"
#include "nau/utils/result.h"

namespace nau::lua
{
    struct NAU_LUATOOLKIT_EXPORT StackGuard
    {
        lua_State* const luaState;
        const int top;

        StackGuard(lua_State* l);
        ~StackGuard();

        StackGuard(const StackGuard&) = delete;
        StackGuard& operator=(const StackGuard&) = delete;
    };

    NAU_LUATOOLKIT_EXPORT
    Result<> loadBuffer(lua_State* l, std::string_view buffer, const char* chunkName);

    NAU_LUATOOLKIT_EXPORT
    int getAbsoluteStackPos(lua_State* l, int index);

    /*
     *
     */
    class UpValuesEnumerator
    {
    public:
        class iterator
        {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = std::string_view;
            using difference_type = size_t;
            using pointer = nullptr_t;
            using reference = nullptr_t;

            iterator();
            iterator& operator++();
            std::string_view operator*() const;
            std::string_view Name() const;
            int Index() const;

        private:
            iterator(lua_State* l, int index, int n);

            lua_State* const m_luaState = nullptr;
            const int m_index = 0;
            int m_n = -1;
            const char* m_name = nullptr;

            friend class UpValuesEnumerator;
            friend bool operator==(const iterator&, const iterator&);
            friend bool operator!=(const iterator&, const iterator&);
        };

        UpValuesEnumerator(lua_State*, int index = -1);
        UpValuesEnumerator(const UpValuesEnumerator&) = delete;
        iterator begin() const;
        iterator end() const;

    private:
        lua_State* const m_luaState;
        const int m_index;
    };

    class TableEnumerator
    {
    public:
        class iterator
        {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = std::string_view;
            using difference_type = size_t;
            using pointer = nullptr_t;
            using reference = nullptr_t;

            iterator() = default;
            iterator& operator++();
            constexpr std::pair<int, int> operator*() const
            {
                return {keyIndex(), valueIndex()};
            }

            constexpr int keyIndex() const
            {
                return -2;
            }

            constexpr int valueIndex() const
            {
                return -1;
            }

        private:
            static constexpr int BadIndex = 0;

            iterator(lua_State* l, int tableIndex) :
                m_luaState(l),
                m_tableIndex(tableIndex)
            {
            }

            iterator& takeNext();

            lua_State* m_luaState = nullptr;
            int m_tableIndex = BadIndex;

            friend class TableEnumerator;

            // actually comparing only with end
            friend bool operator==(const iterator& i1, const iterator& i2)
            {
                return i1.m_tableIndex == i2.m_tableIndex;
            }

            friend bool operator!=(const iterator& i1, const iterator& i2)
            {
                return i1.m_tableIndex != i2.m_tableIndex;
            }
        };

        TableEnumerator(lua_State* l, int tableIndex);

        TableEnumerator(const UpValuesEnumerator&) = delete;

        iterator begin() const;

        iterator end() const
        {
            return {};
        }

        lua_State* const m_luaState;
        const int m_tableIndex;
    };

}  // namespace nau::lua

#define guard_lstack(luaState)                                 \
    const ::nau::lua::StackGuard ANONYMOUS_VAR(_luaStackGuard) \
    {                                                          \
        luaState                                               \
    }
