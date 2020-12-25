#include <cmath>
#include <iostream>
#include <queue>

template <class T>
class IsLess{
public:
    bool operator()(const T& lhs, const T& rhs) {
        return lhs < rhs;
    }
};

template <class T, class Comporator>
class BinTree {
public:
    BinTree();
    BinTree(Comporator);
    ~BinTree();
    void Add(const T& key);
    void InOrderDFS(void visit(T));
    int TreeHeight();

private:
    struct Node {
        Node(): left(nullptr), right(nullptr), parent(nullptr), key(NULL) {}
        Node(Node*& _parent, T _key):
            left(nullptr),
            right(nullptr),
            parent(_parent),
            key(_key) {}
        Node* left;
        Node* right;
        Node* parent;
        T key;        
    };
    Node* root;
    Comporator comp;
    void PostOrederDFS(void visit(Node*));
};

template <class T, class Comporator>
BinTree<T, Comporator>::BinTree(): root(nullptr), comp(IsLess<T>()) {}

template <class T, class Comporator>
BinTree<T, Comporator>::BinTree(Comporator _comp): root(nullptr), comp(_comp) {}

template <class T, class Comporator>
BinTree<T, Comporator>::~BinTree(){PostOrederDFS([](Node* node){delete node;});}

template <class T, class Comporator>
void BinTree<T, Comporator>::Add(const T& key) {
    if (root == nullptr) {
        root = new Node(root, key);
        return;
    }
    Node* node = root;
    Node* prev = root;
    while (node != nullptr) {
        prev = node;
        if (comp(node->key, key)) {
            node = prev->right;
        } else {
            node = prev->left;
        }
    }
    node = new Node(prev, key);
    if (comp(prev->key, key)) {
        prev->right = node;
    } else {
        prev->left = node;
    }
    return;
}

template <class T, class Comporator>
void BinTree<T, Comporator>::InOrderDFS(void visit(T)) {
    Node* node = root;
    Node* lastVisited = nullptr;
    while (node != nullptr) {
        if (lastVisited == node->parent) {
            if (node->left != nullptr) {
                lastVisited = node;
                node = node->left;
                continue;
            } else {
                lastVisited = nullptr;
            }
        }
        if (lastVisited == node->left) {
            visit(node->key);
            if (node->right != nullptr) {
                lastVisited = node;
                node = node->right;
                continue;
            } else {
                lastVisited = nullptr;
            }
        }
        if (lastVisited == node->right) { // возвращаемся наверх
            lastVisited = node;
            node = node->parent;
        }
    }
}

template <class T, class Comporator>
void BinTree<T, Comporator>::PostOrederDFS(void visit(Node*)) {
    Node* node = root;
    Node* lasVisited = nullptr;
    while (node != nullptr) {
        if (lasVisited == node->parent) {
            if (node->left != nullptr) {
                lasVisited = node;
                node = node->left;
                continue;
            } else {
                lasVisited = nullptr;
            }
        }
        if (lasVisited == node->left) {
            if (node->right != nullptr) {
                lasVisited = node;
                node = node->right;
                continue;
            } else {
                lasVisited = nullptr;
            }
        }
        if (lasVisited == node->right) {
            lasVisited = node;
            node = node->parent;
            visit(lasVisited);
        }

    }
}

template <class T, class Comporator>
int BinTree<T, Comporator>::TreeHeight() {
    if (!root) {
        return 0;
    }
    std::queue<Node*> nodes;
    nodes.push(root);
    int result = 0;
    for (; ;) {
        size_t nodesNumber = nodes.size();
        if (nodesNumber == 0) {
            return result;
        }
        ++result;
        while (nodesNumber != 0) {
            Node* node = nodes.front();
            nodes.pop();
            if (node->left) {
                nodes.push(node->left);
            }
            if (node->right) {
                nodes.push(node->right);
            }
            --nodesNumber;
        }
    }
}

template <class T, class Comparator>
class Treap {
    struct Node {
        Node():
        key(0), priority(0), left(nullptr), right(nullptr) {}

        Node(const T& _key, const int& _priority):
        key(_key), priority(_priority) {}

        T key;
        int priority;
        Node* left;
        Node* right;
    };

public:
    Treap(): root(nullptr), comp(IsLess<T>()) {}
    Treap(Comparator _comp): root(nullptr), comp(_comp) {}
    void Add(const T& _key, const int& _priority) {
        AddAux(root, _key, _priority);
    }
    ~Treap() {
        delete root;
    }
    int TreeHeight();

private:
    void AddAux(Node* node, const T& _key, const int& _priority);
    void Split(Node* node, const T& _key, Node*& left, Node*& right);
    Node* Merge(Node* left, Node* right) {
            if (left == nullptr || right == nullptr) {
            if (left == nullptr) {
                return right;
            } else {
                return left;
            }
        }
        if (left->priority > right->priority) {
            left->right = Merge(left->right, right);
            return left;
        } else {
            right->left = Merge(left, right->left);
        }
    }
    Node* root;
    Comparator comp;
};

template <class T, class Comparator>
void Treap<T, Comparator>::AddAux(Node* node, const T& _key, const int& _priority) {
    Node* newNode = new Node(_key, _priority);
    if (root == nullptr) {
        root = newNode;
        return;
    }
    Node* current = root;
    Node* parent = nullptr;
    while (current && current->priority > _priority) {
        parent = current;
        if (comp(_key, current->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    Split(current, _key, newNode->left, newNode->right);
    if (parent && comp(_key, parent->key)) {
        parent->left = newNode;
    } else if (parent && !comp(_key, parent->key)) {
        parent->right = newNode;
    } else {
        root = newNode;
    }
}

template <class T, class Comparator>
void Treap<T, Comparator>::Split(Node* node, const T& _key, Node*& left, Node*& right) {
    if (node == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    if (comp(_key, node->key)) {
        Split(node->left, _key, left, node->left);
        right = node;
    } else {
        Split(node->right, _key, node->right, right);
        left = node;
    }
}

template <class T, class Comparator>
int Treap<T, Comparator>::TreeHeight() {
    if (!root) {
        return 0;
    }
    std::queue<Node*> nodes;
    nodes.push(root);
    int result = 0;
    for (; ;) {
        size_t nodesNumber = nodes.size();
        if (nodesNumber == 0) {
            return result;
        }
        ++result;
        while (nodesNumber != 0) {
            Node* node = nodes.front();
            nodes.pop();
            if (node->left) {
                nodes.push(node->left);
            }
            if (node->right) {
                nodes.push(node->right);
            }
            --nodesNumber;
        }
    }
}

int main() {
    Treap<int, IsLess<int>> treap;
    BinTree<int, IsLess<int>> binTree;
    size_t operationsNumber = 0;
    std::cin >> operationsNumber;
    for (size_t i = 0 ; i < operationsNumber ; ++i) {
        int key = 0;
        int priority = 0;
        std::cin >> key >> priority;
        treap.Add(key, priority);
        binTree.Add(key);
    }
    std::cout << abs(binTree.TreeHeight() - treap.TreeHeight()) << std::endl;
    return 0;
}
