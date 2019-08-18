// djalgo

#include <vector>
#include <cstdlib>
#include <iostream>

#include "include/int.h"

#include "algorithm/sort.h"

int main(int argc, char* argv[])
{
	dja::vector<Int> vec;

	constexpr int len = 20;
	for (int i = 0; i < len; ++i) {
		vec.push_back(rand() % len);
	}

	for (int i = 0; i < vec.size(); ++i) {
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";

	dja::sort<dja::ShellSort>(vec);
	for (int i = 0; i < vec.size(); ++i) {
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";

	return 0;
}