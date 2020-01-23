#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Sorting algorithms in C++.
//

#ifndef HUNDUN_SORT_H_
#define HUNDUN_SORT_H_

#include "sort_base.h"

namespace hd {
template <typename Comparable>
class BubbleSort : public Sort<Comparable> {
 public:
  void operator()(Vector<Comparable>& arr) {
    int size = arr.size();
    for (int i = 0; i < size - 1; ++i) {
      for (int j = 0; j < size - i - 1; ++j) {
        if (arr[j] > arr[j + 1]) {
          Comparable temp(arr[j]);
          arr[j] = arr[j + 1];
          arr[j + 1] = temp;
        }
      }
    }
  }
};

template <typename Comparable>
class InsertionSort : public Sort<Comparable> {
 public:
  void operator()(Vector<Comparable>& arr) {
    int size = arr.size();
    for (int i = 1; i < size; ++i) {
      Comparable temp(arr[i]);
      int j = i;
      for (; j > 0 && temp < arr[j - 1]; --j) {
        arr[j] = arr[j - 1];
      }
      arr[j] = temp;
    }
  }
};

template <typename Comparable>
class PancakeSort : public Sort<Comparable> {
 public:
  void operator()(Vector<Comparable>& arr) {
    //
  }
};

template <typename Comparable>
class ShellSort : public Sort<Comparable> {
 public:
  void operator()(Vector<Comparable>& arr) {
    int size = arr.size();
    for (int gap = size / 2; gap > 0; gap /= 2) {
      for (int i = gap; i < size; ++i) {
        Comparable temp(arr[i]);
        int j = i;
        for (; j >= gap && temp < arr[j - gap]; j -= gap) {
          arr[j] = arr[j - gap];
        }
        arr[j] = temp;
      }
    }
  }
};
}  // namespace hd

#endif
