#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <tuple>


namespace gelfcpp
{
namespace decorator
{

/**
 * \brief Statically combines multiple decorators
 *
 * \note The decorators are applied in definition order.
 *
 * \tparam Decorators decorators that should be applied
 */
template<typename... Decorators>
class StaticDecoratorSet
{
    using DecoratorSet = std::tuple<Decorators...>;
public:
    /**
     * \brief Decorator apply operator
     * \param message the message
     */
    void operator()(GelfMessage& message)
    {
        operator()(message, std::make_index_sequence<std::tuple_size<DecoratorSet>::value>{});
    }

#ifndef GELFCPP_DOXYGEN_RUNNING

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

#else
    /**
     * \brief Accesses the decorator in the set by index
     * \tparam I decorator index
     * \return the decorator
     */
    template<std::size_t I> Decorator& get();

    /**
     * \brief Accesses the decorator in the set by name
     * \tparam T decorator type
     * \return the decorator
     */
    template<typename T> Decorator& get();
#endif

private:
    template<std::size_t... I>
    void operator()(GelfMessage& message, std::index_sequence<I...>)
    {
        int dummy[] = { (std::get<I>(decorators_)(message), 0)... };
        (void) dummy;
    }

private:
    DecoratorSet decorators_;
};

}
}
