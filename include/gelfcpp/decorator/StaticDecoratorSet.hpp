#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <tuple>


namespace gelfcpp
{
namespace decorator
{

template<typename... Decorators>
class StaticDecoratorSet
{
    using DecoratorSet = std::tuple<Decorators...>;
public:
    void operator()(GelfMessage& message)
    {
        operator()(message, std::make_index_sequence<std::tuple_size<DecoratorSet>::value>{});
    }

    template<std::size_t I>
    auto get() -> std::add_lvalue_reference_t<std::tuple_element_t<I, DecoratorSet>>
    {
        return std::get<I>(decorators_);
    }

    template<typename T>
    auto get() -> std::add_lvalue_reference_t<T>
    {
        return std::get<T>(decorators_);
    }

private:
    template<std::size_t... I>
    void operator()(GelfMessage& message, std::index_sequence<I...>)
    {
        [](...) {}((std::get<I>(decorators_)(message), 0)...);
    }

private:
    DecoratorSet decorators_;
};

}
}
