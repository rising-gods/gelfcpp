#pragma once

#include <rapidjson/document.h>
#include <set>
#include <string>
#include <chrono>


namespace gelfcpp
{

namespace detail
{
struct DocumentAccessor;
}

class GelfMessage
{
    friend struct detail::DocumentAccessor;

public:
    GelfMessage() :
            doc_(rapidjson::kObjectType)
    {
        doc_.AddMember(rapidjson::Value("version", 7), rapidjson::Value("1.1", 3), doc_.GetAllocator());
    }

    void SetMessage(const std::string& message)
    {
        Set("short_message", message);
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic<std::decay_t<T>>::value && !std::is_same<std::decay_t<T>, bool>::value>>
    void Set(const std::string& field, T&& value)
    {
        rapidjson::Value json(value);
        Set(field, json);
    }

    template<typename T, typename = std::enable_if_t<std::is_same<std::decay_t<T>, bool>::value>>
    void Set(const std::string& field, T value)
    {
        Set(field, value ? 1 : 0);
    }

    void Set(const std::string& field, const std::string& value)
    {
        rapidjson::Value json(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), doc_.GetAllocator());
        Set(field, json);
    }

    void Set(const std::string& field, rapidjson::Value& value)
    {
        static const std::set<std::string> BUILDIN_FIELDS{ "version", "host", "short_message", "full_message", "timestamp", "level" };

        rapidjson::Value key;
        if (BUILDIN_FIELDS.find(field) != BUILDIN_FIELDS.end())
            key.SetString(field.c_str(), static_cast<rapidjson::SizeType>(field.size()), doc_.GetAllocator());
        else
            key.SetString(("_" + field).c_str(), static_cast<rapidjson::SizeType>(field.size() + 1), doc_.GetAllocator());

        doc_.AddMember(key, value, doc_.GetAllocator());
    }

    void Set(const std::string& field, const rapidjson::Value& value)
    {
        rapidjson::Value copy(value, doc_.GetAllocator());
        Set(field, copy);
    }

private:
    rapidjson::Document doc_;
};

}
