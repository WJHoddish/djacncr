#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Pay attention to the use of std::bind.
//

#include <functional>
#include <list>
#include <memory>
#include <thread>

#include "mutex.h"

namespace hd {
class FuncHolderAbstract {
 public:
  virtual ~FuncHolderAbstract() {}

  virtual void run() = 0;
};

template <typename F>
class FuncHolder : public FuncHolderAbstract {
 public:
  FuncHolder(F&& f) : f_(std::forward<F>(f)) {}

  void run() {
    f_();  // run function/functor with no parameter
  }

 private:
  F f_;
};

// project body

class EventData {
 public:
  /// \brief Bind function & parameters together when constructing.
  template <typename F, typename... Args>
  EventData(F&& f, Args&&... args)
      : p_f_(makeFunction(std::bind(std::forward<F>(f),  // function body
                                    std::forward<Args>(args)...  // arguments
                                    ))) {}

  void run() {
    p_f_->run();  // run function/functor with bound parameters
  }

 private:
  /// \brief Receive a bound function/functor, return its shared pointer.
  template <typename F>
  std::shared_ptr<FuncHolder<F>> makeFunction(F&& f) {
    return std::make_shared<FuncHolder<F>>(std::forward<F>(f));
  }

  std::shared_ptr<FuncHolderAbstract> p_f_;
};

class EventLoop {
  using EventDataList = std::list<std::shared_ptr<EventData>>;
  using EventDataListPtr = std::shared_ptr<typename EventDataList>;

 public:
  static EventLoop& app() {
    static EventLoop obj;
    return obj;
  }

  void init() { thread_id_ = std::this_thread::get_id(); }

  bool loop() {
    if (std::this_thread::get_id() != thread_id_) {
      return false;  // error not run in main thread
    }

    EventDataListPtr tasks;
    {
      // create a temp run-tasks-list container
      Mutex lock(mtx_);  // lock on
      tasks.reset(new EventDataList(*tasks_));
      tasks_->clear();
    }

    for (auto iter = tasks->begin(); iter != tasks->end(); iter++) {
      (*iter)->run();  // each iter is a pointer to an `EventData` instance
    }
    return true;
  }

  void add(std::shared_ptr<EventData> task) {
    Mutex lock(mtx_);               // lock on
    if (tasks_.use_count() != 1) {  // !tasks_.unique()
      // multiple reference to the data, separate a new list
      tasks_.reset(new EventDataList(*tasks_));
    }
    tasks_->push_back(task);  // update event list by this new task
  }

 private:
  EventLoop()
      : thread_id_(std::this_thread::get_id()),
        tasks_(std::make_shared<EventDataList>()) {}

  std::mutex mtx_;  // warning: std::mutex is not able to copy
  std::thread::id thread_id_;

  EventDataListPtr tasks_;
};
}  // namespace hd