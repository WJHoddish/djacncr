#pragma once

// Copyright (C) 2019 Jiaheng Wang
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Only support `move`.
//

#include <memory>

namespace hd {
class FunctionWrapper {
 public:
  FunctionWrapper() = default;

  template <typename F>
  FunctionWrapper(F &&f) : f_(new FunType<F>(std::move(f))) {}

  FunctionWrapper(FunctionWrapper &&src) noexcept : f_(std::move(src.f_)) {}

  FunctionWrapper &operator=(FunctionWrapper &&src) noexcept {
    f_ = std::move(src.f_);
    return *this;
  }

  void operator()() { f_->call(); }

  FunctionWrapper(const FunctionWrapper &) = delete;
  FunctionWrapper &operator=(const FunctionWrapper &) = delete;

 private:
  struct FunBase {
    virtual ~FunBase() {}

    virtual void call() = 0;
  };

  template <typename F>
  struct FunType : FunBase {
    FunType(F &&f) : f_(std::move(f)) {}

    void call() { f_(); }

    F f_;
  };

  std::unique_ptr<FunBase> f_;
};
}  // namespace hd