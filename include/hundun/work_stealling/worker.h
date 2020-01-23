#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "lock_queue.h"

namespace hd {
class Worker;
using WorkersPtr =
    std::shared_ptr<std::vector<std::shared_ptr<Worker>>>;  // a Woker vector

class Worker {
  using F = FunctionWrapper;

 public:
  Worker(WorkersPtr workers)
      : enable_(true),
        workers_(workers)  // smart pointer copy
  {
    thread_ = std::thread(&Worker::execute,
                          this);  // construct, and instantly activate a thread
  }

  ~Worker() {}

  //

  bool empty() const { return queue_.empty(); }

  void assign(F task) { queue_.push(std::move(task)); }

  bool steal(F& task) { return queue_.steal(task); }

  void join() {
    enable_ = false;
    thread_.join();
  }

 private:
  void execute() {
    thread_id_ = std::this_thread::get_id();
    while (enable_) {  // execute task by task in the queue
      F task;

      // travers tasks
      while (queue_.pop(task)) {
        task();
      }

      // check if has task
      if (std::all_of(workers_->begin(),  // iter begin
                      workers_->end(),    // iter end
                      [](std::shared_ptr<Worker> worker) {
                        return worker->empty();
                      }  // condition
                      )  // the sign of no task in all workers
      ) {
        continue;
      }

      // steal
      if (workers_->at(rand() % workers_->size())->steal(task)) {
        task();
      }
    }
  }

  bool enable_;

  std::thread thread_;
  std::thread::id thread_id_;

  WorkersPtr workers_;

  LockQueue queue_;  // store tasks
};
}  // namespace hd