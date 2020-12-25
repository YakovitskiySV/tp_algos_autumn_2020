#include <iostream>
#include <cassert>
#include <vector>
#include <stack>

template <class T>
class IsLess{
public:
    bool operator()(const T& lhs, const T& rhs) {
        return lhs <= rhs;
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

private:
    struct Node {
        Node(): left(nullptr), right(nullptr), key(NULL) {}
        Node(T _key):
            left(nullptr),
            right(nullptr),
            key(_key) {}
        Node* left;
        Node* right;
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
BinTree<T, Comporator>::~BinTree() {
    PostOrederDFS([](Node* node) {
        delete node;
        }
        );
    }

template <class T, class Comporator>
void BinTree<T, Comporator>::Add(const T& key) {
    if (root == nullptr) {
        root = new Node(key);
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
    node = new Node( key);
    if (comp(prev->key, key)) {
        prev->right = node;
    } else {
        prev->left = node;
    }
    return;
}

template <class T, class Comporator>
void BinTree<T, Comporator>::InOrderDFS(void visit(T)) {
    if (root ==  nullptr) {
        return;
    }
    std::stack<Node*> nodes;
    Node* node = root;
    while (node || !nodes.empty()) {
        if (node) {
        nodes.push(node);
        node = node->left;
        } else {
            Node* popped_node = nodes.top();
            nodes.pop();
            visit(popped_node->key);
            node = popped_node->right;
        }
    }
}

template <class T, class Comporator>
void BinTree<T, Comporator>::PostOrederDFS(void visit(Node*)) {
    std::stack<Node*> stack1;
    std::stack<Node*> stack2;
    stack1.push(root);
    while (!stack1.empty()) {
        Node* node = stack1.top();
        stack2.push(node);
        stack1.pop();
        if (node->left) {
            stack1.push(node->left);
        }
        if (node->right) {
            stack1.push(node->right);
        }
    }
    while (!stack2.empty()) {
        Node* node = stack2.top();
        visit(node);
        stack2.pop();
    }
}

void test() {
    BinTree<int, IsLess<int>> tree;
    tree.Add(9);
    tree.Add(10);
    tree.Add(4);
    tree.Add(3);
    tree.Add(2);
    tree.Add(7);
    tree.Add(8);
    tree.Add(5);
    tree.Add(1);
    tree.Add(6);
    tree.InOrderDFS([](int val){std::cout << val << ' ';});
    return;
}

int main() {
    BinTree<int, IsLess<int>> tree;
    //test();
    size_t n = 0;
    std::cin >> n;
    for (size_t i = 0 ; i < n ; ++i) {
        int elem = 0;
        std::cin >> elem;
        tree.Add(elem);
    }
    tree.InOrderDFS([](int val){std::cout << val << ' ';});
    return 0;
}