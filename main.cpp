// djalgo

#include <vector>
#include <cstdlib>
#include <iostream>

#include "test_data.h"

#include "algorithm/sort.h"

int main(int argc, char* argv[])
{
	dja::vector<Data> vec;

	constexpr int len = 10;
	for (int i = 0; i < len; ++i) {
		vec.push_back(rand() % len);
	}

	for (int i = 0; i < vec.size(); ++i) {
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";

	dja::sort<dja::BubbleSort>(vec);
	for (int i = 0; i < vec.size(); ++i) {
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";

	return 0;
}