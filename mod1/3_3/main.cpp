#include <iostream>
#include <cassert>
#include <cstring>
#define PUSH_OPERATION 3
#define POP_OPERATION 2
#define DEFAULT_GROW 2

template<class T>
class Stack {
public:
    Stack();
    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;
    Stack operator =(const Stack&) = delete;
    Stack operator =(Stack&&) = delete;
    ~Stack();
    void Push(const T&);
    T Pop();
    bool IsEmpty();

private:
    void Grow();
    size_t size;
    size_t topNumber;
    T* data;
};

template<class T>
Stack<T>::Stack() :
        size(0),
        topNumber(0),
        data(nullptr)
{}

template<class T>
Stack<T>::~Stack() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
}

template<class T>
void Stack<T>::Grow() {
    if (size == 0) {
        data = new T[++size];
        return;
    }
    if (topNumber < size) {
        return;
    }
    size *= DEFAULT_GROW;
    T* buf = new T[size];
    for (size_t i = 0 ; i < topNumber ; ++i) {
        buf[i] = data[i];
    }
    delete[] data;
    data = buf;
}

template<class T>
bool Stack<T>::IsEmpty() {
    if (size != 0 && topNumber != 0) {
        return false;
    }
    return true;
}

template<class T>
void Stack<T>::Push(const T& k) {
    if (IsEmpty() || topNumber == size) {
        Grow();
    }
    data[topNumber++] = k;
}

template<class T>
T Stack<T>::Pop() {
    if (IsEmpty()) {
        return -1;
    }
    return data[--topNumber];
}

template<class T>
class Queue {
public:
    Queue();
    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;
    Queue operator =(const Queue&) = delete;
    Queue operator =(Queue&&) = delete;
    ~Queue();
    void Push(const T&);
    T Pop();
    bool IsEmpty();
private:
    Stack<T> left;
    Stack<T> right;
};

template<class T>
Queue<T>::Queue() {}

template<class T>
Queue<T>::~Queue() {}

template<class T>
bool Queue<T>::IsEmpty() {
    if (left.IsEmpty() && right.IsEmpty()) {
        return true;
    }
    return false;
}

template<class T>
void Queue<T>::Push(const T& k) {
    left.Push(k);
}

template<class T>
T Queue<T>::Pop() {
    if (IsEmpty()) {
        return -1;
    }
    if (right.IsEmpty()) {
        while (!left.IsEmpty()) {
            right.Push(left.Pop());
        }
    }
    return right.Pop();
}

template<class T>
bool Answer(Queue<T>& qu, const int& operation, const T& value) {
    switch (operation) {
        case PUSH_OPERATION:
            qu.Push(value);
            return true;
        case POP_OPERATION:
            if (qu.Pop() != value) {
                return false;
            }
            return true;
        default:
            return false;
    }
}

int main() {
    int numberOfOperations = 0;
    std::cin >> numberOfOperations;
    assert(numberOfOperations > 0);
    Queue<int> qu = {};
    for (int i = 0; i < numberOfOperations; ++i) {
        int operation = 0;
        int value = 0;
        std::cin >> operation >> value;
        if (!Answer(qu, operation, value)) {
            std::cout << "NO" << std::endl;
            return 0;
        }
    }
    std::cout << "YES" << std::endl;
    return 0;
}
