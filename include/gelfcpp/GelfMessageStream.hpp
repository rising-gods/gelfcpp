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

/**
 * \brief Allows stream like sending of GelfMessage
 *
 * Usually used in conjunction with a convenience macro like GELF_MESSAGE to allow conditional sending.
 *
 * Allows contextual-conversion to bool to check wether the output is valid.
 * An output is valid if it can accept messages, meaning:
 * * output is a reference to an output type
 * * output is a non-null pointer to an output type
 * * output is a valid smart pointer (shared, unique) to an output type
 *
 * \note
 *  Send explicitly requires to provide the output again, since it is not stored inside the stream.
 *  This is due to the need to support arbitrary, non-virtual output types.
 *
 * Usage example:
 * \code
 * GelfMessageStream stream(output);
 * if (stream)
 * {
 *     stream.Send(output) = message;
 *     // or
 *     stream.Send(output) << message_1 << message_2;
 * }
 * \endcode
 */
struct GelfMessageStream
{
public:
    /**
     * \brief Creates a new stream
     *
     * Checks wether the output is valid and thus invalidates this stream.
     *
     * @tparam T output type
     * @param output output
     */
    template<typename T>
    explicit GelfMessageStream(T&& output) :
            valid_(detail::Sender<std::decay_t<T>>::IsValid(output))
    {}

    /**
     * \brief Checks if this stream is associated to a valid output
     *
     * @return \c true if output is valid, \c false otherwise
     */
    explicit operator bool()
    {
        return valid_;
    }

    /**
     * \brief Opens a stream to an output
     *
     * \note After calling Send() once this stream is considered invalid.
     *
     * @tparam T output type
     * @param output output
     * @return Sink for GelfMessages, which are send to the output
     */
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

/**
 * \ingroup Marcos
 * \brief Convenience macro for easy stream-like creation and sending of GelfMessages
 *
 * Usage:
 * \code
 * GELF_MESSAGE()("short_message")("field_1", value);
 * \endcode
 */
#define GELF_MESSAGE(output) \
    for (::gelfcpp::GelfMessageStream stream(output); stream ;) \
        stream.Send(output) = ::gelfcpp::GelfMessageBuilder()
