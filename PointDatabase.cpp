#include "PointDatabase.h"
#include <cmath>

// Constructor: Sorts points and builds the 2D Range Tree
PointDatabase::PointDatabase(const std::vector<Point>& points) {
    std::vector<Point> points_copy = points;
    // Initial sort by X-coordinate
    std::sort(points_copy.begin(), points_copy.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });
    root = buildXTree(points_copy);
}

// Destructor: Deletes the root, which triggers recursive deletion
PointDatabase::~PointDatabase() {
    delete root;
}

// Merges two vectors of points, maintaining Y-sort order
std::vector<Point> PointDatabase::mergeByY(const std::vector<Point>& left, const std::vector<Point>& right) {
    std::vector<Point> merged;
    merged.reserve(left.size() + right.size());
    auto it_l = left.begin();
    auto it_r = right.begin();

    while (it_l != left.end() && it_r != right.end()) {
        if (it_l->y < it_r->y) {
            merged.push_back(*it_l++);
        } else {
            merged.push_back(*it_r++);
        }
    }
    merged.insert(merged.end(), it_l, left.end());
    merged.insert(merged.end(), it_r, right.end());
    return merged;
}

// Recursively builds the primary X-tree from a sorted vector of points
PointDatabase::Node* PointDatabase::buildXTree(std::vector<Point>& points_sorted_by_x) {
    if (points_sorted_by_x.empty()) {
        return nullptr;
    }
    int mid = points_sorted_by_x.size() / 2;
    Node* node = new Node(points_sorted_by_x[mid]);

    std::vector<Point> left_points(points_sorted_by_x.begin(), points_sorted_by_x.begin() + mid);
    std::vector<Point> right_points(points_sorted_by_x.begin() + mid + 1, points_sorted_by_x.end());

    node->left = buildXTree(left_points);
    node->right = buildXTree(right_points);
    
    // Create the sorted-by-Y list for the current node's subtree
    std::vector<Point> left_y_sorted = node->left ? node->left->y_sorted_points : std::vector<Point>();
    std::vector<Point> right_y_sorted = node->right ? node->right->y_sorted_points : std::vector<Point>();
    
    node->y_sorted_points = mergeByY(left_y_sorted, right_y_sorted);
    node->y_sorted_points = mergeByY(node->y_sorted_points, {node->point}); // Add self

    // Build the auxiliary Y-tree for this node
    node->aux_tree = buildYTree(node->y_sorted_points);

    return node;
}

// Recursively builds an auxiliary Y-tree from a sorted-by-Y vector
PointDatabase::Node* PointDatabase::buildYTree(const std::vector<Point>& points_sorted_by_y) {
    if (points_sorted_by_y.empty()) {
        return nullptr;
    }
    int mid = points_sorted_by_y.size() / 2;
    Node* node = new Node(points_sorted_by_y[mid]);

    std::vector<Point> left_points(points_sorted_by_y.begin(), points_sorted_by_y.begin() + mid);
    std::vector<Point> right_points(points_sorted_by_y.begin() + mid + 1, points_sorted_by_y.end());

    node->left = buildYTree(left_points);
    node->right = buildYTree(right_points);

    return node;
}

// Public search function
std::vector<Point> PointDatabase::searchNearby(Point q, double d) {
    std::vector<Point> result;
    int x_min = static_cast<int>(ceil(q.x - d));
    int x_max = static_cast<int>(floor(q.x + d));
    int y_min = static_cast<int>(ceil(q.y - d));
    int y_max = static_cast<int>(floor(q.y + d));

    searchXTree(root, x_min, x_max, y_min, y_max, result);
    return result;
}

// Recursive search on the primary X-tree
void PointDatabase::searchXTree(Node* node, int x_min, int x_max, int y_min, int y_max, std::vector<Point>& result) {
    if (!node) return;

    if (x_max < node->point.x) {
        searchXTree(node->left, x_min, x_max, y_min, y_max, result);
    } else if (x_min > node->point.x) {
        searchXTree(node->right, x_min, x_max, y_min, y_max, result);
    } else { // Split node case
        // Check if the current node's point is in the y-range
        if (node->point.y >= y_min && node->point.y <= y_max) {
            result.push_back(node->point);
        }
        // Search left and right children recursively
        searchXTree(node->left, x_min, x_max, y_min, y_max, result);
        searchXTree(node->right, x_min, x_max, y_min, y_max, result);
    }
}

// Adds all points in a subtree to the result vector
void PointDatabase::reportSubtree(Node* node, std::vector<Point>& result) {
    if (!node) return;
    result.push_back(node->point);
    reportSubtree(node->left, result);
    reportSubtree(node->right, result);
}

// 1D range search on an auxiliary Y-tree
void PointDatabase::searchYTree(Node* node, int y_min, int y_max, std::vector<Point>& result) {
    if (!node) return;

    // If node's point is in range, add it and search both children
    if (y_min <= node->point.y && node->point.y <= y_max) {
        result.push_back(node->point);
        searchYTree(node->left, y_min, y_max, result);
        searchYTree(node->right, y_min, y_max, result);
    } else if (y_max < node->point.y) {
        searchYTree(node->left, y_min, y_max, result);
    } else { // y_min > node->point.y
        searchYTree(node->right, y_min, y_max, result);
    }
}