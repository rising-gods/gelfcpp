#include <gelfcpp/GelfMessage.hpp>
#include <gtest/gtest.h>

using namespace gelfcpp;
using namespace std::string_literals;

TEST(TestGelfMessageCompile, SetMessage)
{
    GelfMessage message;

    message.SetMessage("Test cstring");
    message.SetMessage("Test std::string"s);
}

TEST(TestGelfMessageCompile, SetFieldString)
{
    GelfMessage message;

    message.Set("Field cstring", "Value cstring");
    message.Set("Field std::string"s, "Value cstring");
    message.Set("Field cstring", "Value std::string"s);
    message.Set("Field std::string"s, "Value std::string"s);
}

TEST(TestGelfMessageCompile, SetFieldFundamental)
{
    GelfMessage message;
    {
        message.Set("Field uint8", uint8_t(0));
        message.Set("Field uint16", uint16_t(0));
        message.Set("Field uint32", uint32_t(0));
        message.Set("Field uint64", uint64_t(0));
        message.Set("Field int8", int8_t(0));
        message.Set("Field int16", int16_t(0));
        message.Set("Field int32", int32_t(0));
        message.Set("Field int64", int64_t(0));
    }
    {
        message.Set("Field float", float(0));
        message.Set("Field double", double(0));
    }
}
