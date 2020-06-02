#include <list>
#include <functional>
#include <optional>
#include <iostream>
#include <stdexcept>
#include <string>

template <typename K, typename V>
class BinarySearchTree{
public:

	struct Node{
		K key;
		V value;

		Node *left = nullptr;
		Node *right = nullptr;
		Node *parent = nullptr;

		Node(const Node &node){
			this->key = node.key;
			this->value = node.value;
		}

		Node(K key, V value)
			: key(key), value(value){}

		bool isLeft() const {
			if(this->parent)
				return this->parent->left == this;
			return false;
		}

		bool isRight() const {
			if(this->parent)
				return this->parent->right == this;
			return false;
		}

		void operator=(const Node &node){
			this->key = node.key;
			this->value = node.value;
		}

	};

private:
	Node *root = nullptr;
	const std::function<int(K,K)> cmp;

	Node *getMinNode(Node *start){
		Node *current = start;
		while(current->left)
			current = current->left;

		return current;
	}


	void removeNode(Node *node){
		if(node->left == nullptr && node->right == nullptr){
			if(node->isLeft())			node->parent->left = nullptr;
			else if(node->isRight())	node->parent->right = nullptr;
			else this->root = nullptr;
			delete node;
		}
		else if(node->left == nullptr && node->right != nullptr){
			if(node->isLeft())			node->parent->left = node->right;
			else if(node->isRight())	node->parent->right = node->right;
			else this->root = node->right;
			node->right->parent = node->parent;
			delete node;
		}
		else if(node->left != nullptr && node->right == nullptr){
			if(node->isLeft())	node->parent->left = node->left;
			if(node->isRight())	node->parent->right = node->left;
			else{
				node->left->parent = nullptr;
				this->root = node->left;
			}
			node->left->parent = node->parent;
			delete node;
		}
		else if(node->left != nullptr && node->right != nullptr){
			Node *minNode = getMinNode(node->right);
			*node = *minNode;
			removeNode(minNode);
		}
	}

	Node* searchNode(K key) const {
		if(this->root == nullptr){
			return nullptr;
		}

		Node *current = this->root;
		while(true){
			if(cmp(current->key, key) > 0){
				if(current->left == nullptr){
					return nullptr;
				}

				current = current->left;
			}
			else if(cmp(current->key, key) < 0){
				if(current->right == nullptr){
					return nullptr;
				}

				current = current->right;
			}
			else{
				return *current;
			}
		}
	}


public:

	BinarySearchTree(const std::function<int(K,K)> &cmp)
		: cmp(cmp){}

	void add(const Node &node){
		if(this->root == nullptr){
			this->root = new Node(node);
			return;
		}

		Node *current = this->root;
		while(true){
			if(cmp(current->key, node.key) > 0){
				if(current->left == nullptr){
					current->left = new Node(node);
					current->left->parent = current;
					break;
				}

				current = current->left;
			}
			else if(cmp(current->key, node.key) < 0){
				if(current->right == nullptr){
					current->right = new Node(node);
					current->right->parent = current;
					break;
				}

				current = current->right;
			}
			else{
				current->value = node.value;
				break;
			}
		}
	}

	std::optional<Node> search(K key) const {
		Node *found = searchNode(key);
		if(found) return found;
		else return std::nullopt;
	}

	void remove(K key){
		Node *found = searchNode(key);		

		if(!found) throw std::logic_error("key not found");

		removeNode(found);
	}


};


template <typename K, typename V>
class Heap {
public:
	struct Node{
		K key;
		V value;

		Node(const Node &node){
			this->key = node.key;
			this->value = node.value;
		}

		Node(K key, V value)
			: key(key), value(value){}

		void operator=(const Node &node){
			this->key = node.key;
			this->value = node.value;
		}


	};

private:
	std::optional<Node> *heap;
	int size = 0;
	int capacity = 0;

	const std::function<int(K,K)> cmp;


	int parentIndex(int index) const {
		if(index == 0) return -1;
		return (index - 1) / 2;
	}

