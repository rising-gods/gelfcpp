#pragma once

#include <gelfcpp/GelfMessageBuilder.hpp>
#include <memory>

namespace gelfcpp
{
namespace detail
{
template<typename T>
class Trigger
{
public:
    constexpr Trigger(T& output) :
            output_(output),
            triggered_(false)
    {}

    operator bool()
    {
        return !triggered_;
    }

    T* get()
    {
        triggered_ = true;
        return &output_;
    }

private:
    T& output_;
    bool triggered_;
};

template<typename T>
class Trigger<T*>
{
public:
    constexpr Trigger(T* output) :
            output_(output)
    {}

    operator bool()
    {
        return output_ != nullptr;
    }

    T* get()
    {
        auto old = output_;
        output_ = nullptr;
        return old;
    }

private:
    T* output_;
};

template<typename T>
class Trigger<std::shared_ptr<T>>
{
public:
    constexpr Trigger(std::shared_ptr<T> output) :
            output_(std::move(output)),
            triggered_(false)
    {}

    operator bool()
    {
        return !triggered_;
    }

    T* get()
    {
        triggered_ = true;
        return output_.get();
    }

private:
    std::shared_ptr<T> output_;
    bool triggered_;
};
}

template<typename T>
static GelfMessageBuilder<T> StreamMessage(T* output)
{
    return {output};
}
}

#define GELF_MESSAGE(output) \
    for (::gelfcpp::detail::Trigger<decltype(output)> trigger(output); trigger ;) \
        gelfcpp::StreamMessage(trigger.get())
