#include <cmath>
#include <iostream>

typedef struct point {
    int x;
    int y;
} point;

float calculate_square(point *const edges, const size_t &edge_number);

int main() {
    size_t edge_number = 0;
    std::cin >> edge_number;
    point edges[edge_number];
    for (size_t i = 0; i < edge_number; ++i) {
        std::cin >> edges[i].x;
        std::cin >> edges[i].y;
    }
    float result = calculate_square(edges, edge_number);
    std::cout << result << std::endl;
    return 0;
}

float calculate_square(point *const edges, const size_t &edge_number) {
    float square = 0;
    for (size_t i = 0; i < edge_number - 1; ++i) {
        square += static_cast<float>((edges[i].x + edges[i + 1].x) *
                                     (edges[i + 1].y - edges[i].y));
    }
    square += static_cast<float>((edges[0].x + edges[edge_number - 1].x) *
                                 (edges[0].y - edges[edge_number - 1].y));
    return fabs(square / 2);
}
