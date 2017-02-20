#include <gelfcpp/output/GelfJSONOutput.hpp>
#include <gtest/gtest.h>

using namespace gelfcpp;
using namespace std::string_literals;

TEST(TestGelfJSONOutput, Write)
{
    std::ostringstream os;
    output::GelfJSONOutput output(os);
    {
        GelfMessage message;
        message.SetMessage("message");
        message.SetHost("host");

        output.Write(message);
    }
    EXPECT_EQ(os.str(), R"({"version":"1.1","short_message":"message","host":"host"})");
    {
        GelfMessage message;
        message.SetMessage("message 2");
        message.SetHost("host");

        output.Write(message);
    }
    EXPECT_EQ(os.str(), R"({"version":"1.1","short_message":"message","host":"host"}{"version":"1.1","short_message":"message 2","host":"host"})");
}

TEST(TestGelfJSONOutput, Stream)
{
    std::ostringstream os;
    {
        GelfMessage message;
        message.SetMessage("message");
        message.SetHost("host");

        os << message;
    }
    EXPECT_EQ(os.str(), R"({"version":"1.1","short_message":"message","host":"host"})");
    {
        GelfMessage message;
        message.SetMessage("message 2");
        message.SetHost("host");

        os << message;
    }
    EXPECT_EQ(os.str(), R"({"version":"1.1","short_message":"message","host":"host"}{"version":"1.1","short_message":"message 2","host":"host"})");
}
