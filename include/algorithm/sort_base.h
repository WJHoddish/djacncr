#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Sorting algorithms in C++ (interface only).
//

#ifndef DJA_SORT_BASE_H_
#define DJA_SORT_BASE_H_

#include <iostream>

#include "structure/list.h"
#include "structure/vector.h"

namespace dja {
/// \brief The abstract base class for sorting methods.
template <typename Comparable>
class Sort {
 public:
  virtual void operator()(vector<Comparable>&) = 0;
};

/// \brief A template function using sorting methods (class name) as template to
/// sort a given vector.
template <template <typename T> typename SortMethod, typename Comparable>
auto sort(vector<Comparable>& arr) -> typename std::enable_if<
    std::is_base_of<Sort<Comparable>, SortMethod<Comparable> >::value>::type {
  // use as a functor
  SortMethod<Comparable> sorter;
  sorter(arr);
}
}  // namespace dja

#endif
