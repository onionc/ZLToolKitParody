#pragma once

#include <functional>
#include <cstddef>

namespace toolkit{

template<typename T>
struct function_traits;

// 普通函数
template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{

public:
    enum {arity = sizeof...(Args)};
    typedef Ret function_type(Args...);
    typedef Ret return_type;
    using stl_function_type = std::function<function_type>;
    typedef Ret(*pointer)(Args...);

    template<size_t I>
    struct args{
        static_assert( I < arity, "index is out of range");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

// 函数指针
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)>{};

// std::fucntion
template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)>{};

// 成员函数
#define FUNCTION_TRAITS(...) \
    template <typename ReturnType, typename ClassType, typename... Args> \
    struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{}; \


FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

// 函数对象
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())>{};

}