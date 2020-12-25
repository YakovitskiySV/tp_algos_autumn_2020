#include <iostream>
#include <cstdlib>

template<class T>
class IsLess {
public:
    bool operator()(const T& lhs, const T& rhs) {
        return lhs < rhs;
    }
};

template<class T, class H>
size_t Partition(T *arr, const int &left, const int &right,
                 H comparator) {
    if (left == right) {
        return left;
    }
    size_t pivotIdx = left + rand() % (right - left);
    std::swap(arr[pivotIdx], arr[left]);
    T pivot = arr[left];
    int i = right;
    int j = right - 1;
    for (; j > left ; j--) {
        if (!comparator(arr[j], pivot)) {
            std::swap(arr[--i], arr[j]);
        }
    }
    std::swap(arr[--i], arr[left]);
    return i;
}

template <class T, class H>
T KthElem(T *arr, const size_t &k, int left, int right, H comparator) {
    int pos = -1;
    for (;;) {
        pos = Partition(arr, left, right, comparator);
        if (pos < k) {
            left = pos;
        } else if (pos > k) {
            right = pos;
        } else {
            return arr[pos];
        }
    }
}

int main() {
    int n = 0;
    size_t k = 0;
    std::cin >> n >> k;
    int *arr = new int[n];
    for (size_t i = 0; i < n ; ++i) {
        std::cin >> arr[i];
    }
    std::cout << KthElem(arr, k, 0, n, IsLess<int>());
    delete[] arr;
    return 0;
}
