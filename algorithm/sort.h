#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// Sorting algorithms in C++.
//

#ifndef DJA_SORT_H_
#define DJA_SORT_H_

#include <iostream>

#include "../structure/vector.h"

namespace dja {
	/// \brief The abstract base class for sorting methods.
	template <typename Comparable>
	class Sort {
	public:
		virtual void operator()(vector<Comparable>&) = 0;
	};

	template <typename Comparable>
	class BubbleSort : public Sort<Comparable> {
	public:
		void operator()(vector<Comparable>& arr) {
			Comparable temp;
			int size = arr.size();
			for (int i = 0; i < size - 1; ++i) {
				for (int j = 0; j < size - i - 1; ++j) {
					if (arr[j] > arr[j + 1]) {
						temp = arr[j];
						arr[j] = arr[j + 1];
						arr[j + 1] = temp;
					}
				}
			}
		}
	};

	template <typename Comparable>
	class InsertionSort : public Sort<Comparable> {
	public:
		void operator()(vector<Comparable>& arr) {
			Comparable temp;
			int size = arr.size();
			for (int i = 1; i < size; ++i) {
				temp = arr[i];
				int j = i;
				for (; j > 0 && temp < arr[j - 1]; --j) {
					arr[j] = arr[j - 1];
				}
				arr[j] = temp;
			}
		}
	};

	/// \brief A template function using sorting methods (class name) as template to sort a given vector.
	template <template <typename T> typename SortMethod, typename Comparable>
	typename std::enable_if<std::is_base_of<Sort<Comparable>, SortMethod<Comparable>>::value>::type
		sort(vector<Comparable>& arr) {
		SortMethod<Comparable> sorter;
		sorter(arr);
	}
}

#endif
