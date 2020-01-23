#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Include this file to introduce APIs.
//

#ifndef HUNDUN_TASK_LAUNCH_H_
#define HUNDUN_TASK_LAUNCH_H_

#include "task_group.h"

namespace hd {
template <typename... Fs>
void parallel_invoke(Fs&&... rest) {
  TaskGroup group;
  group.run(std::forward<Fs>(rest)...);
  group.wait();
}

template <class I, class F>
void parallel_for_each(I begin, I end, F& fn) {
  auto part_num = std::thread::hardware_concurrency();
  auto block_size = std::distance(begin, end) / part_num;

  I last = begin;
  if (block_size > 0) {
    std::advance(last, (part_num - 1) * block_size);
  } else {
    last = end;
    block_size = 1;
  }

  std::vector<std::future<void>> futures;
  for (; begin != last; std::advance(begin, block_size)) {
    futures.emplace_back(std::async([begin, block_size, &fn] {
      std::for_each(begin, begin + block_size, fn);
    }));
  }
  futures.emplace_back(
      std::async([&begin, &end, &fn] { std::for_each(begin, end, fn); }));

  std::for_each(futures.begin(), futures.end(), [](std::future<void>& f) {
    f.get();
  });  // wait for all finished
}

template <typename Range, typename RangeFunc, typename ReduceFunc>
inline typename Range::value_type ParallelReduce(
    Range& range, typename Range::value_type& init, RangeFunc& rangeFunc,
    ReduceFunc& reduceFunc) {}
}  // namespace hd

namespace hd {
/// \breif Create a task which represents all tasks registered are finished.
template <typename F, typename... Args,
          typename R = typename Task<F>::return_type>
auto whenAll(std::vector<Task<F>>& range, Args&&... args)
    -> Task<std::vector<R>()> {
  Task<std::vector<R>()> task([&range, &args...] {
    std::vector<std::shared_future<R>> fv;
    for (auto& task : range) {
      fv.emplace_back(task.run(std::forward<Args>(args)...));
    }
    std::vector<R> vv;
    for (auto& item : fv) {
      vv.emplace_back(item.get());  // items in "fv" are std::future objects
    }
    return vv;
  });
  return task;
}
}  // namespace hd

#endif
