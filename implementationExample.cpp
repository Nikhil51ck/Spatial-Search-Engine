#include <iostream>
#include <vector>
#include "PointDatabase.h"

// Helper function to print a vector of points
void printPoints(const std::vector<Point>& points) {
    if (points.empty()) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[";
    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << "(" << points[i].x << ", " << points[i].y << ")";
        if (i < points.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::vector<Point> points = {
        {38, 26}, {43, 24}, {5, 25}, {30, 2}, {29, 7}, {37, 16},
        {51, 15}, {40, 23}, {23, 20}, {8, 49}, {34, 45}, {42, 12},
        {32, 39}, {17, 19}, {12, 4}
    };

    PointDatabase db(points);

    std::cout << "Query: (20, 40), d=37.9" << std::endl;
    std::vector<Point> results1 = db.searchNearby({20, 40}, 37.9);
    printPoints(results1); // Expected: A list of many points from the initial set

    std::cout << "\nQuery: (5, 5), d=1" << std::endl;
    std::vector<Point> results2 = db.searchNearby({5, 5}, 1);
    printPoints(results2); // Expected: []

    std::cout << "\nQuery: (4, 8), d=2" << std::endl;
    std::vector<Point> results3 = db.searchNearby({4, 8}, 2);
    printPoints(results3); // Expected: (5, 25) -- note: your original test case seems off based on the data

    std::cout << "\nQuery: (30, 15), d=5" << std::endl;
    std::vector<Point> results4 = db.searchNearby({30, 15}, 5);
    printPoints(results4); // Expected: (29, 7), (32, 39), (34, 45) etc. -- points within x:[25,35] and y:[10,20]

    return 0;
}