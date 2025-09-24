#ifndef POINTDATABASE_H
#define POINTDATABASE_H

#include <vector>
#include <algorithm>
#include <memory>

// A simple struct to represent a 2D point
struct Point {
    int x, y;
};

class PointDatabase {
private:
    // Internal Node structure for the 2D Range Tree
    struct Node {
        Point point;          // The point stored at this node
        Node* left = nullptr;
        Node* right = nullptr;
        Node* aux_tree = nullptr; // Pointer to the root of the associated Y-tree
        std::vector<Point> y_sorted_points; // Points in this subtree, sorted by Y

        // Constructor
        Node(Point p) : point(p) {}

        // Destructor to recursively delete nodes and prevent memory leaks
        ~Node() {
            delete left;
            delete right;
            delete aux_tree;
        }
    };

    Node* root = nullptr;

    // Helper function to build the primary tree sorted by X-coordinates
    Node* buildXTree(std::vector<Point>& points_sorted_by_x);

    // Helper function to build the auxiliary trees sorted by Y-coordinates
    Node* buildYTree(const std::vector<Point>& points_sorted_by_y);

    // Helper to merge two sorted-by-Y lists of points
    std::vector<Point> mergeByY(const std::vector<Point>& left, const std::vector<Point>& right);

    // Recursive search functions
    void searchXTree(Node* node, int x_min, int x_max, int y_min, int y_max, std::vector<Point>& result);
    void searchYTree(Node* node, int y_min, int y_max, std::vector<Point>& result);
    void reportSubtree(Node* node, std::vector<Point>& result);

public:
    // Constructor takes a list of points
    PointDatabase(const std::vector<Point>& points);

    // Destructor
    ~PointDatabase();

    // Public search method
    std::vector<Point> searchNearby(Point q, double d);
};

#endif // POINTDATABASE_H