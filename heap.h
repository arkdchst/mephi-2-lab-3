#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

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

		bool operator==(const Node &node) const {
			return this->key == node.key && this->value == node.value;
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
