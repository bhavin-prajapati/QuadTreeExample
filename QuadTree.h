#ifndef QUADTREE_H
#define QUADTREE_H

#include "Point.h"
#include <vector>
#include <memory>

class QuadTree {
private:
    static const int CAPACITY = 4;  // Maximum points per node before subdivision
    
    struct QuadNode {
        Rectangle boundary;
        std::vector<QuadPoint> points;
        
        // Four quadrants: NW, NE, SW, SE
        std::unique_ptr<QuadNode> northwest;
        std::unique_ptr<QuadNode> northeast;
        std::unique_ptr<QuadNode> southwest;
        std::unique_ptr<QuadNode> southeast;
        
        bool divided;
        
        QuadNode(const Rectangle& boundary) 
            : boundary(boundary), divided(false) {}
        
        // Check if this node can accept a new point
        bool insert(const QuadPoint& point);
        
        // Subdivide this node into four quadrants
        void subdivide();
        
        // Query points within a range
        void query(const Rectangle& range, std::vector<QuadPoint>& result) const;
        
        // Get all subdivision boundaries for visualization
        void getBoundaries(std::vector<Rectangle>& boundaries) const;
    };
    
    std::unique_ptr<QuadNode> root;
    
public:
    QuadTree(const Rectangle& boundary);
    ~QuadTree() = default;
    
    // Insert a point into the quad tree
    bool insert(const QuadPoint& point);
    
    // Query points within a rectangular range
    std::vector<QuadPoint> query(const Rectangle& range) const;
    
    // Get all points in the quad tree
    std::vector<QuadPoint> getAllPoints() const;
    
    // Get all subdivision boundaries for visualization
    std::vector<Rectangle> getBoundaries() const;
    
    // Clear all points from the quad tree
    void clear();
    
    // Get the root boundary
    Rectangle getBoundary() const;
};

#endif // QUADTREE_H
