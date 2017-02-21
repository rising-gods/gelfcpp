#pragma once

#include <gelfcpp/GelfMessage.hpp>


namespace gelfcpp
{
namespace detail
{

struct DocumentAccessor
{
    static const rapidjson::Document& get(const GelfMessage& message)
    {
        return message.doc_;
    }
};

}
}
