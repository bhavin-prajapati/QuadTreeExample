#ifndef POINT_H
#define POINT_H

// Simple 2D point structure
struct QuadPoint {
    float x, y;
    
    QuadPoint() : x(0), y(0) {}
    QuadPoint(float x, float y) : x(x), y(y) {}
    
    bool operator==(const QuadPoint& other) const {
        return x == other.x && y == other.y;
    }
};

// Rectangle structure for quad tree bounds
struct Rectangle {
    float x, y;      // top-left corner
    float width, height;
    
    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(float x, float y, float width, float height) 
        : x(x), y(y), width(width), height(height) {}
    
    // Check if point is inside rectangle
    bool contains(const QuadPoint& point) const {
        return (point.x >= x && point.x < x + width &&
                point.y >= y && point.y < y + height);
    }
    
    // Check if this rectangle intersects with another
    bool intersects(const Rectangle& other) const {
        return !(other.x >= x + width || 
                 other.x + other.width <= x ||
                 other.y >= y + height || 
                 other.y + other.height <= y);
    }
    
    // Get center point
    QuadPoint center() const {
        return QuadPoint(x + width / 2.0f, y + height / 2.0f);
    }
};

#endif // POINT_H
