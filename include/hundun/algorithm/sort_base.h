#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Sorting algorithms in C++ (interface only).
//

#ifndef HUNDUN_SORT_BASE_H_
#define HUNDUN_SORT_BASE_H_

#include <iostream>

#include "hundun/structure/list.h"
#include "hundun/structure/vector.h"

namespace hd {
/// \brief The abstract base class for sorting methods.
template <typename Comparable>
class Sort {
 public:
  virtual void operator()(Vector<Comparable>&) = 0;
};

/// \brief A template function using sorting methods (class name) as template to
/// sort a given vector.
template <template <typename T> typename SortMethod, typename Comparable>
auto sort(Vector<Comparable>& arr) -> typename std::enable_if<
    std::is_base_of<Sort<Comparable>, SortMethod<Comparable> >::value>::type {
  // use as a functor
  SortMethod<Comparable> sorter;
  sorter(arr);
}
}  // namespace hd

#endif