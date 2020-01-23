#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// A simple, header-only cpp11 thread pool implementation.
//

#ifndef HUNDUN_TASK_POOL_H_
#define HUNDUN_TASK_POOL_H_

#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include <condition_variable>  // using c++11 multi-thread components
#include <future>
#include <mutex>
#include <thread>

namespace hd {
class TaskPool {
 public:
  TaskPool(int num_threads) : stop_(false) {
    // TODO(jiaheng): Throw exception if `num_threads` <= 0.

    for (int i = 0; i < num_threads; ++i) {
      workers_.emplace_back(  // lambda expressions are load in the thread
                              // vector as tasks
          [this] {
            while (1) {  // for each worker, repeatedly getting a task from the
                         // queue
              std::function<void()> task;
              {
                std::unique_lock<std::mutex> lock(mtx_);
                this->condition_.wait(
                    lock,  // get lock
                    [this] {
                      return (this->stop_ || !this->tasks_.empty());
                    }  // if the expression is false (), stuck
                );
                if (this->stop_ && this->tasks_.empty()) {
                  return;
                }
                task = std::move(this->tasks_.front());
                this->tasks_.pop();
              }  // unique_lock will release the mutex after leaving this space
              task();  // execute the task
            }
          });
    }
  }

  ~TaskPool() {
    {
      std::unique_lock<std::mutex> lock(mtx_);
      stop_ = true;
    }
    condition_.notify_all();
    for (std::thread& worker : workers_) {
      worker.join();
    }
  }

  /// \param f A function/functor with a complete type F.
  /// Its return value type will be derived through `std::result_of<>`.
  /// \param args All parameters that the function `f` needs.
  /// \return The return type is qualified by std::future...
  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(mtx_);
      if (stop_) {  // don't allow enqueueing after stopping the pool
        throw std::runtime_error("enqueue on stopped pool");
      }
      tasks_.emplace([task]() { (*task)(); });
    }  // unique_lock will release the mutex after leaving this space
    condition_.notify_one();
    return res;
  }

 private:
  bool stop_;

  ///< \brief queue's mutex
  std::mutex mtx_;
  std::condition_variable condition_;

  ///< \brief the task queue
  std::queue<std::function<void()>> tasks_;

  ///< \brief keep track of threads so we can join them
  std::vector<std::thread> workers_;
};
}  // namespace hd

#endif
