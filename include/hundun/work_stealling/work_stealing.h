#pragma once

#include "worker.h"

namespace hd {
/// \brief A special thread pool.
class WorkStealing {
  using F = FunctionWrapper;

 public:
  WorkStealing(size_t num_thread = std::thread::hardware_concurrency())
      : workers_(std::make_shared<std::vector<std::shared_ptr<Worker>>>()) {
    for (size_t i = 0; i < num_thread; i++) {
      workers_->push_back(std::make_shared<Worker>(workers_));
    }
  }

  ~WorkStealing() {
    for (auto iter = workers_->begin(); iter != workers_->end(); iter++) {
      (*iter)->join();
    }
    workers_->clear();
  }

  // submit a function object

  void submit(F task) {  // F only supports `std::move`
                         // randomly select a worker
    workers_->at(rand() % workers_->size())->assign(std::move(task));
  }

 private:
  WorkersPtr workers_;
};
}  // namespace hd