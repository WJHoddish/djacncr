#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// A doubly linked list implementation.
//

#ifndef DJA_LIST_H_
#define DJA_LIST_H_

#include "list_body.h"

namespace dja {
/// \brief The nested const iterator.
template <typename T>
class list<T>::const_iterator {
  friend class list<T>;

 public:
  /// \brief The constructor.
  const_iterator();

  const T &operator*() const {
    if (!current_) {
      throw Exception("list iterator is null");
    }
    return current_->data_;
  }

  const_iterator &operator++() {
    if (current_->next_ != nullptr) {
      current_ = current_->next_;
    }
    return *this;
  }

  const_iterator &operator--() {
    if (current_->prev_ != nullptr) {
      current_ = current_->prev_;
    }
    return *this;
  }

  const_iterator &operator++(int) {
    static const_iterator old;
    old = *this;
    if (current_->next_ != nullptr) {
      current_ = current_->next_;
    }
    return old;
  }

  const_iterator &operator--(int) {
    static const_iterator old;
    old = *this;
    if (current_->prev_ != nullptr) {
      current_ = current_->prev_;
    }
    return old;
  }

  bool operator==(const const_iterator &rhs) const {
    return current_ == rhs.current_;
  }

  bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }

 protected:
  /// \brief Protected constructor for use only by derived iterator, friends.
  const_iterator(list::Node *p, const list &lst);

  list::Node *current_;
  const list *list_;
};

template <typename T>
list<T>::const_iterator::const_iterator() : current_(nullptr), list_(nullptr) {}

template <typename T>
list<T>::const_iterator::const_iterator(list::Node *p, const list &lst)
    : current_(p), list_(&lst) {}
}  // namespace dja

namespace dja {
/// \brief The nested iterator.
template <typename T>
class list<T>::iterator : public list<T>::const_iterator {
  friend class list<T>;

 public:
  iterator();

  T &operator*() {
    typename list<T>::Node *&current_ = list<T>::const_iterator::current_;
    if (!current_) {
      throw Exception("list iterator is null");
    }
    return current_->data_;
  }

  iterator &operator++() {
    typename list<T>::Node *&current_ = list<T>::const_iterator::current_;
    if (current_->next_ != nullptr) {
      current_ = current_->next_;
    }
    return *this;
  }

  iterator &operator--() {
    typename list<T>::Node *&current_ = list<T>::const_iterator::current_;
    if (current_->prev_ != nullptr) {
      current_ = current_->prev_;
    }
    return *this;
  }

  iterator &operator++(int) {
    typename list<T>::Node *&current_ = list<T>::const_iterator::current_;
    static iterator old;
    old = *this;
    if (current_->next_ != nullptr) {
      current_ = current_->next_;
    }
    return old;
  }

  iterator &operator--(int) {
    typename list<T>::Node *&current_ = list<T>::const_iterator::current_;
    static iterator old;
    old = *this;
    if (current_->prev_ != nullptr) {
      current_ = current_->prev_;
    }
    return old;
  }

 protected:
  /// \brief A chain to the base class, call the proteted constructor of base.
  iterator(list::Node *p, const list &lst);
};

template <typename T>
list<T>::iterator::iterator() {}

template <typename T>
list<T>::iterator::iterator(list::Node *p, const list &lst)
    : const_iterator(p, lst) {}
}  // namespace dja

#endif