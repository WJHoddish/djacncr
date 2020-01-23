#pragma once

// Copyright (C) 2019 Jiaheng Wang
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Available for the work stealing, based on lock.
//

#include <deque>
#include <mutex>
#include <thread>

#include "function_wrapper.h"

namespace hd {
/// \brief The so-called work-stealing queue, a queue stores functions (tasks).
class LockQueue {
  using F = FunctionWrapper;

 public:
  LockQueue() {}

  LockQueue(const LockQueue&) = delete;
  LockQueue& operator=(const LockQueue&) = delete;

  bool empty() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return tasks_.empty();
  }

  void push(F ele) {
    std::lock_guard<std::mutex> lock(mtx_);
    tasks_.push_front(std::move(ele));
  }

  // fetch task though a passed-in reference

  bool pop(F& res) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (tasks_.empty()) {  // empty task queue
      return false;
    }
    res = std::move(tasks_.front());  // transfer functor
    tasks_.pop_front();
    return true;
  }

  /// \brief Provide the element at `back()`.
  bool steal(F& res) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (tasks_.empty()) {
      return false;
    }
    res = std::move(tasks_.back());
    tasks_.pop_back();
    return true;
  }

 private:
  mutable std::mutex mtx_;

  std::deque<F> tasks_;
};
}  // namespace hd
