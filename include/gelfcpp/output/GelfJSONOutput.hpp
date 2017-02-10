#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/detail/DocumentAccessor.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace gelfcpp
{

class GelfJSONOutput
{
public:
    GelfJSONOutput(std::ostream& output) :
            output_(output)
    {}

    void Send(const GelfMessage& message)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        detail::DocumentAccessor::get(message).Accept(writer);

        output_.write(buffer.GetString(), buffer.GetSize());
    }

private:
    std::ostream& output_;
};

std::ostream& operator<<(std::ostream& os, const GelfMessage& message)
{
    GelfJSONOutput(os).Send(message);
    return os;
}

}
