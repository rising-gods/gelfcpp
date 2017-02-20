#pragma once

#include <rapidjson/document.h>
#include <unordered_set>
#include <string>
#include <chrono>
#include <bits/unordered_set.h>


namespace gelfcpp
{

namespace detail
{
struct DocumentAccessor;
}

class GelfMessage
{
    friend struct detail::DocumentAccessor;

    struct FieldSetter
    {
        GelfMessage& owner;
        std::string field;

        template<typename T>
        void operator=(T&& value)
        {
            owner.SetField(field, std::forward<T>(value));
        }
    };

public:
    GelfMessage() :
            doc_(rapidjson::kObjectType)
    {
        SetField("version", "1.1");
    }

    void SetMessage(const std::string& message)
    {
        SetField("short_message", message);
    }

    void SetFullMessage(const std::string& message)
    {
        SetField("full_message", message);
    }

    void SetHost(const std::string& host)
    {
        SetField("host", host);
    }

    void SetTimestamp(double timestamp)
    {
        SetField("timestamp", timestamp);
    }

    template<typename T>
    auto SetField(const std::string& name, T value) -> std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value>
    {
        rapidjson::Value json(std::forward<T>(value));
        SetField(name, std::move(json));
    }

    void SetField(const std::string& name, bool value)
    {
        SetField(name, value ? 1 : 0);
    }

    void SetField(const std::string& name, const char* value)
    {
        rapidjson::Value json(value, doc_.GetAllocator());
        SetField(name, std::move(json));
    }

    void SetField(const std::string& name, const std::string& value)
    {
        rapidjson::Value json(value, doc_.GetAllocator());
        SetField(name, std::move(json));
    }

    FieldSetter operator[](const std::string& field)
    {
        return {*this, field};
    }

private:
    void SetField(const std::string& field, rapidjson::Value&& value)
    {
        static const std::unordered_set<std::string> BUILDIN_FIELDS{ "version", "host", "short_message", "full_message", "timestamp", "level" };

        rapidjson::Value key;
        if (BUILDIN_FIELDS.find(field) != BUILDIN_FIELDS.end())
            key.SetString(field.c_str(), static_cast<rapidjson::SizeType>(field.size()), doc_.GetAllocator());
        else
            key.SetString(("_" + field).c_str(), static_cast<rapidjson::SizeType>(field.size() + 1), doc_.GetAllocator());

        auto find = doc_.FindMember(key);
        if (find != doc_.MemberEnd())
            find->value = std::move(value);
        else
            doc_.AddMember(std::move(key), std::move(value), doc_.GetAllocator());
    }

private:
    rapidjson::Document doc_;
};

}
