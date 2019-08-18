#pragma once

// Copyright (C) summer 2019 Jiaheng Wang
// License(BSD)
// Author: Jiaheng Wang <wjhgeneral@outlook.com>
// This is a user-defined class to test algorithms.
//

#ifndef DJA_data_H_
#define DJA_data_H_

#include <iostream>

class Int {
public:
	Int(int data = 0)
		:
		data_(data) {}

	Int(const Int& src)
		:
		data_(src.data_) {}

	~Int() {}

	const Int& operator=(const Int& rhs) {
		if (this != &rhs) {
			data_ = rhs.data_;
		}
		return *this;
	}

	bool operator>(const Int& rhs) {
		return data_ > rhs.data_;
	}

	bool operator<(const Int& rhs) {
		return data_ < rhs.data_;
	}

	bool operator==(const Int& rhs) {
		return data_ == rhs.data_;
	}

	friend std::ostream& operator<<(std::ostream& os, const Int& src);

private:
	int data_;
};

#endif
