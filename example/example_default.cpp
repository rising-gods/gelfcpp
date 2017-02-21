#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/output/GelfUDPOutput.hpp>
#include <gelfcpp/output/GelfJSONOutput.hpp>
#include <gelfcpp/GelfMessageStream.hpp>

#include <gelfcpp/decorator/StaticDecoratorSet.hpp>
#include <gelfcpp/decorator/Timestamp.hpp>
#include <gelfcpp/decorator/Host.hpp>

#include <iostream>


using namespace std::string_literals;

int main()
{
    // define a nice decorator
    // - add hostname
    // - add current timestamp
    using Decorator = gelfcpp::decorator::StaticDecoratorSet<gelfcpp::decorator::CurrentTimestamp, gelfcpp::decorator::Host>;
    Decorator decorator;

    // explicitly set hostname, defaults to the hostname of the system
    decorator.get<gelfcpp::decorator::Host>().SetHost("explicit-hostname");

    // define an udp output: hostname + port
    gelfcpp::output::GelfUDPOutput graylog("192.168.0.119", 13000);

    // define an JSON output, printing directly to std::cout
    gelfcpp::output::GelfJSONOutput json(std::cout);


    // we can also wrap outputs in managed pointers
    auto test = std::make_shared<gelfcpp::output::GelfJSONOutput>(std::cout);

    // these outputs are explictly invalid -> if using the stream API no message is generated
    auto null = std::shared_ptr<gelfcpp::output::GelfJSONOutput>();
    auto null_ptr = null.get();

    // sending some messages
    GELF_MESSAGE(json)(decorator)
                              ("Test message to std::cout")
                              ("tagged_value", 1);
    std::cout << std::endl;

    GELF_MESSAGE(graylog)(decorator)
                                 ("Test message to out graylog instance via UDP")
                                 ("from_gelfcpp", true);

    // this should not be visible, since the output is invalid
    GELF_MESSAGE(null)(decorator)("I'm not printed");


    // the streaming API is on-demand, if output is invalid, the stream expression is not executed
    // this should be visible
    GELF_MESSAGE(json)([](gelfcpp::GelfMessage& msg)
                       {
                           msg.SetField("i_am_not_executed", false);
                           std::cout << "This decorator is executed!" << std::endl;
                       });
    std::cout << std::endl;

    // ... but this not
    GELF_MESSAGE(null_ptr)(decorator)([](gelfcpp::GelfMessage&)
                                      {
                                          std::cout << "This decorator is not executed,"
                                                    << " since streaming is on-demand";
                                      });
}
