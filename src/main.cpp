#include <iostream>
#include <chrono>
#include "skiplist.h"

// test number of data
#define TEST_COUNT 10000
skiplist<string> skiplist1(15);

int main() {
	/*
	 * test of insert, find and erase
	 *
	 * */
	/*
	skiplist<string> skiplist1(20);
	skiplist1.print();

	skiplist1.insert(1, "1");
	skiplist1.insert(2, "2");
	skiplist1.insert(3, "3");
	skiplist1.insert(4, "4");
	skiplist1.insert(6, "6");
	skiplist1.insert(7, "7");
	skiplist1.insert(9, "9");
	skiplist1.insert(10, "10");
	skiplist1.insert(11, "11");
	skiplist1.print();

	skiplist1.insert(1, "1");
	skiplist1.insert(8, "8");
	skiplist1.print();

	skiplist1.find(1);
	skiplist1.find(2);
	skiplist1.find(8);
	skiplist1.find(5);
	skiplist1.find(12);

	skiplist1.erase(1);
	skiplist1.erase(3);
	skiplist1.erase(4);
	skiplist1.erase(2);
	skiplist1.erase(5);
	skiplist1.erase(12);
	skiplist1.print();

	skiplist1.erase(6);
	skiplist1.erase(7);
	skiplist1.erase(8);
	skiplist1.print();

	skiplist1.erase(9);
	skiplist1.erase(10);
	skiplist1.erase(11);
	skiplist1.print();

	skiplist1.erase(1);
	*/

	/*
	 * stress test, replace of test.cpp
	 * there's some problem in test.cpp in ./stress_test/
	 *
	 * */
	// insert test
	vector<std::chrono::duration<double> > time_record;

	auto start = std::chrono::high_resolution_clock::now();

	for(int i = 1; i < TEST_COUNT; ++i) {
		skiplist1.insert(i, "1");
	}

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "insert elapsed:" << elapsed.count() << std::endl;
	time_record.push_back(elapsed);
	/////////////////////////////////////////
	// find test
	start = std::chrono::high_resolution_clock::now();

	for(int i = 1; i < TEST_COUNT; ++i) {
		skiplist1.find(i);
	}

	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "find elapsed:" << elapsed.count() << std::endl;
	time_record.push_back(elapsed);


	skiplist1.dump_file();
	/////////////////////////////////////////
	// erase test
	start = std::chrono::high_resolution_clock::now();

	for(int i = 1; i < TEST_COUNT; ++i) {
		skiplist1.erase(i);
	}

	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "erase elapsed:" << elapsed.count() << std::endl;
	time_record.push_back(elapsed);


	// result time print
	cout << endl;
	cout << "insert elapsed:" << time_record[0].count() << endl;
	cout << "find elapsed:" << time_record[1].count() << endl;
	cout << "erase elapsed:" << time_record[2].count() << endl;

	return 0;
}
