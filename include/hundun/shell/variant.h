#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// This is a cpp implementation meta class using RTTI for type erase.
//

#ifndef DJA_VARIANT_H_
#define DJA_VARIANT_H_

#include "traits_variant.h"

namespace dja {
template <typename... Ts>
struct VariantHelper;

template <typename F, typename... Ts>
struct VariantHelper<F, Ts...> {
  /// \brief Release data node.
  inline static void destroy(size_t id, void* data) {
    if (id == typeid(F).hash_code()) {
      reinterpret_cast<F*>(data)->~F();
    } else {
      VariantHelper<Ts...>::destroy(id, data);
    }
  }

  inline static void move(size_t id, void* data_old, void* data_new) {
    if (id == typeid(F).hash_code()) {
      new (data_new) F(std::move(*reinterpret_cast<F*>(data_old)));
    } else {
      VariantHelper<Ts...>::move(id, data_old, data_new);
    }
  }

  inline static void copy(size_t id, const void* data_old, void* data_new) {
    if (id == typeid(F).hash_code()) {
      new (data_new) F(*reinterpret_cast<const F*>(data_old));
    } else {
      VariantHelper<Ts...>::copy(id, data_old, data_new);
    }
  }
};

/// \brief The recursion end point.
template <>
struct VariantHelper<> {
  inline static void destroy(size_t id, void* data) {}
  inline static void move(size_t id, void* data_old, void* data_new) {}
  inline static void copy(size_t id, const void* data_old, void* data_new) {}
};

template <typename... Ts>
class Variant {
  enum {
    data_size = MaxInteger<sizeof(Ts)...>::value,
    data_align = MaxAlign<Ts...>::value
  };

  using helper_t = typename VariantHelper<Ts...>;

  using data_t = typename std::aligned_storage<data_size, data_align>::type;

 public:
  // build/clear up objects

  Variant() : type_id_(invalid_type()) {}

  Variant(const Variant<Ts...>& src) : type_id_(src.type_id_) {
    helper_t::copy(src.type_id_, &src.data_, &data_);
  }

  Variant(Variant<Ts...>&& src) : type_id_(src.type_id_) {
    helper_t::move(src.type_id_, &src.data_, &data_);
  }

  template <typename T>
  Variant(const T& val) : type_id_(invalid_type()) {
    helper_t::destroy(type_id_, &data_);  // destroy current contents first
    new (&data_) T(val);
    type_id_ = typeid(T).hash_code();
  }

  ~Variant() { helper_t::destroy(type_id_, &data_); }

  // operator overload

  /// \brief
  ///
  /// \attention It serves as both the move and the copy assignment operator.
  Variant<Ts...>& operator=(Variant<Ts...> src) {
    std::swap(data_, src.data_);
    std::swap(type_id_, src.type_id_);
    return *this;
  }

  /// \brief A significant support to insert variant instance into a
  /// std::map/std::multimap.
  bool operator<(const Variant& src) const { return type_id_ < src.type_id_; }

  // get/set

  template <typename T>
  T& get() {
    if (is<T>()) {
      return *reinterpret_cast<T*>(&data_);  // a dynamic_cast - like behaviour
    } else {
      throw std::bad_cast();
    }
  }

  template <typename T, typename... Args>
  void set(Args&&... args) {
    helper_t::destroy(type_id_, &data_);  // destroy current contents first
    new (&data_) T(std::forward<Args>(args)...);
    type_id_ = typeid(T).hash_code();
  }

  // object status

  template <typename T>
  bool is() const {
    return (type_id_ == typeid(T).hash_code());
  }

  bool empty() const { return (type_id_ == invalid_type()); }

  template <typename F, typename... Rest>
  void visit(F&& head, Rest&&... rest) {
    using T = typename function_traits<F>::template arg<0>::type;
    if (is<T>()) {
      visit(std::forward<F>(head));
    } else {
      visit(std::forward<Rest>(rest)...);
    }
  }

  /// \brief The recursion endpoint.
  template <typename F>
  void visit(F&& f) {
    using T = typename function_traits<F>::template arg<0>::type;
    if (is<T>()) {
      f(get<T>());
    }
  }

 private:
  /// \brief Returns a unique hash code of a certain type.
  static inline size_t invalid_type() { return typeid(void).hash_code(); }

  data_t data_;

  size_t type_id_;
};
}  // namespace dja

#endif
