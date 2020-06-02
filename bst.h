#include <functional>
#include <optional>
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
				return current;
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
		if(found) return *found;
		else return std::nullopt;
	}

	void remove(K key){
		Node *found = searchNode(key);		

		if(!found) throw std::logic_error("key not found");

		removeNode(found);
	}


};
