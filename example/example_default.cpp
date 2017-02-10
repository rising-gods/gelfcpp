#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/output/GelfUDPOutput.hpp>
#include <gelfcpp/output/GelfJSONOutput.hpp>
#include <gelfcpp/GelfMessageStream.hpp>
#include <iostream>

#include <gelfcpp/provider/StaticCombiner.hpp>
#include <gelfcpp/provider/Timestamp.hpp>
#include <gelfcpp/provider/Host.hpp>


int main()
{
    using Provider = gelfcpp::provider::StaticCombiner<gelfcpp::provider::CurrentTimestamp, gelfcpp::provider::Host>;
    Provider provider;
    provider.get<gelfcpp::provider::Host>().SetHost("lool");

    gelfcpp::GelfUDPOutput out("192.168.0.119", 13000);
    gelfcpp::GelfJSONOutput json(std::cout);

    auto test = std::make_shared<gelfcpp::GelfJSONOutput>(std::cout);
    auto null = std::shared_ptr<gelfcpp::GelfJSONOutput>();
    auto null_ptr = null.get();

    GELF_MESSAGE(json)(provider)("Help")("me", 1);
    GELF_MESSAGE(test)(provider)("Help")("me", 1);
    GELF_MESSAGE(test.get())(provider)("Help")("me", 1);
    GELF_MESSAGE(null)(provider)("Help")("me", 1);
    GELF_MESSAGE(null_ptr)(provider)("Help")("me", 1);
}
