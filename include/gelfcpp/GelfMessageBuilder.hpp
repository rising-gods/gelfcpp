#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <type_traits>
#include <functional>

namespace gelfcpp
{

/**
 * \brief Builder class for GelfMessage instances.
 *
 * Allows convenient construction for GelfMessages with additional Decorator support.
 * The builder is implicitly convertible to a GelfMessage and can be directly passed to an Output.
 *
 * \note The internal GelfMessage is not copy-constructible, thus \code GelfMessage msg = builder; \endcode is invalid!
 *  But move-construction from an temporary GelfMessageBuilder is allowed.
 *
 * Example usage:
 * \code
 * GelfMessageBuilder builder;
 * builder("This is the short_message");
 * builder("field_1", true)("field_2", "chains are possible too!");
 *
 * output->Write(builder);
 * \endcode
 *
 * Decorators are also supported.
 * A decorator is a functional having the following syntax:
 * \code
 * void operator()(GelfMessage& message)
 * \endcode
 * This allows to dynamically set common values via a globally defined decorator.
 * Common use cases are e.g. adding the current timestamp, setting the "host" field.
 *
 * Example useage:
 * \code
 * auto add_common_fields = [](GelfMessage& message)
 * {
 *     message.SetTimestamp(std::time(nullptr));
 *     message.SetHost("my-host");
 * };
 * builder(add_common_fields);
 * \endcode
 */
struct GelfMessageBuilder
{
public:
#ifndef GELFCPP_DOXYGEN_RUNNING
    GelfMessageBuilder&& operator()(const std::string& message) &&
    {
        message_.SetMessage(message);
        return std::move(*this);
    }

    template<typename T>
    GelfMessageBuilder&& operator()(const std::string& field, T&& value) &&
    {
        message_.SetField(field, std::forward<T>(value));
        return std::move(*this);
    }

    GelfMessageBuilder& operator()(const std::string& message) &
    {
        message_.SetMessage(message);
        return *this;
    }

    template<typename T>
    GelfMessageBuilder& operator()(const std::string& field, T&& value) &
    {
        message_.SetField(field, std::forward<T>(value));
        return *this;
    }

    template<typename Decorator, typename = decltype(std::declval<Decorator>()(std::declval<GelfMessage&>()))>
    GelfMessageBuilder& operator()(Decorator&& decorator) &
    {
        std::forward<Decorator>(decorator)(message_);
        return *this;
    }

    template<typename Decorator, typename = decltype(std::declval<Decorator>()(std::declval<GelfMessage&>()))>
    GelfMessageBuilder&& operator()(Decorator&& decorator) &&
    {
        std::forward<Decorator>(decorator)(message_);
        return std::move(*this);
    }

    operator const GelfMessage&() const&
    {
        return message_;
    }

    operator GelfMessage&&() &&
    {
        return std::move(message_);
    }
#else
    /**
     * \brief Sets the short_message field
     *
     * Equivalent to \c gelf_message.SetMessage(message).
     *
     * \param message message
     * \return \c *this , for chaining
     * \see GelfMessage::SetMessage()
     */
    GelfMessageBuilder& operator()(const std::string& message) {}

    /**
     * \brief Sets a field to the given value
     *
     * Equivalent to \c gelf_message.SetField(field, value).
     *
     * \tparam T any supported field type, \see GelfMessage::SetField()
     * \param field field name
     * \param value field value
     * \return \c *this , for chaining
     * \see GelfMessage::SetField()
     */
    template<typename T> GelfMessageBuilder& operator()(const std::string& field, T value) {}

    /**
     * \brief Applies a decorator to the message
     *
     * Equivalent to \c decorator(gelf_message).
     *
     * \tparam Decorator decorator type
     * \param decorator decorator function
     * \return \c *this , for chaining
     */
    template<typename Decorator> GelfMessageBuilder& operator()(Decorator decorator) {}
#endif

private:
    GelfMessage message_;
};


}
