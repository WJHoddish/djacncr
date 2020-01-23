#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
//

#include <functional>
#include <future>

namespace hd {
template <typename T>
class Task;

/// \brief A partial specialization.
///
/// \attention R is the return type.
template <typename R, typename... Args>
class Task<R(Args...)> {
 public:
  using return_type = R;

  /// \brief Use a std::function or a lambda exp to construct.
  Task(std::function<R(Args...)>& f) : fn_(f) {}

  Task(std::function<R(Args...)>&& f) : fn_(std::move(f)) {}

  ~Task() {}

  // asynchronous call

  void wait() { std::async(fn_).wait(); }

  /// \brief Activate a thread and instantly return a std::future object.
  template <typename... Args>
  std::shared_future<R> run(Args&&... args) {
    return std::async(fn_, std::forward<Args>(args)...);
  }

  template <typename... Args>
  R get(Args&&... args) {
    return std::async(fn_, std::forward<Args>(args)...).get();
  }

 private:
  ///< \brief registered function
  std::function<R(Args...)> fn_;
};
}  // namespace hd