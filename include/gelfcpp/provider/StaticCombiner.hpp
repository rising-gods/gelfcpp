#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <tuple>


namespace gelfcpp
{
namespace provider
{

template<typename... Providers>
class StaticCombiner
{
    using Storage = std::tuple<Providers...>;
public:
    void operator()(GelfMessage& message)
    {
        operator()(message, std::make_index_sequence<std::tuple_size<Storage>::value>{});
    }

    template<std::size_t I>
    auto get() -> std::add_lvalue_reference_t<std::tuple_element_t<I, Storage>>
    {
        return std::get<I>(providers_);
    }

    template<typename T>
    auto get() -> std::add_lvalue_reference_t<T>
    {
        return std::get<T>(providers_);
    }

private:
    template<std::size_t... I>
    void operator()(GelfMessage& message, std::index_sequence<I...>)
    {
        [](...) {}((std::get<I>(providers_)(message), 0)...);
    }

private:
    Storage providers_;
};
}
}
