#pragma once

#ifndef DJA_TEST_DATA_H_
#define DJA_TEST_DATA_H_

class Data {
public:
	Data(int data = 0)
		:
		data_(data) {
	}

	~Data() {
	}

	const Data& operator=(const Data& rhs) {
		if (this != &rhs) {
			data_ = rhs.data_;
		}
		return *this;
	}

	bool operator>(const Data& rhs) {
		return data_ > rhs.data_;
	}

	bool operator<(const Data& rhs) {
		return data_ < rhs.data_;
	}

	bool operator==(const Data& rhs) {
		return data_ == rhs.data_;
	}

	friend std::ostream& operator<<(std::ostream& os, const Data& src);

private:
	int data_;
};

std::ostream& operator<<(std::ostream& os, const Data& src) {
	os << src.data_;
	return os;
}

#endif
