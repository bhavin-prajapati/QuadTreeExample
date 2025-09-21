#include "QuadTree.h"
#include <iostream>
#include <cassert>

int main() {
    // Create a QuadTree with a 100x100 boundary
    Rectangle boundary(0, 0, 100, 100);
    QuadTree tree(boundary);
    
    // Test inserting points
    QuadPoint p1(10, 10);
    QuadPoint p2(20, 20);
    QuadPoint p3(80, 80);
    QuadPoint p4(90, 90);
    
    assert(tree.insert(p1));
    assert(tree.insert(p2));
    assert(tree.insert(p3));
    assert(tree.insert(p4));
    
    std::cout << "✓ Point insertion test passed" << std::endl;
    
    // Test query functionality
    Rectangle queryRange(0, 0, 50, 50);
    std::vector<QuadPoint> results = tree.query(queryRange);
    
    // Should find p1 and p2
    assert(results.size() == 2);
    std::cout << "✓ Query test passed - found " << results.size() << " points" << std::endl;
    
    // Test getting all points
    std::vector<QuadPoint> allPoints = tree.getAllPoints();
    assert(allPoints.size() == 4);
    std::cout << "✓ Get all points test passed - found " << allPoints.size() << " points" << std::endl;
    
    // Test boundary subdivision
    std::vector<Rectangle> boundaries = tree.getBoundaries();
    std::cout << "✓ Tree has " << boundaries.size() << " subdivisions" << std::endl;
    
    // Test clear functionality
    tree.clear();
    assert(tree.getAllPoints().size() == 0);
    std::cout << "✓ Clear test passed" << std::endl;
    
    std::cout << "\n🎉 All QuadTree tests passed!" << std::endl;
    std::cout << "The QuadTree implementation is working correctly." << std::endl;
    
    return 0;
}
