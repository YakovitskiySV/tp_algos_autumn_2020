#include <iostream>
#include <cassert>
#include <stack>

template <class T>
class DefaultComparator {
    public:
    inline int operator()(const T& lhs, const T& rhs) {
        if (lhs < rhs) {
            return -1;
        }
        if (lhs == rhs) {
            return 0;
        }
        return 1;
    }
};

template <class Key, class Comparator=DefaultComparator<Key>>
class AVLTree {
public:
    AVLTree(const AVLTree&) = delete;
    AVLTree operator=(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;
    explicit AVLTree(Comparator _comp=Comparator()):
        root(nullptr), nodes_number(0), comp(_comp) {}
    
    ~AVLTree() {
        delete root;
    }

    bool Find(const Key& _key) {
        return FindAux(_key, root);
    }

    void Add(const Key& _key) {
        root = AddAux(_key, root);
    }

    void Erase(const Key& _key) {
        size_t curPos = 0;
        root = EraseAux(_key, root, curPos);
    }

    inline size_t Size() {
        return nodes_number;
    }

    size_t FindSoldierPos(const Key& _key) {
        size_t curPos = 0;
        return FindSoldierPosAux(_key, curPos, root);
    }

private:
    struct Node {
        Node():
            left(nullptr), right(nullptr), key(0), height(1), nodes(1) {}

        Node(const Key& _key):
            left(nullptr), right(nullptr), key(_key), height(1), nodes(1) {}

        Node* left;
        Node* right;
        Key key;
        uint8_t height;
        size_t nodes;
    };

    Node* root;
    size_t nodes_number;
    Comparator comp;

    size_t NodesNumber(Node* _node) {
        if (!_node) {
            return 0;
        }
        return _node->nodes;
    }

    void FixNodes(Node* _node) {
        if (!_node) {
            return;
        }
        _node->nodes = NodesNumber(_node->left) + NodesNumber(_node->right) + 1;
    }

    uint8_t Height(Node* _node) {
        if (!_node) {
            return 0;
        }
        return _node->height;
    }

    void FixHeight(Node* _node) {
        if (!_node) {
            return;
        }
        _node->height = std::max(Height(_node->left), Height(_node->right)) + 1;
    }

    Node* RotateLeft(Node* p) {
        Node* q = p->right;
        p->right = q->left;
        q->left = p;
        FixHeight(p);
        FixNodes(p);
        FixHeight(q);
        FixNodes(q);
        return q;
    }

    Node* RotateRight(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        FixHeight(p);
        FixNodes(p);
        FixHeight(q);
        FixNodes(q);
        return q;
    }

    Node* Balance(Node* _node) {
        FixHeight(_node);
        FixNodes(_node);
        int bf = BalanceFactor(_node);
        if (bf == 2) {
            if (BalanceFactor(_node->right) < 0) {
                _node->right = RotateRight(_node->right); 
            }
            return RotateLeft(_node);
        } else if (bf == -2) {
            if (BalanceFactor(_node->left) > 0) {
                _node->left = RotateLeft(_node->left);
            }
            return RotateRight(_node);
        }
        return _node;
    }

    inline int BalanceFactor(Node* _node) {
        return Height(_node->right) - Height(_node->left);
    }

    bool FindAux(const Key& _key, Node* _node) {
        if (!_node) {
            return false;
        }
        int comp_result = comp(_key, _node->key);
        if (comp_result == -1) {
            return FindAux(_key, _node->left);
        } else if (comp_result == 1) {
            return FindAux(_key, _node->right);
        }
        return true;
    }

    size_t FindSoldierPosAux(const Key& _key, size_t& _curPos, Node* _node) {
        if (!_node) {
            assert(false);
            return _curPos;
        }
        int comp_result = comp(_key, _node->key);
        if (comp_result == -1) {
            _curPos += NodesNumber(_node->right) + 1;
            return FindSoldierPosAux(_key, _curPos, _node->left);
        } else if (comp_result == 1) {
            return FindSoldierPosAux(_key, _curPos, _node->right);
        } else if (comp_result == 0) {
            _curPos += NodesNumber(_node->right);
        }
        return _curPos;
    }

    Node* AddAux(const Key& _key, Node* _node) {
        if (!_node) {
            ++nodes_number;
            return new Node(_key);
        }
        ++(_node->nodes);
        int comp_result = comp(_key, _node->key);
        if (comp_result == -1) {
            _node->left = AddAux(_key, _node->left);
        } else if (comp_result == 1) {
            _node->right = AddAux(_key, _node->right);
        }
        return Balance(_node);
    }    

    Node* EraseAux(const Key& _key, Node* _node, size_t& _curPos) {
        if (!_node) {
            return nullptr;
        }
        int comp_result = comp(_key - _curPos, NodesNumber(_node->right));
        if (comp_result == -1) {
            _node->right = EraseAux(_key, _node->right, _curPos);
        } else if (comp_result == 1) {
            _curPos += NodesNumber(_node->right) + 1;
            _node->left = EraseAux(_key, _node->left, _curPos);
        } else {
            _curPos += NodesNumber(_node->right);
            Node* left = _node->left;
            Node* right = _node->right;
            delete _node;
            --nodes_number;
            if (!right) {
                return left;
            }
            Node* min_node = FindAndRemoveMin(right);
            min_node->left = left;
            //--(min_node->nodes);
            return Balance(min_node);
        }
        return Balance(_node);
    }

    Node* FindAndRemoveMin(Node* _node) {
        Node* parent = nullptr;
        Node* current = _node;
        std::stack<Node*> nodes;
        while (current->left != nullptr) {
            parent = current;
            current = current->left;
            nodes.push(parent);
        }

        if (parent != nullptr) {
            parent->left = current->right;
            current->right = _node;

            Node* current_balance = nodes.top();
            nodes.pop();
            while (!nodes.empty()) {
                Node* parent_balance = nodes.top();
                nodes.pop();
                parent_balance->left = Balance(current_balance);
                current_balance = parent_balance;
            }
            current->right = Balance(current->right);
        }
        return current;
    }
};

int main() {
    AVLTree<int> tree;
    size_t operations_number = 0;
    std::cin >> operations_number;
    for (size_t i = 0 ; i < operations_number ; ++i) {
        int operation = 0;
        int key = 0;
        std::cin >> operation >> key;
        switch (operation)
        {
        case 1:
            tree.Add(key);
            std::cout << tree.FindSoldierPos(key) << std::endl;
            break;
        
        case 2:
            tree.Erase(key);
            break;
        }
    }
    return 0;
}