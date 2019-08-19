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

	std::system("pause");

	{
		dja::vector<Int> vec2(vec);

		std::system("pause");

		dja::sort<dja::ShellSort>(vec2);
		for (int i = 0; i < vec2.size(); ++i) {
			std::cout << vec2[i] << " ";
		}
		std::cout << "\n";
	}

	return 0;
}