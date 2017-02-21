#include <gelfcpp/GelfMessageStream.hpp>
#include <gelfcpp/output/GelfJSONOutput.hpp>
#include <gtest/gtest.h>


using namespace gelfcpp;
using namespace std::string_literals;

namespace
{
struct TestGelfMessageStream : public ::testing::Test
{
    TestGelfMessageStream() :
            Test(),
            output(result)
    {}

    std::ostringstream result;
    output::GelfJSONOutput output;
};

struct NullDeleter { template<typename T> void operator()(T*) {} };

}

TEST_F(TestGelfMessageStream, SendReference)
{
    {
        GelfMessageStream stream(output);
        stream.Send(output) = GelfMessageBuilder()("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"})");
    }
    {
        GELF_MESSAGE(output)("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"}{"version":"1.1","short_message":"message"})");
    }
}

TEST_F(TestGelfMessageStream, SendPointer)
{
    auto ptr = &output;
    {
        GelfMessageStream stream(ptr);
        stream.Send(ptr) = GelfMessageBuilder()("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"})");
    }
    {
        GELF_MESSAGE(ptr)("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"}{"version":"1.1","short_message":"message"})");
    }
}

TEST_F(TestGelfMessageStream, SendSharedPointer)
{
    auto ptr = std::shared_ptr<output::GelfJSONOutput>(&output, NullDeleter());
    {
        GelfMessageStream stream(ptr);
        stream.Send(ptr) = GelfMessageBuilder()("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"})");
    }
    {
        GELF_MESSAGE(ptr)("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"}{"version":"1.1","short_message":"message"})");
    }
}

TEST_F(TestGelfMessageStream, SendUniquePointer)
{
    auto ptr = std::unique_ptr<output::GelfJSONOutput, NullDeleter>(&output);
    {
        GelfMessageStream stream(ptr);
        stream.Send(ptr) = GelfMessageBuilder()("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"})");
    }
    {
        GELF_MESSAGE(ptr)("message");
        EXPECT_EQ(result.str(), R"({"version":"1.1","short_message":"message"}{"version":"1.1","short_message":"message"})");
    }
}
