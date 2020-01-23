#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
//

#include <map>
#include <string>
#include <vector>

#include "hundun/shell/any.h"
#include "hundun/shell/variant.h"

#include "task.h"

namespace hd {
class TaskGroup {
  using ReturnVariant =
      dja::Variant<int, short, double, unsigned int, std::string>;

 public:
  template <typename R>
  auto run(Task<R()>& task) ->
      typename std::enable_if<!std::is_same<R, void>::value>::type {
    group_.insert(std::make_pair<ReturnVariant, dja::Any>(R(), task.run()));
  }

  template <typename R>
  auto run(Task<R()>&& task) ->
      typename std::enable_if<!std::is_same<R, void>::value>::type {
    group_.insert(std::make_pair<ReturnVariant, dja::Any>(R(), task.run()));
  }

  void run(Task<void()>& task) { group_void_.push_back(task.run()); }

  void run(Task<void()>&& task) { group_void_.push_back(task.run()); }

  template <typename F, typename... Fs>
  void run(F&& head, Fs&&... rest) {
    run(std::forward<F>(head));
    run(std::forward<Fs>(rest)...);
  }

  /// \brief The recursion endpoint.
  template <typename F>
  void run(F&& f) {
    run(Task<std::result_of<F()>::type()>(std::forward<F>(f)));
  }

  /// \brief Wait for all tasks finshed.
  void wait() {
    for (auto it = group_.begin(); it != group_.end(); ++it) {
      auto vrt = it->first;
      vrt.visit([&](int v) { getFuture<int>(it->second); },
                [&](short v) { getFuture<short>(it->second); },
                [&](double v) { getFuture<double>(it->second); },
                [&](unsigned int v) { getFuture<unsigned int>(it->second); },
                [&](std::string v) { getFuture<std::string>(it->second); });
    }
    for (auto it = group_void_.begin(); it != group_void_.end(); ++it) {
      it->get();
    }
  }

 private:
  template <typename T>
  void getFuture(dja::Any& f) {
    f.any_cast<std::shared_future<T>>().get();
  }

  std::map<ReturnVariant, dja::Any> group_;

  std::vector<std::shared_future<void>> group_void_;
};
}  // namespace hd