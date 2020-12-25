 /*
  Дан массив целых чисел A[0..n), n не превосходит 100 000.
  Так же задан размер некотрого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n.
  Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне.
  Скорость работы O(n log n), память O(n).
*/

#include <iostream>
#include <cstring>
#include <cassert>

#define DEFAULT_GROW 2

template<class T>
class IsLess {
public:
    bool operator () (const T& lhs, const T& rhs) const {
        return (lhs < rhs);
    }
};

template <class T, class H = IsLess<T>>
class Heap {
public:
    Heap();
    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;
    Heap operator = (const Heap&) = delete;
    Heap operator = (Heap&&) = delete;
    ~Heap();
    bool IsEmpty();
    void Insert(const T &k);
    T ExtractMax();
    const T &Top() const;

private:
    void Grow();
    void BuildHeap();
    void SiftDown(const int &i);
    void SiftUp(int i);
    size_t capacity;
    size_t size;
    T *data;
    H comparator;
};

template<class T, class H>
Heap<T, H>::Heap():
        capacity(0),
        size(0),
        data(nullptr),
        comparator(IsLess<T>())
{}

template<class T, class H>
Heap<T, H>::~Heap() {
    delete[] data;
}

template<class T, class H>
void Heap<T, H>::Grow() {
    if (capacity == 0) {
        data = new T[++capacity];
    }
    if (size < capacity) {
        return;
    }
    capacity *= DEFAULT_GROW;
    T* buf = new T[capacity];
    for (size_t i = 0 ; i < size ; ++i) {
        buf[i] = data[i];
    }
    delete[] data;
    data = buf;
}

template<class T, class H>
void Heap<T, H>::SiftDown(const int &i) {
    size_t leftChild = i * 2 + 1;
    size_t rightChild = i * 2 + 2;
    size_t largest = i;
    if(leftChild < size
       && comparator(data[largest], data[leftChild])) {
        largest = leftChild;
    }
    if(rightChild < size
       && comparator(data[largest], data[rightChild])) {
        largest = rightChild;
    }
    if(largest != i) {
        std::swap(data[largest], data[i]);
        SiftDown(largest);
    }
}

template<class T, class H>
void Heap<T, H>::SiftUp(int i) {
    while(i > 0) {
        int parent = (i - 1) / 2;
        if (comparator(data[i], data[parent])) {
            return;
        }
        std::swap(data[parent], data[i]);
        i = parent;
    }
}

template<class T, class H>
void Heap<T, H>::BuildHeap() {
    for (int i = size / 2 - 1 ; i >= 0 ; --i) {
        SiftDown(i);
    }
}

template<class T, class H>
bool Heap<T, H>::IsEmpty() {
    return size == 0;
}

template<class T, class H>
void Heap<T, H>::Insert(const T &k) {
    if (IsEmpty() || size == capacity) {
        Grow();
    }
    data[size] = k;
    SiftUp(size);
    size++;
}

template<class T, class H>
T Heap<T, H>::ExtractMax() {
    assert(!IsEmpty());
    T result = data[0];
    std::swap(data[0], data[--size]);
    if (!IsEmpty()) {
        SiftDown(0);
    }
    return result;
}

template<class T, class H>
const T& Heap<T, H>::Top() const {
    return data[0];
}

void Answer(std::pair<int, size_t> *arr,
                const size_t &n, const size_t &k) {
    assert(arr != nullptr);
    Heap<std::pair<int, size_t>> heap;
    for (size_t i = 0 ; i < k ; ++i) {
        heap.Insert(arr[i]);
    }
    std::cout << heap.Top().first << " ";
    for (size_t i = k ; i < n ; ++i) {
        heap.Insert(arr[i]);
        while (i - heap.Top().second >= k) {
            heap.ExtractMax();
        }
        std::cout << heap.Top().first << " ";
    }
}

int main() {
    size_t n = 0;
    std::cin >> n;
    std::pair<int, size_t> arr[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr[i].first;
        arr[i].second = i;
    }
    size_t k = 0;
    std::cin >> k;
    Answer(arr, n, k);
    return 0;
}
  // P.S. Если решать задачу на массив unsigned int,
  // а не int как в данной версии, то падает 9 тест с ошибкой WA
