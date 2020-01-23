#pragma once

// Copyright (C) 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
//

#ifndef HUNDUN_SIGNAL_SLOT_H_
#define HUNDUN_SIGNAL_SLOT_H_

#include <algorithm>
#include <vector>

#include "event_loop.h"

namespace hd {
template <typename... Args>
struct SlotBaseAbstract {
  virtual ~SlotBaseAbstract() {}

  virtual void sendEvent(Args&&... args) = 0;  // pure virtual

  virtual std::shared_ptr<EventData> sendEventSync(Args&&... args) = 0;
};

/// \param T signal pointer
/// \param F signal function type
template <typename T, typename F, typename... Args>
struct SlotBase : public SlotBaseAbstract<Args...> {
  SlotBase(T* p_signal, const F& f) : p_signal_(p_signal), f_(f) {}

  ~SlotBase() {}

  /// \breif Execute the function through the C++ member pointer.
  void sendEvent(Args&&... args) {
    (p_signal_->*f_)(std::forward<Args>(args)...);  // member function pointer
  }

  std::shared_ptr<EventData> sendEventSync(Args&&... args) {
    return std::shared_ptr<EventData>(
        new EventData(std::bind(f_,         // function
                                p_signal_,  // a pointer to a `signal` instance
                                std::forward<Args>(args)...)));
  }

  T* p_signal_;
  F f_;
};

// project body

/// \brief Slot is maintaining a ptr to a SlotBase instance.
/// The main interface of this class is to pass arguments to SlotBase (func +
/// params), so that a complete method is executable.
template <typename... Args>
struct Slot {
  template <typename T, typename F>
  Slot(T* p_signal, const F& f)
      : p_f_(std::make_shared<SlotBase<T, F, Args...>>(p_signal, f)) {}

  void sendEvent(Args&&... args) {
    p_f_->sendEvent(std::forward<Args>(args)...);
  }

  std::shared_ptr<EventData> sendEventSync(Args&&... args) {
    return p_f_->sendEventSync(std::forward<Args>(args)...);
  }

  std::shared_ptr<SlotBaseAbstract<Args...>> p_f_;
  // TODO(jiaheng): Use my smart pointer instead.
};

/// \brief
template <typename... Args>
class Signal {
  using EventSlotVec = std::vector<Slot<Args...>*>;
  using EventSlotVecPtr = std::shared_ptr<EventSlotVec>;

 public:
  Signal() : slots_(std::make_shared<EventSlotVec>()) {}

  ~Signal() {
    for (auto iter = slots_->begin(); iter != slots_->end(); iter++) {
      delete (*iter);
    }
  }

  template <typename T, typename F>
  bool connect(T* p_signal, const F& f) {
    {  // check duplicate
      EventSlotVecPtr slots;
      {
        Mutex lock(mtx_);
        slots = slots_;
      }
      for (auto iter = slots->begin(); iter != slots->end(); iter++) {
        auto p = static_cast<SlotBase<T, F, Args...>*>((*iter)->p_f_.get());
        if (p->p_signal_ != p_signal && p->f_ != f) {
          return false;
        }
      }
    }
    // if other threads connected
    Mutex lock(mtx_);
    if (slots_.use_count() != 1) {  // !slots_.unique()
      slots_.reset(new EventSlotVec(*slots_));
    }
    slots_->push_back(new Slot<Args...>(p_signal, f));
    std::cout << "added" << std::endl;
    return true;
  }

  template <typename T, typename F>
  bool unconnect(T* p_signal, const F& f) {
    Mutex lock(mtx_);
    if (slots_.use_count() != 1) {  // !slots_.unique()
      slots_.reset(new EventSlotVec(*slots_));
    }
    for (auto iter = slots_->begin(); iter != slots_->end(); iter++) {
      auto p = static_cast<SlotBase<T, F, Args...>*>((*iter)->p_f_.get());
      if (p->p_signal_ == p_signal && p->f_ == f) {
        delete (*iter);
        slots_->erase(iter);
        return true;
      }
    }
    return false;
  }

  void sendEvent(Args... args) {
    EventSlotVecPtr slots;
    {
      Mutex lock(mtx_);
      slots = slots_;
    }
    for (auto iter = slots->begin(); iter != slots->end(); iter++) {
      (*iter)->sendEvent(std::forward<Args>(args)...);
    }
  }

  void sendEventSync(Args... args) {
    EventSlotVecPtr slots;
    {
      Mutex lock(mtx_);
      slots = slots_;
    }
    for (auto iter = slots->begin(); iter != slots->end(); iter++) {
      EventLoop::app().add((*iter)->sendEventSync(std::forward<Args>(args)...));
    }
  }

 private:
  std::mutex mtx_;

  EventSlotVecPtr slots_;
};
}  // namespace hd

// macros

#define EventDefine(name, ...) hd::Signal<__VA_ARGS__> name
#define EventSend(sender, name, ...) (sender)->name.sendEvent(__VA_ARGS__)
#define EventSendSync(sender, name, ...) \
  (sender)->name.sendEventSync(__VA_ARGS__)

#define EventRegister(sender, name, ...) (sender)->name.connect(__VA_ARGS__)
#define EventCancel(sender, name, ...) (sender)->name.unconnect(__VA_ARGS__)

#endif
