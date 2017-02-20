#include <gelfcpp/output/GelfUDPOutput.hpp>
#include <gtest/gtest.h>

using namespace gelfcpp;
using namespace std::string_literals;

TEST(DISABLED_TestGelfUDPOutput, Write)
{
    output::GelfUDPOutput output("localhost", 12222);
    {
        GelfMessage message;
        message.SetMessage("message");
        message.SetHost("host");

        output.Write(message);
    }
//    EXPECT_EQ(os.str(), R"({"version":"1.1","short_message":"message","host":"host"})");
    {
        GelfMessage message;
        message.SetMessage("message 2");
        message.SetHost("host");

        output.Write(message);
    }
//    EXPECT_EQ(os.str(), R"({"version":"1.1","short_message":"message","host":"host"}{"version":"1.1","short_message":"message 2","host":"host"})");
}
