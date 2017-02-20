#include <gelfcpp/decorator/Host.hpp>
#include <gelfcpp/decorator/Timestamp.hpp>
#include <gelfcpp/decorator/StaticDecoratorSet.hpp>
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

bool HasTimestamp(const GelfMessage& message)
{
    const auto& doc = detail::DocumentAccessor::get(message);
    auto itr = doc.FindMember("timestamp");
    if (itr == doc.MemberEnd())
        return false;

    return itr->value.IsDouble();
}

struct DummyDecorator
{
    void operator()(GelfMessage& msg)
    {
        msg.SetTimestamp(42);
    }
};

}

TEST(TestDecorators, Host)
{
    decorator::Host host("test-pc");
    {
        GelfMessage msg;
        host(msg);
        EXPECT_EQ(Serialize(msg), R"({"version":"1.1","host":"test-pc"})");
    }

    host.SetHost("pc-test");
    {
        GelfMessage msg;
        host(msg);
        EXPECT_EQ(Serialize(msg), R"({"version":"1.1","host":"pc-test"})");
    }
    {
        GelfMessage msg = GelfMessageBuilder()(host);
        EXPECT_EQ(Serialize(msg), R"({"version":"1.1","host":"pc-test"})");
    }
}

TEST(TestDecorators, CurrentTimestamp)
{
    decorator::CurrentTimestamp timestamp;
    {
        GelfMessage msg;
        timestamp(msg);
        EXPECT_TRUE(HasTimestamp(msg));
    }
    {
        GelfMessage msg = GelfMessageBuilder()(timestamp);
        EXPECT_TRUE(HasTimestamp(msg));
    }
}

TEST(TestDecorators, StaticDectoratorSet)
{
    {
        decorator::StaticDecoratorSet<> set;
        {
            GelfMessage msg;
            set(msg);
            EXPECT_EQ(Serialize(msg), R"({"version":"1.1"})");
        }
    }
    {
        decorator::StaticDecoratorSet<decorator::Host> set;
        set.get<0>().SetHost("test-pc");
        {
            GelfMessage msg;
            set(msg);
            EXPECT_EQ(Serialize(msg), R"({"version":"1.1","host":"test-pc"})");
        }
        set.get<decorator::Host>().SetHost("pc-test");
        {
            GelfMessage msg;
            set(msg);
            EXPECT_EQ(Serialize(msg), R"({"version":"1.1","host":"pc-test"})");
        }
    }
    {
        decorator::StaticDecoratorSet<DummyDecorator, decorator::Host> set;
        set.get<1>().SetHost("test-pc");
        {
            GelfMessage msg;
            set(msg);
            EXPECT_EQ(Serialize(msg), R"({"version":"1.1","timestamp":42.0,"host":"test-pc"})");
        }
        set.get<decorator::Host>().SetHost("pc-test");
        {
            GelfMessage msg;
            set(msg);
            EXPECT_EQ(Serialize(msg), R"({"version":"1.1","timestamp":42.0,"host":"pc-test"})");
        }
    }
}
