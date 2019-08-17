#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// This is a container, similar to the std::vector.
//

#ifndef DJA_VECTOR_H_
#define DJA_VECTOR_H_

namespace dja {
	template <class T>
	class vector {
	public:
		class OutOfBoundsException {};

		class EmptyVectorException {};

		/// \brief The constructor.
		vector(int size = 0) {
			init(size);
		}

		/// \brief The copy constructor.
		vector(const vector& src) {
			data_ = nullptr;
			operator=(src);
		}

		/// \brief The destructor.
		~vector() {
			destroy();
		}

		const vector& operator=(const vector& rhs) {
			if (this != &rhs) {
				destroy();
				{
					setSize(rhs.size_);
					setCapacity(rhs.capacity_);
					data_ = new T[capacity_];
				}
				for (int i = 0; i < size_; ++i) {
					data_[i] = rhs.data_[i];
				}
			}
			return *this;
		}

		void resize(int new_size) {
			if (new_size <= capacity_) {
				setSize(new_size);
				return;
			}
			reserve(2 * new_size + 1);
			setSize(new_size);
		}

		void reserve(int new_capacity) {
			if (new_capacity < size_) {
				return;
			}
			// take previous data
			T* old_data;
			setCapacity(new_capacity);
			old_data = data_;
			data_ = new T[capacity_];
			for (int i = 0; i < size_; ++i) {
				data_[i] = old_data[i];
			}
			delete[] old_data;
		}

		/// \brief Get values.
		T& operator[](int idx) {
			if (idx < 0 || idx >= size_) {
				throw OutOfBoundsException();
			}
			return data_[idx];
		}

		const T& operator[](int idx) const {
			if (idx < 0 || idx >= size_) {
				throw OutOfBoundsException();
			}
			return data_[idx];
		}

		T& at(int idx) {
			// thrown in for stl vector compatibility
			return (*this)[idx];
		}

		const T& at(int idx) const {
			return (*this)[idx];
		}

		bool empty() const {
			return (size_ == 0);
		}

		int size() const {
			return size_;
		}

		int capacity() const {
			return capacity_;
		}

		void push_back(const T& src) {
			if (size_ == capacity_) {
				reserve(2 * capacity_ + 1);
			}
			data_[size_++] = src;
		}

		void pop_back() {
			if (size_ > 0) { size_--; }
		}

		const T& back() const {
			if (size_ <= 0) {
				throw EmptyVectorException();
			}
			return data_[size_ - 1];
		}

		const T& front() const {
			if (size_ <= 0) {
				throw EmptyVectorException();
			}
			return data_[0];
		}

		/// \brief To
		void clear() {
			// call the overload function of assignment
			*this = vector<T>();
		}

		typedef T* iterator;
		typedef const T* const_iterator;

		iterator begin() {
			return &data_[0];
		}

		const_iterator begin() const {
			return &data_[0];
		}

		iterator end() {
			return &data_[size()];
		}

		const_iterator end() const {
			return &data_[size()];
		}

		/// \brief To erase objects in a certain range.
		///
		/// \warning Objects must have good deep copy management.
		/// \attention It moves pointer directly.
		iterator erase(iterator first, iterator last) {
			if (first < begin() || last > end() || first >= last) {
				// invalid sequence
				return nullptr;
			}
			iterator iter1, iter2, retVal, endVal;
			{
				// prepare a return value (first element after erase block)
				retVal = first;
				// for faster looping
				endVal = end();
			}
			for (iter1 = first, iter2 = last; iter2 != endVal;) {
				*iter1++ = *iter2++;
			}
			setSize(size_ - (last - first));
			// points to first element not erased after block
			return retVal;
		}

		iterator erase(iterator item_to_erase) {
			return erase(item_to_erase, item_to_erase + 1);
		}

	private:
		void setSize(int size) {
			size_ = (size < 0) ? 0 : size;
		}

		void setCapacity(int capacity) {
			capacity_ = (capacity <= size_) ? size_ + spare_capacity_ : capacity;
		}

		void init(int size) {
			setSize(size);
			setCapacity(size_ + spare_capacity_);
			data_ = new T[capacity_];
		}

		void destroy() {
			if (data_) {
				delete[] data_;
				data_ = nullptr;
			}
		}

		int size_;
		int capacity_;
		T* data_;

		static const int spare_capacity_ = 16;
	};
}

#endif
