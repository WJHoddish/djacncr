#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// A doubly linked list implementation.
//

#ifndef DJA_LIST_BODY_H_
#define DJA_LIST_BODY_H_

#include "core/error.h"

namespace dja {
template <typename T>
class list {
  class Node;

 public:
  class const_iterator;

  class iterator;

  list() { init(); }

  list(const list &src) {
    init();
    *this = src;
  }

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

  /// \brief To clear the data structure up.
  void clear() {
    while (size_ > 0) {
      pop_front();
    }
  }

  /// \brief Add a new element at the head of list.
  void push_front(const T &src) {
    Node *temp = new Node(src, head_, head_->next_);
    (head_->next_)->prev_ = temp;
    head_->next_ = temp;
    size_++;
  }

  /// \brief Attach a new element to the tail.
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

  const_iterator begin() const { return const_iterator(head_->next_, *this); }

  iterator begin() { return iterator(head_->next_, *this); }

  const_iterator end() const { return const_iterator(tail_, *this); }

  iterator end() { return iterator(tail_, *this); }

  /// \brief Insert new element into the list when traversing.
  iterator insert(iterator iter, const T &src) {
    if (iter.list_ != this) {
      throw Exception("list iterator mismatch");
    }

    Node *p = iter.current_;
    if (p == nullptr) {
      throw Exception("list iterator is null");
    }
    if (p->prev_ == nullptr) {
      throw Exception("list iterator is null");
    }

    // build a node and link it up
    Node *new_node = new Node(src, p->prev_, p);
    p->prev_->next_ = new_node;
    p->prev_ = new_node;
    size_++;

    // form a new iterator
    return iterator(new_node, *this);
  }

  /// \brief Erase the data represented by this iterator.
  iterator erase(iterator iter) {
    if (iter.list_ != this) {
      throw Exception("list iterator mismatch");
    }

    Node *p = iter.current_;
    if (p == nullptr) {
      throw Exception("list iterator is null");
    }
    if (p->prev_ == nullptr || p->next_ == nullptr) {
      throw Exception("list iterator is null");
    }

    iterator ret_val(p->next_, *this);
    p->prev_->next_ = p->next_;
    p->next_->prev_ = p->prev_;
    {
      delete p;
      p = nullptr;
    }
    size_--;
    return ret_val;
  }

  /// \brief Erase the data in a certain range.
  iterator erase(iterator start, iterator stop) {
    if (start.list_ != this || stop.list_ != this) {
      throw Exception("list iterator mismatch");
    }

    if (start.current_ == nullptr || stop.current_ == nullptr) {
      throw Exception("list iterator is null");
    }

    if (start.current_->prev_ == nullptr || start.current_->next_ == nullptr ||
        stop.current_->prev_ == nullptr) {
      throw Exception("list iterator is null");
    }

    for (iterator iter = start; iter != stop;) {
      iter = erase(iter);
    }
    return stop;
  }

 private:
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
  ///< \brief the pointers to the previous & the next one
  Node *prev_, *next_;
};

}  // namespace dja

#endif