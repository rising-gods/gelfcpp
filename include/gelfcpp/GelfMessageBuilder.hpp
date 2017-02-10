#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <type_traits>
#include <functional>

namespace gelfcpp
{

template<typename Output>
struct GelfMessageBuilder
{
public:
    GelfMessageBuilder(Output* output) :
            output_(output)
    {}

    ~GelfMessageBuilder()
    {
        if (output_)
            output_->Send(message_);
    }

    GelfMessageBuilder& Set()
    {
        return operator()();
    }

    GelfMessageBuilder& Set(const std::string& message)
    {
        return operator()(message);
    }

    template<typename T>
    GelfMessageBuilder& Set(const std::string& field, T&& value)
    {
        return operator()(field, std::forward<T>(value));
    }

    template<typename Fn, typename enable = decltype(std::declval<Fn>()(std::declval<GelfMessage&>()))>
    GelfMessageBuilder& Set(Fn&& provider)
    {
        return operator()(std::forward<Fn>(provider));
    };

    GelfMessageBuilder& operator()()
    {
        return *this;
    }

    GelfMessageBuilder& operator()(const std::string& message)
    {
        message_.SetMessage(message);
        return *this;
    }

    template<typename T>
    GelfMessageBuilder& operator()(const std::string& field, T&& value)
    {
        message_.Set(field, std::forward<T>(value));
        return *this;
    }

    template<typename Fn, typename = decltype(std::declval<Fn>()(std::declval<GelfMessage&>()))>
    GelfMessageBuilder& operator()(Fn&& provider)
    {
        std::forward<Fn>(provider)(message_);
        return *this;
    }

    operator const GelfMessage&() const
    {
        return message_;
    }

private:
    Output* output_;
    GelfMessage message_;
};


}
