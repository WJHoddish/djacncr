#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
//

#ifndef HUNDUN_FOR_EACH_H_
#define HUNDUN_FOR_EACH_H_

#include <type_traits>

namespace hd {
template <typename T>
auto move(T&& t) -> typename std::remove_reference<T>::type&& {
  using R = typename std::remove_reference<T>::type&&;
  return static_cast<R>(t);
}

/// \brief A similar version of std::for_each.
///
/// \attention The input function (most likely to be a functor) is input as a
/// reference. Assume it owns a mechanism to keep a log when processing each
/// element.
///
/// \return Return nothing, which indicates the main difference from the
/// std::for_each.
template <typename I, typename F>
void for_each(I begin, I end, F& fn) {
  while (begin != end) {
    // call the function/functor
    fn(*begin);
    // move the pointer/iterator
    ++begin;
  }
  return /*return type is transformed by std::move in C++11*/;
}
}  // namespace hd

#endif