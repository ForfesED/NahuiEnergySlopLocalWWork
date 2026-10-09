// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./json_to_runtime_value.h"

#include "nau/io/stream.h"
#include "nau/memory/bytes_buffer.h"
#include "nau/serialization/json.h"
#include "nau/serialization/runtime_value_builder.h"

namespace nau::json_detail
{

    RuntimeValue::Ptr wrapJsonValueAsCollection(const Ptr<JsonValueHolderImpl>& root, Json::Value& jsonValue);

    RuntimeValue::Ptr wrapJsonValueAsDictionary(const Ptr<JsonValueHolderImpl>& root, Json::Value& jsonValue);

    RuntimeValue::Ptr createJsonNullValue(const Ptr<JsonValueHolderImpl>& root);

    Result<> setJsonValue(Json::Value& jsonValue, RuntimeValue& rtValue)
    {
        if (auto* const optValue = rtValue.as<RuntimeOptionalValue*>())
        {
            if (optValue->hasValue())
            {
                NauCheckResult(setJsonValue(jsonValue, *optValue->getValue()));
            }
            else
            {
                jsonValue = Json::Value::nullSingleton();
            }
        }
        else if (const auto* const intValue = rtValue.as<const RuntimeIntegerValue*>())
        {
            const bool isLong = intValue->getBitsCount() == sizeof(int64_t);

            if (intValue->isSigned())
            {
                const int64_t i = intValue->getInt64();
                jsonValue = isLong ? Json::Value{static_cast<Json::Int64>(i)} : Json::Value{static_cast<Json::Int>(i)};
            }
            else
            {
                const uint64_t i = intValue->getUint64();
                jsonValue = isLong ? Json::Value{static_cast<Json::UInt64>(i)} : Json::Value{static_cast<Json::UInt>(i)};
            }
        }
        else if (const auto* const floatValue = rtValue.as<const RuntimeFloatValue*>())
        {
            jsonValue = Json::Value{floatValue->getDouble()};
        }
        else if (const auto* const boolValue = rtValue.as<const RuntimeBooleanValue*>())
        {
            jsonValue = Json::Value{boolValue->getBool()};
        }
        else if (const auto* const strValue = rtValue.as<RuntimeStringValue*>())
        {
            jsonValue = Json::Value{strValue->getString()};
        }
        else if (auto* coll = rtValue.as<RuntimeReadonlyCollection*>(); coll)
        {
            if (jsonValue.type() != Json::ValueType::arrayValue)
            {
                jsonValue = Json::Value{Json::ValueType::arrayValue};
            }

            for (size_t i = 0, size = coll->getSize(); i < size; ++i)
            {
                Json::Value elementJsonValue;

                if (RuntimeValue::Ptr elementValue = coll->getAt(i))
                {
                    NauCheckResult(setJsonValue(elementJsonValue, *elementValue));
                }

                jsonValue.append(std::move(elementJsonValue));
            }
        }
        else if (auto* dict = rtValue.as<RuntimeReadonlyDictionary*>(); dict)
        {
            if (jsonValue.type() != Json::ValueType::objectValue)
            {
                jsonValue = Json::Value{Json::ValueType::objectValue};
            }

            for (size_t i = 0, size = dict->getSize(); i < size; ++i)
            {
                const auto [key, fieldValue] = (*dict)[i];

                Json::Value* const field = jsonValue.demand(key.data(), key.data() + key.size());
                NAU_FATAL(field);

                if (fieldValue)
                {
                    NauCheckResult(setJsonValue(*field, *fieldValue));
                }
                else
                {
                    *field = Json::Value::nullSingleton();
                }
            }
        }

        return ResultSuccess;
    }

    RuntimeValue::Ptr getValueFromJson(const nau::Ptr<JsonValueHolderImpl>& root, Json::Value& jsonValue)
    {
        if (jsonValue.isNull())
        {
            return createJsonNullValue(root);
        }
        else if (jsonValue.isUInt())
        {
            return makeValueCopy(jsonValue.asUInt());
        }
        else if (jsonValue.isInt())
        {
            return makeValueCopy(jsonValue.asInt());
        }
        else if (jsonValue.isUInt64())
        {
            return makeValueCopy(jsonValue.asUInt64());
        }
        else if (jsonValue.isInt64())
        {
            return makeValueCopy(jsonValue.asInt64());
        }
        else if (jsonValue.isDouble())
        {
            return makeValueCopy(jsonValue.asDouble());
        }
        else if (jsonValue.isBool())
        {
            return makeValueCopy(jsonValue.asBool());
        }
        else if (jsonValue.isString())
        {
            std::string str = jsonValue.asString();

            if (root)
            {
                if (auto newString = root->transformString(strings::toStringView(str)); newString)
                {
                    return makeValueCopy(std::move(*newString));
                }
            }

            return makeValueCopy(std::move(str));
        }
        else if (jsonValue.isArray())
        {
            return wrapJsonValueAsCollection(root, jsonValue);
        }
        else if (jsonValue.isObject())
        {
            return wrapJsonValueAsDictionary(root, jsonValue);
        }

        NAU_FAILURE("Don't known how to encode jsonValue. Type: ({})", static_cast<int>(jsonValue.type()));
        return nullptr;
    }

