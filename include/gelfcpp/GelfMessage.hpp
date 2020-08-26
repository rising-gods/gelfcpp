#pragma once

#include <rapidjson/document.h>
#include <unordered_set>
#include <string>
#include <chrono>


namespace gelfcpp
{

namespace detail
{
struct DocumentAccessor;
}

/**
 * \brief A single GELF message, with write-only access to its fields.
 *
 * \note This class only supports GELF version 1.1 and treats deprecated standard fields as normal additional fields.
 *
 * Currently the following standard fields are handled: <code>version, host, short_message, full_message, timestamp, level</code>.
 * Standard fields are included "as-is". All other fields are prefixed with "_".
 */
class GelfMessage
{
    friend struct detail::DocumentAccessor;

#ifdef __GNUC__
// RAPIDJSON_DIAG_OFF(effc++)
#define RAPIDJSON_DIAG_WARNING(x) \
    RAPIDJSON_DIAG_PRAGMA(warning RAPIDJSON_STRINGIFY(RAPIDJSON_JOIN(-W,x)))
// ! FIXME - Avoid error on GCC 8.3.0 when `-Werror=effc++` is active !
RAPIDJSON_DIAG_WARNING(effc++)
#endif // __GNUC__

    struct FieldSetter
    {
        GelfMessage& owner;
        std::string field;

        // 
        // ! FIXME - Still warning/error on `-Weffc++` on GCC 8.3.0 !
        //               * error: ‘operator=’ should return a reference to ‘*this’ [-Werror=effc++]
        //           @see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84364
        // 
        template<typename T>
        FieldSetter& operator=(T&& value)
        {
            owner.SetField(field, std::forward<T>(value));
            return *this;
        }
    };

#ifdef __GNUC__
RAPIDJSON_DIAG_POP
#endif // __GNUC__

public:
    GelfMessage() :
            doc_(rapidjson::kObjectType)
    {
        SetField("version", "1.1");
    }

    /**
     * \brief Quick accessor for <code>short_message</code> field.
     *
     * Behaves like \code SetField("short_message", message). \endcode
     *
     * \param message message
     */
    void SetMessage(const std::string& message)
    {
        SetField("short_message", message);
    }

    /**
     * \brief Quick accessor for <code>full_message</code> field.
     *
     * Behaves like \code SetField("full_message", message). \endcode
     *
     * \param message message
     */
    void SetFullMessage(const std::string& message)
    {
        SetField("full_message", message);
    }

    /**
     * \brief Quick accessor for <code>host</code> field.
     *
     * Behaves like \code SetField("host", message). \endcode
     *
     * \param host hostname
     */
    void SetHost(const std::string& host)
    {
        SetField("host", host);
    }

    /**
     * \brief Quick accessor for <code>timestamp</code> field.
     *
     * Behaves like \code SetField("timestamp", message). \endcode
     *
     * \param timestamp timestamp
     */
    void SetTimestamp(double timestamp)
    {
        SetField("timestamp", timestamp);
    }

#ifndef GELFCPP_DOXYGEN_RUNNING

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

#else
    /**
     * \brief Adds a field to the message.
     *
     * \note If a field was already assigned the old value is overwritten.
     *
     * \tparam T value type, supported: <code>bool, integral, floating point, strings</code>
     * \param name field name, the "_" prefix is added automatically
     * \param value field value of any supported type
     */
    template<typename T> void SetField(const std::string& name, T value) {}
#endif

    /**
     * \brief Adds a field to the message.
     *
     * This behaves like SetField(const std::string&, T) but allows map/array like syntax.
     *
     * Example usage:
     * \code{.cpp}
     * GelfMessage message;
     * message["field_name"] = field_value;
     * \endcode
     *
     * \param field field name, the "_" prefix is added automatically
     * \return wrapper to allow assignment, see the usage example.
     */
    FieldSetter operator[](const std::string& field)
    {
        return { *this, field };
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
