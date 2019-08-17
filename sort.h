#pragma once

#include "vector.h"

namespace dja {
	template <typename Comparable>
	void insertionSort(vector<Comparable>& a) {
		int k, pos, arraySize;
		Comparable tmp;

		arraySize = a.size();
		for (pos = 1; pos < arraySize; pos++) {
			tmp = a[pos];
			for (k = pos; k > 0 && tmp < a[k - 1]; k--)
				a[k] = a[k - 1];
			a[k] = tmp;
		}
	}
}