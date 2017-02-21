#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/detail/DocumentAccessor.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace gelfcpp
{
namespace output
{
/**
 * \brief Writes GelfMessages as JSON to given std::ostream
 *
 * Uses short notation, which minimizes the space requirements
 * No newline is appended, the output is not flushed.
 */
class GelfJSONOutput
{
public:
    /**
     * \brief Constructs a new output on the given stream
     *
     * \param output underlying stream
     */
    GelfJSONOutput(std::ostream& output) :
            output_(output) {}

    /**
     * \brief Serializes a message to the underlying stream
     *
     * \param message the message
     */
    void Write(const GelfMessage& message)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        detail::DocumentAccessor::get(message).Accept(writer);

        output_.write(buffer.GetString(), buffer.GetSize());
    }

private:
    std::ostream& output_;
};

}

/**
 * \brief Serializes a message to the given stream
 *
 * \param os the stream
 * \param message the message
 * \return the stream
 * \see GelfJSONOutput
 */
inline std::ostream& operator<<(std::ostream& os, const GelfMessage& message)
{
    output::GelfJSONOutput(os).Write(message);
    return os;
}

}
