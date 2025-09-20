#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <memory>
#include <functional>

// Forward declarations
class QuadTreeNode;
class GraphicsObject;

/**
 * Represents a rectangular boundary in 2D space
 */
struct Bounds {
    float x, y;           // Top-left corner
    float width, height;  // Dimensions
    
    Bounds(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
    
    // Property getters
    float centerX() const { return x + width * 0.5f; }
    float centerY() const { return y + height * 0.5f; }
    float left() const { return x; }
    float right() const { return x + width; }
    float top() const { return y; }
    float bottom() const { return y + height; }
    
    // Spatial operations
    bool containsPoint(float px, float py) const;
    bool intersects(const Bounds& other) const;
    bool contains(const Bounds& other) const;
};

/**
 * Represents a graphics object that can be stored in the quad tree
 */
class GraphicsObject {
public:
    float x, y;           // Position
    float width, height;  // Dimensions
    int objectType;       // User-defined type identifier
    void* userData;       // Pointer to user data (texture, mesh, etc.)
    
    GraphicsObject(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f, 
                   int type = 0, void* data = nullptr);
    
    Bounds getBounds() const;
    float centerX() const { return x + width * 0.5f; }
    float centerY() const { return y + height * 0.5f; }
    
    // Comparison operators for std::vector operations
    bool operator==(const GraphicsObject& other) const;
    bool operator!=(const GraphicsObject& other) const;
};

/**
 * A single node in the quad tree structure
 */
class QuadTreeNode {
public:
    static constexpr int NW = 0; // Northwest
    static constexpr int NE = 1; // Northeast  
    static constexpr int SW = 2; // Southwest
    static constexpr int SE = 3; // Southeast
    
private:
    Bounds bounds_;
    std::vector<GraphicsObject> objects_;
    std::unique_ptr<QuadTreeNode> children_[4];
    
    int maxObjects_;
    int maxDepth_;
    int depth_;
    bool isSubdivided_;
    
public:
    QuadTreeNode(const Bounds& bounds, int maxObjects = 10, int maxDepth = 5, int depth = 0);
    ~QuadTreeNode() = default;
    
    // Core operations
    bool insert(const GraphicsObject& obj);
    bool remove(const GraphicsObject& obj);
    void clear();
    
    // Querying
    void queryRange(const Bounds& range, std::vector<GraphicsObject>& result) const;
    void queryPoint(float x, float y, std::vector<GraphicsObject>& result) const;
    
    // Utility methods
    int getTotalObjects() const;
    const Bounds& getBounds() const { return bounds_; }
    bool isLeaf() const { return !isSubdivided_; }
    int getDepth() const { return depth_; }
    
    // Tree traversal
    void forEachObject(const std::function<void(const GraphicsObject&)>& func) const;
    void forEachNode(const std::function<void(const QuadTreeNode&)>& func) const;
    
private:
    void subdivide();
    int getChildIndex(const GraphicsObject& obj) const;
    bool canFitInChild(const GraphicsObject& obj) const;
};

/**
 * Main QuadTree class for graphics applications
 */
class QuadTree {
private:
    std::unique_ptr<QuadTreeNode> root_;
    Bounds bounds_;
    
public:
    QuadTree(const Bounds& bounds, int maxObjects = 10, int maxDepth = 5);
    ~QuadTree() = default;
    
    // Disable copy constructor and assignment operator for performance
    QuadTree(const QuadTree&) = delete;
    QuadTree& operator=(const QuadTree&) = delete;
    
    // Enable move semantics
    QuadTree(QuadTree&&) = default;
    QuadTree& operator=(QuadTree&&) = default;
    
    // Core operations
    bool insert(const GraphicsObject& obj);
    bool remove(const GraphicsObject& obj);
    void clear();
    
    // Querying operations
    std::vector<GraphicsObject> queryRange(const Bounds& range) const;
    std::vector<GraphicsObject> queryPoint(float x, float y) const;
    std::vector<GraphicsObject> frustumCull(const Bounds& cameraView) const;
    
    // Utility methods
    int getTotalObjects() const;
    const Bounds& getBounds() const { return bounds_; }
    
    // Advanced operations
    void update(const GraphicsObject& oldObj, const GraphicsObject& newObj);
    void rebuild();
    
    // Tree traversal and debugging
    void forEachObject(const std::function<void(const GraphicsObject&)>& func) const;
    void forEachNode(const std::function<void(const QuadTreeNode&)>& func) const;
    
    // Performance metrics
    int getMaxDepth() const;
    float getAverageObjectsPerLeaf() const;
    int getNodeCount() const;
};

#endif // QUADTREE_H