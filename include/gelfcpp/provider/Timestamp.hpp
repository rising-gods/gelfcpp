#pragma once

#include <gelfcpp/GelfMessage.hpp>


namespace gelfcpp
{
namespace provider
{

class CurrentTimestamp
{
public:
    static double GetTime()
    {
        using clock = std::chrono::system_clock;
        using unixtime = std::chrono::duration<double>;
        return std::chrono::duration_cast<unixtime>(clock::now().time_since_epoch()).count();
    }

    void operator()(GelfMessage& message) const
    {
        message.Set("timestamp", GetTime());
    }
};

}
}
