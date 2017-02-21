#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <boost/asio/ip/host_name.hpp>


namespace gelfcpp
{
namespace decorator
{

/**
 * \brief Decorator which appends the "hostname"
 *
 * Defaults to the current hostname of the system.
 */
class Host
{
public:
    /**
     * \brief Uses the systems current hostname
     */
    Host() : Host(boost::asio::ip::host_name()) {}
    /**
     * \brief Explicitly sets the hostname
     * \param host the hostname
     */
    Host(const std::string& host) : host_(host) {}

    /**
     * \brief Resets the hostname to the system current hostname
     */
    void SetDefault() { SetHost(boost::asio::ip::host_name()); }
    /**
     * \brief Explicitly sets the hostname
     * \param host the hostname
     */
    void SetHost(const std::string& host) { host_ = host; }

    /**
     * \brief Decorator apply operator
     * \param message the message
     */
    void operator()(GelfMessage& message) const
    {
        message.SetField("host", host_);
    }

private:
    std::string host_;
};

}
}
