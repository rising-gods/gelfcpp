#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <boost/asio/ip/host_name.hpp>


namespace gelfcpp
{
namespace provider
{

class Host
{
public:
    Host() : Host(boost::asio::ip::host_name()) {}
    Host(const std::string& host) : host_(host) {}

    void SetDefault() { SetHost(boost::asio::ip::host_name()); }
    void SetHost(const std::string& host) { host_ = host; }

    void operator()(GelfMessage& message) const
    {
        message.Set("host", host_);
    }

private:
    std::string host_;
};

}
}
