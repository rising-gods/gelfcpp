#include <gelfcpp/GelfMessageStream.hpp>
#include <gelfcpp/output/GelfUDPOutput.hpp>
#include <gelfcpp/decorator/Timestamp.hpp>

int main()
{
    gelfcpp::GelfUDPOutput graylog("graylog.example.com", 12001);
    gelfcpp::decorator::CurrentTimestamp timestamp;

    GELF_MESSAGE(graylog)
                ("host", "localhost")
                (timestamp)
                ("I'm sending a message from gelfcpp")
                ("awesomeness", 1337);
}