	int leftChildIndex(int index) const {
		int ret = index * 2 + 1;
		if(ret < size) return ret;
		else return -1;
	}

	int rightChildIndex(int index) const {
		int ret = index * 2 + 2;
		if(ret < size) return ret;
		else return -1;
	}


    void swap(int i, int j){
		Node t = heap[i].value();
		heap[i] = heap[j];
		heap[j] = t;
	}

	bool ok(int index){//true if position of element at index is right
		return		(parentIndex(index)		== -1 || cmp(heap[parentIndex(index)].value().key,		heap[index].value().key) < 0 )
				 && (leftChildIndex(index)	== -1 || cmp(heap[leftChildIndex(index)].value().key,	heap[index].value().key) > 0)
				 && (rightChildIndex(index)	== -1 || cmp(heap[rightChildIndex(index)].value().key,	heap[index].value().key) > 0 );
	}

	void sink(int index){
		while(!ok(index)){
			int newIndex = -1;
			if(rightChildIndex(index) == -1){
				newIndex = leftChildIndex(index);
			}else{
				if(cmp(heap[leftChildIndex(index)].value().key, heap[rightChildIndex(index)].value().key) < 0){
					newIndex = leftChildIndex(index);
				}else{
					newIndex = rightChildIndex(index);
				}
			}


			swap(newIndex, index);
			index = newIndex;
		}		
	}

	void swim(int index){
		while(!ok(index)){
			swap(parentIndex(index), index);
			index = parentIndex(index);
		}
	}

	std::string toString(std::string format, int index) const {
		if(index >= this->size || index == -1) return "";

		std::string out;
		for(int i = 0; i < format.size(); i++){
			if(format[i] == 'K'){//root
				out += std::to_string(heap[index].value().key);
			}
			else if(format[i] == 'L'){
				out += toString(format, leftChildIndex(index));
			}
			else if(format[i] == 'R'){
				out += toString(format, rightChildIndex(index));
			}
			else{
				out += format[i];
			}
		}

		return out;
	}


public:
	Heap(int capacity, const std::function<int(K,K)> &cmp) : capacity(capacity), cmp(cmp) {
		heap = new std::optional<Node>[capacity];
		size = 0;
	}
	

	void add(const Node &node){
		if(this->size == this->capacity) throw std::length_error("heap is full");
		heap[size++] = node;

		swim(size - 1);
	}
	
	
	Node poll(){
		if(size == 0) throw std::out_of_range("size is 0");
		Node ret(heap[0].value());
		heap[0] = heap[size - 1];
		size--;

		this->sink(0);

		return ret;
	}

	Node peek(){
		if(size == 0) throw std::out_of_range("size is 0");

		return heap[0].value();
	}

	int getSize() const {
		return this->size;
	}
	
	std::string toString(std::string format) const {
		return toString(format, 0);
	}

};


int main(){
	BinarySearchTree<int, int> bst( [](int a, int b){return a - b;} );
	Heap<int,int> heap(10,  [](int a, int b){return a - b;} );

	Heap<int, int>::Node node4(4,4); heap.add(node4);
	Heap<int, int>::Node node9(9,9); heap.add(node9);
	Heap<int, int>::Node node3(3,3); heap.add(node3);
	Heap<int, int>::Node node6(6,6); heap.add(node6);
	Heap<int, int>::Node node10(10,10); heap.add(node10);
	Heap<int, int>::Node node1(1,1); heap.add(node1);
	Heap<int, int>::Node node5(5,5); heap.add(node5);
	Heap<int, int>::Node node2(2,2); heap.add(node2);
	Heap<int, int>::Node node7(7,7); heap.add(node7);
	Heap<int, int>::Node node8(8,8); heap.add(node8);

	std::cout << heap.toString("K:(L)(R)") << std::endl;

	while(heap.getSize() > 0){
		std::cout << heap.poll().key << std::endl;
	}


	return 0;
}