#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/output/GelfSerializer.hpp>
#include <boost/asio/ip/udp.hpp>
#include <string>
#include <cstdint>

namespace gelfcpp
{

class GelfUDPOutput
{
public:
    GelfUDPOutput(const std::string& host, uint16_t port)
    {
        boost::asio::ip::udp::resolver resolver(service_);
        boost::asio::ip::udp::resolver::query query(host, std::to_string(port));
        endpoint_ = *resolver.resolve(query);
        socket_.reset(new boost::asio::ip::udp::socket(service_, endpoint_.protocol()));
    }

    void Write(const GelfMessage& message)
    {
        for (const std::string& chunk : serializer_.Serialize(message))
            socket_->async_send_to(boost::asio::buffer(chunk), endpoint_, [](const boost::system::error_code&, unsigned long int) {});
    }

private:
    GelfSerializer serializer_;
    boost::asio::io_service service_;
    boost::asio::ip::udp::endpoint endpoint_;
    std::unique_ptr<boost::asio::ip::udp::socket> socket_;
};

}
