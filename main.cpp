#include <list>
#include <functional>
#include <optional>
#include <iostream>

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

		Node(K key, V value) : key(key), value(value){

		}

		bool isLeft(){
			if(this->parent)
				return this->parent->left == this;
			return false;
		}

		bool isRight(){
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

public:

	BinarySearchTree(const std::function<int(K,K)> &cmp) : cmp(cmp){

	}

	void insert(const Node &node){
		if(root == nullptr){
			root = new Node(node);
			return;
		}

		Node *current = root;
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

	std::optional<Node> search(K key){
		if(root == nullptr){
			return std::nullopt;
		}

		Node *current = root;
		while(true){
			if(cmp(current->key, key) > 0){
				if(current->left == nullptr){
					return std::nullopt;
				}

				current = current->left;
			}
			else if(cmp(current->key, key) < 0){
				if(current->right == nullptr){
					return std::nullopt;
				}

				current = current->right;
			}
			else{
				return *current;
			}
		}
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

	void remove(K key){
		if(root == nullptr){
			return;
		}

		Node *current = root;
		while(true){
			if(cmp(current->key, key) > 0){
				if(current->left == nullptr){
					return;
				}

				current = current->left;
			}
			else if(cmp(current->key, key) < 0){
				if(current->right == nullptr){
					return;
				}

				current = current->right;
			}
			else{
				break;
			}
		}

		removeNode(current);
	}


};

int main(){
	BinarySearchTree<int, int> bst( [](int a, int b){return a - b;} );


	return 0;
}