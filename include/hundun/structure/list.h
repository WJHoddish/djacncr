#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// A doubly linked List implementation.
//

#ifndef HUNDUN_LIST_H_
#define HUNDUN_LIST_H_

#include "List_base.h"

namespace hd {
/// \brief The nested const iterator.
template <typename T>
class List<T>::const_iterator {
  friend class List<T>;

 public:
  /// \brief The constructor.
  const_iterator();

  const T &operator*() const {
    if (!current_) {
      throw Exception("List iterator is null");
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
  const_iterator(List::Node *p, const List &lst);

  List::Node *current_;
  const List *list_;
};

template <typename T>
List<T>::const_iterator::const_iterator() : current_(nullptr), list_(nullptr) {}

template <typename T>
List<T>::const_iterator::const_iterator(List::Node *p, const List &lst)
    : current_(p), List_(&lst) {}
}  // namespace hd

namespace hd {
/// \brief The nested iterator.
template <typename T>
class List<T>::iterator : public List<T>::const_iterator {
  friend class List<T>;

 public:
  iterator();

  T &operator*() {
    typename List<T>::Node *&current_ = List<T>::const_iterator::current_;
    if (!current_) {
      throw Exception("List iterator is null");
    }
    return current_->data_;
  }

  iterator &operator++() {
    typename List<T>::Node *&current_ = List<T>::const_iterator::current_;
    if (current_->next_ != nullptr) {
      current_ = current_->next_;
    }
    return *this;
  }

  iterator &operator--() {
    typename List<T>::Node *&current_ = List<T>::const_iterator::current_;
    if (current_->prev_ != nullptr) {
      current_ = current_->prev_;
    }
    return *this;
  }

  iterator &operator++(int) {
    typename List<T>::Node *&current_ = List<T>::const_iterator::current_;
    static iterator old;
    old = *this;
    if (current_->next_ != nullptr) {
      current_ = current_->next_;
    }
    return old;
  }

  iterator &operator--(int) {
    typename List<T>::Node *&current_ = List<T>::const_iterator::current_;
    static iterator old;
    old = *this;
    if (current_->prev_ != nullptr) {
      current_ = current_->prev_;
    }
    return old;
  }

 protected:
  /// \brief A chain to the base class, call the proteted constructor of base.
  iterator(List::Node *p, const List &lst);
};

template <typename T>
List<T>::iterator::iterator() {}

template <typename T>
List<T>::iterator::iterator(List::Node *p, const List &lst)
    : const_iterator(p, lst) {}
}  // namespace hd

#endif