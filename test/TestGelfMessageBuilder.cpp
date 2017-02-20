#include <gelfcpp/GelfMessageBuilder.hpp>
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

TEST(TestGelfMessageBuilder, ChainTemporary)
{
    GelfMessage msg = GelfMessageBuilder()("a")("bool", true)("string", "std::string"s);
    EXPECT_EQ(Serialize(msg), R"({"version":"1.1","short_message":"a","_bool":1,"_string":"std::string"})");
}

TEST(TestGelfMessageBuilder, Chain)
{
    GelfMessageBuilder builder;
    builder("a");
    builder("bool", true)("string", "std::string"s);
    EXPECT_EQ(Serialize(builder), R"({"version":"1.1","short_message":"a","_bool":1,"_string":"std::string"})");
}

TEST(TestGelfMessageBuilder, Decorator)
{
    {
        auto decorator = [](GelfMessage& msg) { msg["decorated"] = true; };
        GelfMessage msg = GelfMessageBuilder()(decorator);
        EXPECT_EQ(Serialize(msg), R"({"version":"1.1","_decorated":1})");
    }
    {
        auto decorator = [](GelfMessage& msg) { msg["decorated"] = true; };
        EXPECT_EQ(Serialize(GelfMessageBuilder()(decorator)), R"({"version":"1.1","_decorated":1})");
    }
    {
        GelfMessageBuilder builder;
        builder([](GelfMessage& msg) { msg["decorated"] = true; });
        EXPECT_EQ(Serialize(builder), R"({"version":"1.1","_decorated":1})");
    }
}
