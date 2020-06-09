#define BOOST_TEST_MODULE tests
#include <boost/test/included/unit_test.hpp>

#include "bst.h"
#include "heap.h"

#include <optional>
#include <iostream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(bst_test){
	typedef BinarySearchTree<int,int> BST;

	BST bst([](int a, int b){return a - b;});

	const int COUNT = 10;

	for(int i = 0; i < COUNT; i++){
		BST::Node node(i,i + 1); bst.add(node);
	}

	for(int i = 0; i < COUNT; i++){
		std::optional<BST::Node> node = bst.search(i);
		BOOST_TEST(node.has_value());
		if(node.has_value())
			BOOST_TEST(node.value().value == i + 1);
	}
	BOOST_TEST(!bst.search(COUNT).has_value());

	for(int i = COUNT-1; i >= 0; i--){
		BOOST_CHECK_NO_THROW(bst.remove(i));
		BOOST_TEST(!bst.search(i).has_value());
	}

	BOOST_CHECK_THROW(bst.remove(0), std::exception);
}

BOOST_AUTO_TEST_CASE(heap_test){
	typedef Heap<int,int> Heap;

	const int COUNT = 10;

	Heap heap(COUNT, [](int a, int b){return a - b;});


	for(int i = COUNT - 1; i >= 0; i--){
		Heap::Node node(i,i + 1);
		heap.add(node);
	}

	for(int i = 0; i < COUNT; i++){
		std::optional<Heap::Node> node = heap.search(i);
		BOOST_TEST(node.has_value());
		if(node.has_value())
			BOOST_TEST(node.value().value == i + 1);
	}
	BOOST_TEST(!heap.search(COUNT).has_value());

	BOOST_TEST(heap.getSize() == COUNT);

	BOOST_CHECK_THROW(heap.add(Heap::Node(-1,0)), std::exception);


	int prev = 0;
	for(int i = 0; i < COUNT; i++){
		Heap::Node nodeOnTop = heap.peek();
		Heap::Node node = heap.pop();
		if(i == 0){
			prev = node.value;
		}
		else{
			BOOST_TEST(node.value >= prev);
			prev = node.value;
		}

		BOOST_TEST((nodeOnTop == node));
		BOOST_TEST(node.value == i + 1);
	}

	BOOST_CHECK_THROW(heap.pop(), std::exception);
	BOOST_TEST(heap.getSize() == 0);
}