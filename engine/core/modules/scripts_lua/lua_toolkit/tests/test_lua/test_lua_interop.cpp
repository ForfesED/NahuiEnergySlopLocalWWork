// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "lua_toolkit/lua_headers.h"
#include "lua_toolkit/lua_interop.h"
#include "lua_toolkit/lua_utils.h"
#include "nau/dispatch/class_descriptor_builder.h"
#include "nau/meta/common_attributes.h"

namespace nau::test
{
    class TestLuaInterop : public ::testing::Test
    {
    protected:
        TestLuaInterop() :
            m_luaState(createLuaState())
        {
            NAU_ASSERT(m_luaState);
        }

        ~TestLuaInterop()
        {
            lua_close(m_luaState);
        }

        lua_State* getLua() const
        {
            return m_luaState;
        }

        void load(std::string_view code) const
        {
            if(auto res = lua::loadBuffer(getLua(), code, "default_chunk"); !res)
            {
            }

            if(lua_pcall(getLua(), 0, 0, 0) != 0)
            {
            }
        }

        void call(const char* name)
        {
            lua_getglobal(getLua(), name);
            if(lua_pcall(getLua(), 0, 1, 0) != 0)
            {
            }
        }

    private:
        static lua_State* createLuaState()
        {
            auto luaAlloc = [](void* ud, void* ptr, size_t osize, size_t nsize) -> void*
            {
                return ::realloc(ptr, nsize);
            };

            auto* const luaState = lua_newstate(luaAlloc, nullptr);
            luaL_openlibs(luaState);

            return luaState;
        }

        lua_State* const m_luaState;
    };

    struct MyObject
    {
        float x;
        float y;
        std::string name;

        NAU_CLASS_FIELDS(
            CLASS_FIELD(x),
            CLASS_FIELD(y),
            CLASS_FIELD(name)
        )
    };

    class NauTestService : public IRefCounted
    {
        NAU_CLASS_(NauTestService, IRefCounted)

        NAU_CLASS_ATTRIBUTES(
            CLASS_NAME_ATTRIBUTE("NauTestService")
        )

        NAU_CLASS_METHODS(
            CLASS_METHOD(NauTestService, testNumeric),
            CLASS_METHOD(NauTestService, testString),
            CLASS_METHOD(NauTestService, testCollection),
            CLASS_METHOD(NauTestService, testObject))

    public:
        unsigned testNumeric(unsigned value) const
        {
            return value;
        }

        std::string testString(const std::string& str) const
        {
            return str;
        }

        std::vector<unsigned> testCollection(std::vector<unsigned> values) const
        {
            return values;
        }

        MyObject testObject(MyObject value) const
        {
            return value;
        }
    };

    TEST_F(TestLuaInterop, MarshalPrimitiveValue)
    {
        const char* script = R"--(
            function testMain()
                local test = NauTestService:New()
                if test:testNumeric(77) ~= 77 then
                    return false;
                end

                if test:testString('lua_text') ~= 'lua_text' then
                    return false;
                end

                return true
            end

        )--";

        lua::initializeClass(getLua(), getClassDescriptor<NauTestService>(), false).ignore();

        load(script);
        call("testMain");

        const bool testResult = *lua::cast<bool>(getLua(), -1);
        ASSERT_TRUE(testResult);
    }

    TEST_F(TestLuaInterop, MarshalCollection)
    {
        const char* script = R"--(
            function testMain()
                local test = NauTestService:New()
                
                local res = test:testCollection({1,22,333});
                return res[1] == 1 and res[2] == 22 and res[3] == 333;
            end

        )--";

        lua::initializeClass(getLua(), getClassDescriptor<NauTestService>(), false).ignore();

        load(script);
        call("testMain");

        const bool testResult = *lua::cast<bool>(getLua(), -1);
        ASSERT_TRUE(testResult);
    }

    TEST_F(TestLuaInterop, MarshalObject)
    {
        const char* script = R"--(
            function testMain()
                local test = NauTestService:New()

                local obj = {}
                obj.x = 101
                obj.y = 202
                obj.name = 'from_lua'
                
                local res = test:testObject(obj);
                return res.x == 101 and res.y == 202 and res.name == 'from_lua'
            end

        )--";

        lua::initializeClass(getLua(), getClassDescriptor<NauTestService>(), false).ignore();

        load(script);
        call("testMain");

        const bool testResult = *lua::cast<bool>(getLua(), -1);
        ASSERT_TRUE(testResult);
    }

    TEST_F(TestLuaInterop, MarshalFunction)
    {

    }

}  // namespace nau::test
