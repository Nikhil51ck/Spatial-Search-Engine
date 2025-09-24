High-Performance 2D Spatial Search Engine (ℓ∞-Distance)
A specialized spatial database in C++ engineered for highly efficient proximity searches on a 2D point set. The system is optimized for orthogonal range queries using the Chebyshev (ℓ∞) distance, which is crucial in applications like logistics, microchip design, and game development where movement is constrained to a grid.

Project Overview
This project implements a high-performance 2D spatial database using a 2D Range Tree data structure. It's designed to rapidly find all data points within a specified square-shaped region around a query point, a common requirement in many computational fields.

Key Applications
Geospatial Systems: Quickly finding "nearby" points of interest (e.g., restaurants, stores) on a grid-like map.

Logistics & Planning: Optimizing delivery routes where costs are based on the maximum axis-aligned displacement.

Game Development: Efficient collision detection or selecting objects within a character's field of view.

VLSI Design: Locating components within a specific bounding box for layout verification and analysis.

Features & Performance
⚡️ Efficient Construction: The PointDatabase is built in O(nlogn) time for n data points, allowing for quick setup even with large datasets.

🚀 Rapid Proximity Search: Queries for points within an ℓ∞-distance d of a query point q are resolved in O(log 
2
 n+m) time, where m is the number of points returned.

🧊 ℓ∞-Distance Core: Specifically designed to handle the Chebyshev distance, which defines a square-shaped query region, as opposed to Euclidean (circular) or Manhattan (diamond-shaped) distances.

🧩 Integer Coordinate Support: Built for integer coordinates to avoid floating-point precision issues, ideal for grid-based systems.

How ℓ∞-Distance Search Works
The ℓ∞-distance (Chebyshev distance) between two points p_1=(x_1,y_1) and p_2=(x_2,y_2) is defined as:

L 
∞
​
 =max(∣x 
1
​
 −x 
2
​
 ∣,∣y 
1
​
 −y 
2
​
 ∣)
When searching for points within a distance d from a query point q=(q_x,q_y), we are looking for all points p=(x,y) that satisfy:

q_x−d≤x≤q_x+d
AND
q_y−d≤y≤q_y+d

This defines an axis-aligned square centered at q with a side length of 2d.

Data Structure: 2D Range Tree
The performance of this database comes from its underlying 2D Range Tree.

Primary Tree (X-coordinates): The main structure is a balanced binary search tree built on the x-coordinates of all data points.

Associated Trees (Y-coordinates): Each node in the primary X-tree contains an associated balanced binary search tree (the "auxiliary tree"). This Y-tree stores all the points from the primary node's subtree, but sorted by their y-coordinates.

Querying: A search for a square region [x_min, x_max] x [y_min, y_max] efficiently traverses the primary X-tree to find points within the x-range. For qualifying subtrees, it then performs a 1D range query on their associated Y-trees to find points within the y-range, drastically pruning the search space.

Getting Started
Prerequisites
A C++ compiler that supports C++17 or later (e.g., g++, Clang).

make (optional, for convenience).

Compilation & Execution
Clone the repository:

Bash

git clone https://github.com/your-username/Spatial-Search-Engine.git
cd Spatial-Search-Engine
Compile the code:
You can compile the main.cpp file directly using your C++ compiler.

Bash

g++ -std=c++17 -o search_engine main.cpp
Run the executable:

Bash

./search_engine
Example Usage
The main.cpp file contains a simple demonstration of how to use the PointDatabase.

C++


```
#include <iostream>
#include <vector>
#include "PointDatabase.h" // Assuming the class is in this header

int main() {
    // 1. Define your list of points
    std::vector<Point> points = {
        {38, 26}, {43, 24}, {5, 25}, {30, 2}, {29, 7}, {37, 16},
        {51, 15}, {40, 23}, {23, 20}, {8, 49}, {34, 45}, {42, 12},
        {32, 39}, {17, 19}, {12, 4}
    };

    // 2. Create the database object
    PointDatabase db(points);

    // 3. Perform a search query for points near (20, 40) with distance 37.9
    std::vector<Point> nearby_points = db.searchNearby({20, 40}, 37.9);

    // 4. Print the results
    std::cout << "Points near (20, 40) with d=37.9:" << std::endl;
    for (const auto& p : nearby_points) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }

    return 0;
}
``` 