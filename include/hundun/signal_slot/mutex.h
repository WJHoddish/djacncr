#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
//

#include <mutex>

namespace hd {
/// \brief An encapsulation of std::mutex.
class Mutex {
 public:
  explicit Mutex(std::mutex& mtx) : mtx_(mtx) {
    mtx_.lock();  // lock mutex when constructing
  }

  ~Mutex() { mtx_.unlock(); }

 private:
  std::mutex& mtx_;
};
}  // namespace hd