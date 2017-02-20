#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <type_traits>
#include <functional>

namespace gelfcpp
{

struct GelfMessageBuilder
{
public:
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

private:
    GelfMessage message_;
};


}