    /**
     */
    class JsonNull final : public JsonValueHolderImpl,
                           public RuntimeOptionalValue
    {
        NAU_CLASS_(nau::json_detail::JsonNull, JsonValueHolderImpl, RuntimeOptionalValue)

    public:
        JsonNull(const nau::Ptr<JsonValueHolderImpl>& root) :
            JsonValueHolderImpl(root)
        {
        }

        bool isMutable() const override
        {
            return false;
        }

        bool hasValue() const override
        {
            return false;
        }

        RuntimeValue::Ptr getValue() override
        {
            return nullptr;
        }

        Result<> setValue([[maybe_unused]] RuntimeValue::Ptr value) override
        {
            return NauMakeError("Attempt to modify non mutable json value");
        }
    };

    /**
     */
    class JsonCollection final : public JsonValueHolderImpl,
                                 public RuntimeCollection
    {
        NAU_CLASS_(nau::json_detail::JsonCollection, JsonValueHolderImpl, RuntimeCollection)

    public:
        JsonCollection() = default;

        JsonCollection(Json::Value&& value) :
            JsonValueHolderImpl(std::move(value))
        {
        }

        JsonCollection(Json::Value& value) :
            JsonValueHolderImpl(value)
        {
        }

        JsonCollection(const nau::Ptr<JsonValueHolderImpl>& root, Json::Value& value) :
            JsonValueHolderImpl(root, value)
        {
        }

        bool isMutable() const override
        {
            return m_isMutable;
        }

        size_t getSize() const override
        {
            const auto& jsonValue = getThisJsonValue();
            NAU_ASSERT(jsonValue.type() == Json::ValueType::arrayValue);
            return static_cast<size_t>(jsonValue.size());
        }

        RuntimeValue::Ptr getAt(size_t index) override
        {
            NAU_ASSERT(getThisJsonValue().type() == Json::ValueType::arrayValue);
            NAU_ASSERT(index < getSize(), "Invalid index [{}]", index);
            if (index >= getSize())
            {
                return nullptr;
            }

            const auto arrIndex = static_cast<Json::ArrayIndex>(index);
            return getValueFromJson(getRoot(), getThisJsonValue()[arrIndex]);
        }

        Result<> setAt(size_t index, const RuntimeValue::Ptr& value) override
        {
            NAU_ASSERT(value);
            if (!value)
            {
                return NauMakeError("Value is null");
            }

            NAU_ASSERT(index < getSize());
            if (index >= getSize())
            {
                return NauMakeError("Invalid index ({})", index);
            }

            const auto arrIndex = static_cast<Json::ArrayIndex>(index);
            return setJsonValue(getThisJsonValue()[arrIndex], *value);
        }

        void clear() override
        {
            getThisJsonValue().clear();
        }

        void reserve([[maybe_unused]] size_t capacity) override
        {
        }

        Result<> append(const RuntimeValue::Ptr& value) override
        {
            NAU_ASSERT(getThisJsonValue().type() == Json::ValueType::arrayValue);
            NAU_ASSERT(value);
            if (!value)
            {
                return NauMakeError("Value is null");
            }

            Json::Value newValue;
            NauCheckResult(setJsonValue(newValue, *value));
            getThisJsonValue().append(std::move(newValue));

            return ResultSuccess;
        }
    };

