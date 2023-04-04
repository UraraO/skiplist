#include <iostream>
#include <chrono>
#include "../src/skiplist.h"

#define TEST_COUNT 100000
skiplist<string> skiplist1(18);

int main() {
	auto start = std::chrono::high_resolution_clock::now();

	for(int i = 1; i < TEST_COUNT; ++i) {
		skiplist1.insert(i, "1");
	}

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "insert elapsed:" << elapsed.count() << std::endl;

	return 0;

}