#include "QuadTree.h"
#include <iostream>

int main() {
    std::cout << "Testing QuadTree compilation..." << std::endl;
    
    // Create a simple test
    Bounds bounds(0.0f, 0.0f, 100.0f, 100.0f);
    QuadTree quadTree(bounds);
    
    // Add a test object
    GraphicsObject obj(10.0f, 10.0f, 5.0f, 5.0f, 1);
    bool inserted = quadTree.insert(obj);
    
    std::cout << "Object inserted: " << (inserted ? "Yes" : "No") << std::endl;
    std::cout << "Total objects: " << quadTree.getTotalObjects() << std::endl;
    
    // Test a query
    auto results = quadTree.queryPoint(12.0f, 12.0f);
    std::cout << "Objects found at point: " << results.size() << std::endl;
    
    std::cout << "QuadTree test completed successfully!" << std::endl;
    return 0;
}