    /**
     */
    class JsonDictionary : public JsonValueHolderImpl,
                           public RuntimeDictionary
    {
        NAU_CLASS_(nau::json_detail::JsonDictionary, JsonValueHolderImpl, RuntimeDictionary)
    public:
        JsonDictionary() = default;

        JsonDictionary(Json::Value&& value) :
            JsonValueHolderImpl(std::move(value))
        {
        }

        JsonDictionary(Json::Value& value) :
            JsonValueHolderImpl(value)
        {
        }

        JsonDictionary(const nau::Ptr<JsonValueHolderImpl>& root, Json::Value& value) :
            JsonValueHolderImpl(root, value)
        {
        }

        bool isMutable() const override
        {
            return m_isMutable;
        }

        size_t getSize() const override
        {
            const auto& jsonValue = getThisJsonValue();
            NAU_ASSERT(jsonValue.type() == Json::ValueType::objectValue);
            return static_cast<size_t>(jsonValue.size());
        }

        std::string_view getKey(size_t index) const override
        {
            const auto& jsonValue = getThisJsonValue();
            NAU_ASSERT(jsonValue.type() == Json::ValueType::objectValue);
            NAU_ASSERT(index < jsonValue.size(), "Invalid index ({}) > size:({})", index, jsonValue.size());

            auto iter = jsonValue.begin();
            std::advance(iter, index);
            const char* end;
            const char* const start = iter.memberName(&end);
            return std::string_view{start, static_cast<size_t>(end - start)};
        }

        RuntimeValue::Ptr getValue(std::string_view key) override
        {
            if (key.empty())
            {
                return nullptr;
            }

            auto& jsonValue = getThisJsonValue();
            NAU_ASSERT(jsonValue.type() == Json::ValueType::objectValue);

            if (jsonValue.find(key.data(), key.data() + key.size()) == nullptr)
            {
                return nullptr;
            }

            // Value::demand will insert empty default value, so it used only after find.
            Json::Value* const field = jsonValue.demand(key.data(), key.data() + key.size());
            NAU_FATAL(field);

            return getValueFromJson(getRoot(), *field);
        }

        Result<> setValue(std::string_view key, const RuntimeValue::Ptr& value) override
        {
            NAU_ASSERT(value);
            if (!value)
            {
                return NauMakeError("Value is null");
            }

            NAU_ASSERT(!key.empty());
            if (key.empty())
            {
                return NauMakeError("key is empty");
            }

            Json::Value* const field = getThisJsonValue().demand(key.data(), key.data() + key.size());
            NAU_FATAL(field);

            return setJsonValue(*field, *value);
        }

        bool containsKey(std::string_view key) const override
        {
            if (key.empty())
            {
                return false;
            }

            NAU_ASSERT(getThisJsonValue().type() == Json::ValueType::objectValue);

            return getThisJsonValue().find(key.data(), key.data() + key.size()) != nullptr;
        }

        void clear() override
        {
            NAU_ASSERT(getThisJsonValue().type() == Json::ValueType::objectValue);
            getThisJsonValue().clear();
        }

        RuntimeValue::Ptr erase(std::string_view key) override
        {
            NAU_FAILURE("JsonDictionary::erase({}) is not implemented", key);
            return nullptr;
        }
    };

    RuntimeValue::Ptr wrapJsonValueAsCollection(const Ptr<JsonValueHolderImpl>& root, Json::Value& jsonValue)
    {
        return rtti::createInstance<JsonCollection>(root, jsonValue);
    }

    RuntimeValue::Ptr wrapJsonValueAsDictionary(const Ptr<JsonValueHolderImpl>& root, Json::Value& jsonValue)
    {
        return rtti::createInstance<JsonDictionary>(root, jsonValue);
    }

    RuntimeValue::Ptr createJsonNullValue(const Ptr<JsonValueHolderImpl>& root)
    {
        return rtti::createInstance<JsonNull>(root);
    }

    RuntimeDictionary::Ptr createJsonDictionary(Json::Value&& jsonValue)
    {
        return rtti::createInstance<JsonDictionary>(std::move(jsonValue));
    }

    RuntimeCollection::Ptr createJsonCollection(Json::Value&& jsonValue)
    {
        return rtti::createInstance<JsonCollection>(std::move(jsonValue));
    }

    RuntimeDictionary::Ptr wrapJsonDictionary(Json::Value& jsonValue)
    {
        return rtti::createInstance<JsonDictionary>(jsonValue);
    }

    RuntimeCollection::Ptr wrapJsonCollection(Json::Value& jsonValue)
    {
        return rtti::createInstance<JsonCollection>(jsonValue);
    }

}  // namespace nau::json_detail

namespace nau::serialization
{
    RuntimeValue::Ptr jsonToRuntimeValue(Json::Value&& root, IMemAllocator::Ptr)
    {
        if (root.isObject())
        {
            return json_detail::createJsonDictionary(std::move(root));
        }
        else if (root.isArray())
        {
            return json_detail::createJsonCollection(std::move(root));
        }

        return json_detail::getValueFromJson(nullptr, root);
    }

    RuntimeValue::Ptr jsonAsRuntimeValue(Json::Value& root, IMemAllocator::Ptr)
    {
        if (root.isObject())
        {
            return json_detail::wrapJsonDictionary(root);
        }
        else if (root.isArray())
        {
            return json_detail::wrapJsonCollection(root);
        }

        return nullptr;
    }

    RuntimeValue::Ptr jsonAsRuntimeValue(const Json::Value& root, IMemAllocator::Ptr)
    {
        auto value = jsonAsRuntimeValue(const_cast<Json::Value&>(root));
        value->as<json_detail::JsonValueHolderImpl&>().setMutable(false);

        return value;
    }
}  // namespace nau::serialization
