#include "bst.h"
#include "heap.h"

#include <iostream>
#include <string>
#include <exception>
#include <limits>
#include <cmath>


void print(std::string str){
	if(str.empty()) return;

	if(str.back() == ':')
		std::cout << str << "\n";
	else
		std::cout << str << "\n\n";

	std::cout.flush();
}

int readInt(int from=std::numeric_limits<int>::min(), int to=std::numeric_limits<int>::max()){
	while(true){
		try{
			std::string line;
			std::getline(std::cin, line);
			if(line.find_first_not_of("0123456789") != -1) throw std::exception();

			int read = std::stoi(line);
			if(read < from || read > to) throw std::exception();

			std::cout << "\n";
			return read;
		}catch(...){
			print("please, enter a number from " + std::to_string(from) + " to " + std::to_string(to) + ":");
		}

	}
}

void printMainMenu(){
	print("1. BST (submenu)\n2. min-heap (submenu)\n3. exit");
}

void printBstMenu(){
	print("1. add node\n2. add nodes range\n3. search by key\n4. remove by key\n5. return to main menu");
}

void printHeapMenu(bool heapExists){
	if(heapExists){
		print("1. recreate heap\n2. add node\n3. add nodes range\n4. search by key\n5. pop\n6. peek\n7. to string\n8. get size\n9. return to main menu");
	}
	else{
		print("1. create heap\n2. return to main menu");
	}
}


void bstMenu(){
	typedef BinarySearchTree<int, int> BST;
	BST bst([](int a, int b){return a-b;});
	while(true){
		printBstMenu();
		int choice = readInt(1, 5);
		switch(choice){
			case 1:
				{
				print("enter key:");
				int key = readInt();
				print("enter value:");
				int value = readInt();
				bst.add(BST::Node(key, value));
				break;
				}
			case 2:
				{
				print("fill BST with nodes with keys and values from start to finish\nenter start:");
				int start = readInt();
				print("enter finish:");
				int finish = readInt();
				if(start <= finish){
					for(int i = start; i <= finish; i++)
						bst.add(BST::Node(i, i));
				}
				else{
					for(int i = start; i >= finish; i--)
						bst.add(BST::Node(i, i));						
				}
				break;
				}
			case 3:
				{
				print("enter key:");
				int key = readInt();
				std::optional<BST::Node> node = bst.search(key);
				if(node){
					print("node found:\n\tkey: " + std::to_string(node.value().key) + "\n\tvalue: " + std::to_string(node.value().value));
				}
				else{
					print("node not found");
				}
				break;
				}
			case 4:
				{
				print("enter key:");
				int key = readInt();
				try{
					bst.remove(key);
					print("node removed");
				}catch(...){
					print("node not found");
				}
				break;
				}
			case 5:
				return;
		}
	}
}

void heapMenu(){
	typedef Heap<int, int> Heap;
	Heap *heap = nullptr;
	while(true){
		if(heap){
			printHeapMenu(true);
			int choice = readInt(1, 9);
			switch(choice){
				case 1:
					{
					delete heap;
					print("enter heap capacity:");
					int capacity = readInt(1);
					heap = new Heap(capacity, [](int a, int b){return a-b;});
					break;
					}
				case 2:
					{
					if(heap->getSize() == heap->getCapacity()){
						print("heap is full");
						break;
					}
					print("enter key:");
					int key = readInt();
					print("enter value:");
					int value = readInt();
					heap->add(Heap::Node(key, value));
					break;
					}
				case 3:
					{
					print("fill heap with nodes with keys and values from start to finish\nenter start:");
					int start = readInt();
					print("enter finish:");
					int finish = readInt();
					if(std::abs(finish - start) + 1 + heap->getSize() > heap->getCapacity()){
						print("it's to big for this heap");
						break;
					}
					if(start <= finish){
						for(int i = start; i <= finish; i++)
							heap->add(Heap::Node(i, i));
					}
					else{
						for(int i = start; i >= finish; i--)
							heap->add(Heap::Node(i, i));						
					}
					break;
					}
				case 4:
					{
					print("enter key:");
					int key = readInt();
					std::optional<Heap::Node> node = heap->search(key);
					if(node){
						print("node found:\n\tkey: " + std::to_string(node.value().key) + "\n\tvalue: " + std::to_string(node.value().value));
					}
					else{
						print("node not found");
					}
					break;
					}
				case 5:
					{
					if(heap->getSize() == 0){
						print("heap is empty");
						break;
					}

					Heap::Node node = heap->pop();
					print("node:\n\tkey: " + std::to_string(node.key) + "\n\tvalue: " + std::to_string(node.value));

					break;
					}
				case 6:
					{
					if(heap->getSize() == 0){
						print("heap is empty");
						break;
					}

					Heap::Node node = heap->peek();
					print("node:\n\tkey: " + std::to_string(node.key) + "\n\tvalue: " + std::to_string(node.value));
					break;
					}
				case 7:
					{
					print("use K for root, L for left subtree, R for right subtree\nenter format:");
					std::string format;
					std::getline(std::cin, format);
					print(heap->toString(format));
					break;
					}
				case 8:
					print(std::to_string(heap->getSize()));
					break;
				case 9:
					delete heap;
					return;
					break;
			}			
		}
		else{
			printHeapMenu(false);
			int choice = readInt(1, 2);
			switch(choice){
				case 1:
					{
					print("enter heap capacity:");
					int capacity = readInt(1);
					heap = new Heap(capacity, [](int a, int b){return a-b;});
					break;
					}
				case 2:
					delete heap;
					return;
					break;
			}
		}
	}
}

void mainMenu(){
	while(true){
		printMainMenu();
		int choice = readInt(1, 3);
		switch(choice){
			case 1:
				bstMenu();
				break;
			case 2:
				heapMenu();
				break;
			case 3:
				return;
		}
	}

}


int main(){
	mainMenu();

	return 0;
}