#pragma once

#include <functional>
#include <typeindex>
#include <typeinfo>

namespace dja {
// massively use template partial specializations

template <class F>
struct function_traits;

template <typename R, typename... Args>
struct function_traits<R (*)(Args...)> : public function_traits<R(Args...)> {};

template <typename R, typename... Args>
struct function_traits<R(Args...)> {
  using return_type = R;

  static constexpr std::size_t arity = sizeof...(Args);

  template <std::size_t i>
  struct arg {
    using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
  };
};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...)>
    : public function_traits<R(C&, Args...)> {};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const>
    : public function_traits<R(C&, Args...)> {};

template <typename C, typename R>
struct function_traits<R(C::*)> : public function_traits<R(C&)> {};

template <typename F>
struct function_traits {
 private:
  using call_type = function_traits<decltype(&F::operator())>;

 public:
  using return_type = typename call_type::return_type;

  static constexpr std::size_t arity = call_type::arity - 1;

  template <std::size_t i>
  struct arg {
    using type = typename call_type::template arg<i + 1>::type;
  };
};

// get the largest number

template <int arg, int... args>
struct MaxInteger;

template <int arg>
struct MaxInteger<arg> : std::integral_constant<size_t, arg> {};

template <int arg1, int arg2, int... args>
struct MaxInteger<arg1, arg2, args...>
    : std::integral_constant<int, arg1 >= arg2
                                      ? MaxInteger<arg1, args...>::value
                                      : MaxInteger<arg2, args...>::value> {};

// acquire the maximum of align.

template <typename... Args>
struct MaxAlign
    : std::integral_constant<
          int, MaxInteger<std::alignment_of<Args>::value...>::value> {};

// judge if a certain type (T) exists

template <typename T, typename... List>
struct Contains;

template <typename T, typename Head, typename... Rest>
struct Contains<T, Head, Rest...>
    : std::conditional<std::is_same<T, Head>::value, std::true_type,
                       Contains<T, Rest...>>::type {};

template <typename T>
struct Contains<T> : std::false_type {
  // recursion end point
};

// determine the number of types as template params

template <typename T, typename... List>
struct IndexOf;

template <typename T, typename Head, typename... Rest>
struct IndexOf<T, Head, Rest...> {
  enum { value = IndexOf<T, Rest...>::value + 1 };
};

template <typename T>
struct IndexOf<T> {
  enum { value = -1 };
};

// get type of a certain index in the list

template <int index, typename... Types>
struct At;

template <int index, typename Head, typename... Types>
struct At<index, Head, Types...> {
  using type = typename At<index - 1, Types...>::type;
};

template <typename T, typename... Types>
struct At<0, T, Types...> {
  using type = T;
};
}  // namespace dja
