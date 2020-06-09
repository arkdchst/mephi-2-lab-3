#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>

#include "bst.h"
#include "heap.h"

void bst_test(){
	std::cout << "BST TEST STARTED" << std::endl;

	typedef BinarySearchTree<int,int> BST;

	BST bst([](int a, int b){return a - b;});
	int COUNT = 100000000;
	int DC = 100000;

	std::vector<int> added(COUNT);


	srand(time(nullptr));
	for(int i = 0; i < COUNT / DC; i++){
		auto t1 = std::chrono::system_clock::now();
			for(int j = 0; j < DC; j++){
				int add = rand();
				BST::Node node(add, 0);
				bst.add(node);
				added[i * DC + j] = add;
			}
		auto t2 = std::chrono::system_clock::now();
			for(int j = 0; j < DC; j++){
				bst.search(added[rand() % (i + 1)]);
			}
		auto t3 = std::chrono::system_clock::now();



		std::cout << "add: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << ",";
		std::cout << i * DC + DC / 2 << std::endl;
		std::cout << "search: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << ",";
		std::cout << i * DC + DC / 2 << std::endl;
	}

	std::random_shuffle(added.begin(), added.end());


	for(int i = 0; i < COUNT / DC; i++){
		auto t1 = std::chrono::system_clock::now();
			for(int j = 0; j < DC; j++){
				try{
				bst.remove(added[i * DC + j]);
				}catch(...){}
			}
		auto t2 = std::chrono::system_clock::now();

		std::cout << "remove: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << ",";
		std::cout << COUNT - (i * DC + DC / 2) << std::endl;
	}
}

void heap_test(){
	std::cout << "HEAP TEST STARTED" << std::endl;

	typedef Heap<int,int> Heap;

	int COUNT = 100000000;
	int DC = 100000;

	Heap heap(COUNT, [](int a, int b){return a - b;});


	srand(time(nullptr));
	for(int i = 0; i < COUNT / DC; i++){

		auto t1 = std::chrono::system_clock::now();
			for(int j = 0; j < DC; j++){
				int add = rand();
				Heap::Node node(add, 0);
				heap.add(node);
			}
		auto t2 = std::chrono::system_clock::now();


		std::cout << "add: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << ",";
		std::cout << i * DC + DC / 2 << std::endl;
	}


	for(int i = 0; i < COUNT / DC; i++){
		auto t1 = std::chrono::system_clock::now();
			for(int j = 0; j < DC; j++){
				try{
				heap.pop();
				}catch(...){}
			}
		auto t2 = std::chrono::system_clock::now();

		std::cout << "pop: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << ",";
		std::cout << COUNT - (i * DC + DC / 2) << std::endl;
	}
}


int main(){
	bst_test();
	heap_test();

	return 0;
}