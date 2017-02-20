#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/detail/DocumentAccessor.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <gtest/gtest.h>

using namespace gelfcpp;
using namespace std::string_literals;

namespace
{
std::string Serialize(const GelfMessage& message)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    detail::DocumentAccessor::get(message).Accept(writer);

    return buffer.GetString();
}
}

TEST(TestGelfMessage, Empty)
{
    {
        GelfMessage message;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1"})");
    }
}

TEST(TestGelfMessage, SetTimestamp)
{
    {
        GelfMessage message;
        message.SetTimestamp(1234.56789);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","timestamp":1234.56789})");
    }
    {
        GelfMessage message;
        message["timestamp"] = 1234.56789;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","timestamp":1234.56789})");
    }
}

TEST(TestGelfMessage, SetMessage)
{
    {
        GelfMessage message;
        message.SetMessage("cstring");
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","short_message":"cstring"})");
    }
    {
        GelfMessage message;
        message.SetMessage("std::string"s);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","short_message":"std::string"})");
    }
    {
        GelfMessage message;
        message["short_message"] = "cstring";
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","short_message":"cstring"})");
    }
    {
        GelfMessage message;
        message["short_message"] = "std::string"s;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","short_message":"std::string"})");
    }
}

TEST(TestGelfMessage, SetFullMessage)
{
    {
        GelfMessage message;
        message.SetFullMessage("cstring");
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","full_message":"cstring"})");
    }
    {
        GelfMessage message;
        message.SetFullMessage("std::string"s);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","full_message":"std::string"})");
    }
    {
        GelfMessage message;
        message["full_message"] = "cstring";
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","full_message":"cstring"})");
    }
    {
        GelfMessage message;
        message["full_message"] = "std::string"s;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","full_message":"std::string"})");
    }
}

TEST(TestGelfMessage, SetHost)
{
    {
        GelfMessage message;
        message.SetHost("cstring");
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","host":"cstring"})");
    }
    {
        GelfMessage message;
        message.SetHost("std::string"s);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","host":"std::string"})");
    }
    {
        GelfMessage message;
        message["host"] = "cstring";
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","host":"cstring"})");
    }
    {
        GelfMessage message;
        message["host"] = "std::string"s;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","host":"std::string"})");
    }
}

TEST(TestGelfMessage, SetFieldString)
{
    {
        GelfMessage message;
        message.SetField("cstring", "cstring");
        message.SetField("std::string"s, "cstring");
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_cstring":"cstring","_std::string":"cstring"})");
    }
    {
        GelfMessage message;
        message.SetField("cstring", "std::string"s);
        message.SetField("std::string"s, "std::string"s);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_cstring":"std::string","_std::string":"std::string"})");
    }
    {
        GelfMessage message;
        message["cstring"] = "cstring";
        message["std::string"s] = "cstring";
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_cstring":"cstring","_std::string":"cstring"})");
    }
    {
        GelfMessage message;
        message["cstring"] = "std::string"s;
        message["std::string"s] = "std::string"s;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_cstring":"std::string","_std::string":"std::string"})");
    }
}

TEST(TestGelfMessage, SetFieldIntegral)
{
    {
        GelfMessage message;
        message.SetField("uint8", uint8_t(1));
        message.SetField("uint16", uint16_t(2));
        message.SetField("uint32", uint32_t(3));
        message.SetField("uint64", uint64_t(4));
        message.SetField("int8", int8_t(-5));
        message.SetField("int16", int16_t(-6));
        message.SetField("int32", int32_t(-7));
        message.SetField("int64", int64_t(-8));
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_uint8":1,"_uint16":2,"_uint32":3,"_uint64":4,"_int8":-5,"_int16":-6,"_int32":-7,"_int64":-8})");
    }
    {
        GelfMessage message;
        message["uint8"] = uint8_t(1);
        message["uint16"] = uint16_t(2);
        message["uint32"] = uint32_t(3);
        message["uint64"] = uint64_t(4);
        message["int8"] = int8_t(-5);
        message["int16"] = int16_t(-6);
        message["int32"] = int32_t(-7);
        message["int64"] = int64_t(-8);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_uint8":1,"_uint16":2,"_uint32":3,"_uint64":4,"_int8":-5,"_int16":-6,"_int32":-7,"_int64":-8})");
    }
}

TEST(TestGelfMessage, SetFieldFloatingPoint)
{
    {
        GelfMessage message;
        message.SetField("float", std::numeric_limits<float>::min());
        message.SetField("double", std::numeric_limits<double>::min());
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_float":1.1754943508222876e-38,"_double":2.2250738585072014e-308})");
    }
    {
        GelfMessage message;
        message["float"] = std::numeric_limits<float>::min();
        message["double"] = std::numeric_limits<double>::min();
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_float":1.1754943508222876e-38,"_double":2.2250738585072014e-308})");
    }
}

TEST(TestGelfMessage, SetFieldBool)
{
    {
        GelfMessage message;
        message.SetField("true", true);
        message.SetField("false", false);
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_true":1,"_false":0})");
    }
    {
        GelfMessage message;
        message["true"] = true;
        message["false"] = false;
        EXPECT_EQ(Serialize(message), R"({"version":"1.1","_true":1,"_false":0})");
    }
}
