#include "QuadTree.h"
#include <algorithm>

// QuadTree constructor
QuadTree::QuadTree(const Rectangle& boundary) {
    root = std::make_unique<QuadNode>(boundary);
}

// QuadTree public methods
bool QuadTree::insert(const QuadPoint& point) {
    return root->insert(point);
}

std::vector<QuadPoint> QuadTree::query(const Rectangle& range) const {
    std::vector<QuadPoint> result;
    root->query(range, result);
    return result;
}

std::vector<QuadPoint> QuadTree::getAllPoints() const {
    return query(root->boundary);
}

std::vector<Rectangle> QuadTree::getBoundaries() const {
    std::vector<Rectangle> boundaries;
    root->getBoundaries(boundaries);
    return boundaries;
}

void QuadTree::clear() {
    root = std::make_unique<QuadNode>(root->boundary);
}

Rectangle QuadTree::getBoundary() const {
    return root->boundary;
}

// QuadNode implementation
bool QuadTree::QuadNode::insert(const QuadPoint& point) {
    // Check if point is within this node's boundary
    if (!boundary.contains(point)) {
        return false;
    }
    
    // If we haven't reached capacity and haven't divided, add point here
    if (points.size() < CAPACITY && !divided) {
        points.push_back(point);
        return true;
    }
    
    // If we haven't subdivided yet, do it now
    if (!divided) {
        subdivide();
        
        // Move existing points to appropriate quadrants
        std::vector<QuadPoint> pointsToMove = points;
        points.clear();
        
        for (const QuadPoint& p : pointsToMove) {
            if (!northwest->insert(p)) {
                if (!northeast->insert(p)) {
                    if (!southwest->insert(p)) {
                        southeast->insert(p);
                    }
                }
            }
        }
    }
    
    // Try to insert into appropriate quadrant
    if (northwest->insert(point)) return true;
    if (northeast->insert(point)) return true;
    if (southwest->insert(point)) return true;
    if (southeast->insert(point)) return true;
    
    return false;
}

void QuadTree::QuadNode::subdivide() {
    float x = boundary.x;
    float y = boundary.y;
    float w = boundary.width / 2.0f;
    float h = boundary.height / 2.0f;
    
    northwest = std::make_unique<QuadNode>(Rectangle(x, y, w, h));
    northeast = std::make_unique<QuadNode>(Rectangle(x + w, y, w, h));
    southwest = std::make_unique<QuadNode>(Rectangle(x, y + h, w, h));
    southeast = std::make_unique<QuadNode>(Rectangle(x + w, y + h, w, h));
    
    divided = true;
}

void QuadTree::QuadNode::query(const Rectangle& range, std::vector<QuadPoint>& result) const {
    // Check if range intersects with this node's boundary
    if (!boundary.intersects(range)) {
        return;
    }
    
    // Check points in this node
    for (const QuadPoint& point : points) {
        if (range.contains(point)) {
            result.push_back(point);
        }
    }
    
    // Recursively check child nodes if subdivided
    if (divided) {
        northwest->query(range, result);
        northeast->query(range, result);
        southwest->query(range, result);
        southeast->query(range, result);
    }
}

void QuadTree::QuadNode::getBoundaries(std::vector<Rectangle>& boundaries) const {
    boundaries.push_back(boundary);
    
    if (divided) {
        northwest->getBoundaries(boundaries);
        northeast->getBoundaries(boundaries);
        southwest->getBoundaries(boundaries);
        southeast->getBoundaries(boundaries);
    }
}
