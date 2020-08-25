#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <memory>


namespace gelfcpp
{
namespace detail
{

template<typename T>
class Sender
{
public:
    Sender(const Sender&) = delete;
    Sender& operator=(const Sender&) = delete;

    static bool IsValid(const T&)
    {
        return true;
    }

    Sender(T& output) :
            output_(output) {}

    Sender& operator=(const GelfMessage& message)
    {
        output_.Write(message);
        return *this;
    }

private:
    T& output_;
};

template<typename T>
class Sender<T*>
{
public:
    Sender(const Sender&) = delete;
    Sender& operator=(const Sender&) = delete;

    static bool IsValid(const T* output)
    {
        return output != nullptr;
    }

    Sender(T* output) :
            output_(output) {}

    Sender& operator=(const GelfMessage& message)
    {
        output_->Write(message);
        return *this;
    }

private:
    T* output_;
};

template<typename T>
class Sender<std::shared_ptr<T>>
{
public:
    Sender(const Sender&) = delete;
    Sender& operator=(const Sender&) = delete;

    static bool IsValid(const std::shared_ptr<T>& output)
    {
        return output != nullptr;
    }

    Sender(const std::shared_ptr<T>& output) :
            output_(output) {}

    Sender& operator=(const GelfMessage& message)
    {
        output_->Write(message);
        return *this;
    }

private:
    const std::shared_ptr<T>& output_;
};

template<typename T, typename Deleter>
class Sender<std::unique_ptr<T, Deleter>>
{
public:
    Sender(const Sender&) = delete;
    Sender& operator=(const Sender&) = delete;

    static bool IsValid(const std::unique_ptr<T, Deleter>& output)
    {
        return output != nullptr;
    }

    Sender(const std::unique_ptr<T, Deleter>& output) :
            output_(output) {}

    Sender& operator=(const GelfMessage& message)
    {
        output_->Write(message);
        return *this;
    }

private:
    const std::unique_ptr<T, Deleter>& output_;
};

}
}
