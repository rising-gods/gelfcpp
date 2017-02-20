#pragma once

#include <gelfcpp/GelfMessageBuilder.hpp>
#include <memory>

namespace gelfcpp
{
namespace detail
{
template<typename T>
class Sender
{
public:
    static bool IsValid(const T&)
    {
        return true;
    }

    Sender(T& output) :
        output_(output)
    {}

    void operator=(const GelfMessage& message)
    {
        output_.Write(message);
    }
private:
    T& output_;
};

template<typename T>
class Sender<T*>
{
public:
    static bool IsValid(const T* output)
    {
        return output != nullptr;
    }

    Sender(T* output) :
        output_(output)
    {}

    void operator=(const GelfMessage& message)
    {
        output_->Write(message);
    }
private:
    T* output_;
};

template<typename T>
class Sender<std::shared_ptr<T>>
{
public:
    static bool IsValid(const std::shared_ptr<T>& output)
    {
        return output != nullptr;
    }

    Sender(std::shared_ptr<T> output) :
        output_(std::move(output))
    {}

    void operator=(const GelfMessage& message)
    {
        output_->Write(message);
    }
private:
    std::shared_ptr<T> output_;
};

template<typename T, typename Deleter>
class Sender<std::unique_ptr<T, Deleter>>
{
public:
    static bool IsValid(const std::unique_ptr<T, Deleter>& output)
    {
        return output != nullptr;
    }

    Sender(const std::unique_ptr<T, Deleter>& output) :
        output_(output.get())
    {}

    void operator=(const GelfMessage& message)
    {
        output_->Write(message);
    }
private:
    T* output_;
};
}

struct GelfMessageStream
{
public:
    template<typename T>
    explicit GelfMessageStream(T&& ptr) :
            valid_(detail::Sender<std::decay_t<T>>::IsValid(ptr))
    {}

    explicit operator bool()
    {
        return valid_;
    }

    template<typename T>
    detail::Sender<T> Send(T& output)
    {
        valid_ = false;
        return {output};
    }

private:
    bool valid_;
};
}

#define GELF_MESSAGE(output) \
    for (::gelfcpp::GelfMessageStream stream(output); stream ;) \
        stream.Send(output) = ::gelfcpp::GelfMessageBuilder()
