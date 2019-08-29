#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// A doubly linked list implementation.
//

#ifndef DJA_LIST_H_
#define DJA_LIST_H_

#include "core/error.h"

namespace dja {
template <typename T>
class list {
 public:
  list() { init(); }

  ~list() { destroy(); }

  const list operator=(const list &rhs) {
    const_iterator iter;
    if (&rhs != this) {
      clear();
      for (iter = rhs.begin(); iter != rhs.end(); ++iter) {
        push_back(*iter);
      }
    }
    return *this;
  }

  bool empty() const { return (size_ == 0); }

  int size() const { return size_; }

  T &front() { return head_->next_->data_; }

  const T &front() const { return head_->next_->data_; }

  T &back() { return tail_->prev_->data_; }

  const T &back() const { return tail_->prev_->data_; }

  void push_front(const T &src) {
    Node *temp = new Node(src, head_, head_->next_);
    (head_->next_)->prev_ = temp;
    head_->next_ = temp;
    size_++;
  }

  void push_back(const T &src) {
    Node *temp = new Node(src, tail_->prev_, tail_);
    (tail_->prev_)->next_ = temp;
    tail_->prev_ = temp;
    size_++;
  }

  void pop_front() {
    Node *temp;
    if (0 == size_) {
      return;
    }
    temp = head_->next_;
    head_->next_ = temp->next_;
    head_->next_->prev_ = head_;
    {
      delete temp;
      temp = nullptr;
    }
    size_--;
  }

  void pop_back() {
    Node *temp;
    if (0 == size_) {
      return;
    }
    temp = tail_->prev_;
    tail_->prev_ = temp->prev_;
    tail_->prev_->next_ = tail_;
    {
      delete temp;
      temp = nullptr;
    }
    size_--;
  }

  /// \brief To clear the data structure up.
  void clear() {
    while (size_ > 0) {
      pop_front();
    }
  }

  /// \brief The nested iterator.
  class const_iterator {
    friend class list<T>;

   public:
   protected:
  };

  /// \brief The nested iterator.
  class iterator : public list<T>::const_iterator {
    friend class list;

   public:
   protected:
  };

 private:
  /// \brief A nested template component is defined outside.
  class Node;

  void init() {
    size_ = 0;
    head_ = new Node;
    tail_ = new Node;
    head_->next_ = tail_;
    tail_->prev_ = head_;
  }

  void destroy() {
    clear();
    delete head_;
    delete tail_;
  }

  ///< \brief the list size
  int size_;
  ///< \brief node sticked at head and tail in the list
  Node *head_, *tail_;
};

template <typename T>
class list<T>::Node {
  friend class list<T>;

 public:
  /// \brief The constructor.
  ///
  /// \attention Using dummy node constructed by the default constructor given
  /// by the type T.
  Node(const T &data = T(), Node *prev = nullptr, Node *next = nullptr)
      : data_(data), prev_(prev), next_(next) {}

 private:
  T data_;
  Node *prev_, *next_;
};
}  // namespace dja

#endif