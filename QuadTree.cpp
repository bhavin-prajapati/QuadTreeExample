#include "QuadTree.h"
#include <algorithm>
#include <cmath>

// =============================================================================
// Bounds Implementation
// =============================================================================

Bounds::Bounds(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {}

bool Bounds::containsPoint(float px, float py) const {
    return px >= left() && px <= right() && py >= top() && py <= bottom();
}

bool Bounds::intersects(const Bounds& other) const {
    return !(right() < other.left() || 
             left() > other.right() || 
             bottom() < other.top() || 
             top() > other.bottom());
}

bool Bounds::contains(const Bounds& other) const {
    return left() <= other.left() && 
           right() >= other.right() && 
           top() <= other.top() && 
           bottom() >= other.bottom();
}

// =============================================================================
// GraphicsObject Implementation
// =============================================================================

GraphicsObject::GraphicsObject(float x, float y, float width, float height, int type, void* data)
    : x(x), y(y), width(width), height(height), objectType(type), userData(data) {}

Bounds GraphicsObject::getBounds() const {
    return Bounds(x, y, width, height);
}

bool GraphicsObject::operator==(const GraphicsObject& other) const {
    return x == other.x && y == other.y && 
           width == other.width && height == other.height &&
           objectType == other.objectType && userData == other.userData;
}

bool GraphicsObject::operator!=(const GraphicsObject& other) const {
    return !(*this == other);
}

// =============================================================================
// QuadTreeNode Implementation
// =============================================================================

QuadTreeNode::QuadTreeNode(const Bounds& bounds, int maxObjects, int maxDepth, int depth)
    : bounds_(bounds), maxObjects_(maxObjects), maxDepth_(maxDepth), 
      depth_(depth), isSubdivided_(false) {
    
    // Reserve space for objects to reduce allocations
    objects_.reserve(maxObjects);
    
    // Initialize children to nullptr
    for (int i = 0; i < 4; ++i) {
        children_[i] = nullptr;
    }
}

void QuadTreeNode::subdivide() {
    if (isSubdivided_ || depth_ >= maxDepth_) {
        return;
    }
    
    float halfWidth = bounds_.width * 0.5f;
    float halfHeight = bounds_.height * 0.5f;
    
    // Create four child nodes
    children_[NW] = std::make_unique<QuadTreeNode>(
        Bounds(bounds_.x, bounds_.y, halfWidth, halfHeight),
        maxObjects_, maxDepth_, depth_ + 1
    );
    
    children_[NE] = std::make_unique<QuadTreeNode>(
        Bounds(bounds_.x + halfWidth, bounds_.y, halfWidth, halfHeight),
        maxObjects_, maxDepth_, depth_ + 1
    );
    
    children_[SW] = std::make_unique<QuadTreeNode>(
        Bounds(bounds_.x, bounds_.y + halfHeight, halfWidth, halfHeight),
        maxObjects_, maxDepth_, depth_ + 1
    );
    
    children_[SE] = std::make_unique<QuadTreeNode>(
        Bounds(bounds_.x + halfWidth, bounds_.y + halfHeight, halfWidth, halfHeight),
        maxObjects_, maxDepth_, depth_ + 1
    );
    
    isSubdivided_ = true;
    
    // Redistribute existing objects to children
    auto objectsToRedistribute = std::move(objects_);
    objects_.clear();
    
    for (const auto& obj : objectsToRedistribute) {
        insert(obj);
    }
}

int QuadTreeNode::getChildIndex(const GraphicsObject& obj) const {
    float centerX = bounds_.centerX();
    float centerY = bounds_.centerY();
    
    Bounds objBounds = obj.getBounds();
    
    bool topHalf = objBounds.bottom() <= centerY;
    bool bottomHalf = objBounds.top() >= centerY;
    bool leftHalf = objBounds.right() <= centerX;
    bool rightHalf = objBounds.left() >= centerX;
    
    if (topHalf && leftHalf) return NW;
    if (topHalf && rightHalf) return NE;
    if (bottomHalf && leftHalf) return SW;
    if (bottomHalf && rightHalf) return SE;
    
    // Object spans multiple quadrants
    return -1;
}

bool QuadTreeNode::canFitInChild(const GraphicsObject& obj) const {
    return getChildIndex(obj) != -1;
}

bool QuadTreeNode::insert(const GraphicsObject& obj) {
    // Check if object intersects with this node's bounds
    if (!bounds_.intersects(obj.getBounds())) {
        return false;
    }
    
    // If we have children, try to insert into appropriate child
    if (isSubdivided_) {
        int childIndex = getChildIndex(obj);
        if (childIndex != -1 && children_[childIndex]) {
            return children_[childIndex]->insert(obj);
        }
    }
    
    // Add to this node
    objects_.push_back(obj);
    
    // Check if we need to subdivide
    if (static_cast<int>(objects_.size()) > maxObjects_ && 
        depth_ < maxDepth_ && !isSubdivided_) {
        subdivide();
    }
    
    return true;
}

bool QuadTreeNode::remove(const GraphicsObject& obj) {
    // Try to remove from this node
    auto it = std::find(objects_.begin(), objects_.end(), obj);
    if (it != objects_.end()) {
        objects_.erase(it);
        return true;
    }
    
    // Try to remove from children
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            if (children_[i] && children_[i]->remove(obj)) {
                return true;
            }
        }
    }
    
    return false;
}

void QuadTreeNode::clear() {
    objects_.clear();
    
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            children_[i].reset();
        }
        isSubdivided_ = false;
    }
}

void QuadTreeNode::queryRange(const Bounds& range, std::vector<GraphicsObject>& result) const {
    // Check if query range intersects with this node's bounds
    if (!bounds_.intersects(range)) {
        return;
    }
    
    // Check objects in this node
    for (const auto& obj : objects_) {
        if (range.intersects(obj.getBounds())) {
            result.push_back(obj);
        }
    }
    
    // Check children if subdivided
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            if (children_[i]) {
                children_[i]->queryRange(range, result);
            }
        }
    }
}

void QuadTreeNode::queryPoint(float x, float y, std::vector<GraphicsObject>& result) const {
    // Check if point is within this node's bounds
    if (!bounds_.containsPoint(x, y)) {
        return;
    }
    
    // Check objects in this node
    for (const auto& obj : objects_) {
        if (obj.getBounds().containsPoint(x, y)) {
            result.push_back(obj);
        }
    }
    
    // Check children if subdivided
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            if (children_[i]) {
                children_[i]->queryPoint(x, y, result);
            }
        }
    }
}

int QuadTreeNode::getTotalObjects() const {
    int total = static_cast<int>(objects_.size());
    
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            if (children_[i]) {
                total += children_[i]->getTotalObjects();
            }
        }
    }
    
    return total;
}

void QuadTreeNode::forEachObject(const std::function<void(const GraphicsObject&)>& func) const {
    for (const auto& obj : objects_) {
        func(obj);
    }
    
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            if (children_[i]) {
                children_[i]->forEachObject(func);
            }
        }
    }
}

void QuadTreeNode::forEachNode(const std::function<void(const QuadTreeNode&)>& func) const {
    func(*this);
    
    if (isSubdivided_) {
        for (int i = 0; i < 4; ++i) {
            if (children_[i]) {
                children_[i]->forEachNode(func);
            }
        }
    }
}

// =============================================================================
// QuadTree Implementation
// =============================================================================

QuadTree::QuadTree(const Bounds& bounds, int maxObjects, int maxDepth)
    : bounds_(bounds) {
    root_ = std::make_unique<QuadTreeNode>(bounds, maxObjects, maxDepth);
}

bool QuadTree::insert(const GraphicsObject& obj) {
    return root_->insert(obj);
}

bool QuadTree::remove(const GraphicsObject& obj) {
    return root_->remove(obj);
}

void QuadTree::clear() {
    root_->clear();
}

std::vector<GraphicsObject> QuadTree::queryRange(const Bounds& range) const {
    std::vector<GraphicsObject> result;
    result.reserve(100); // Reserve space to reduce allocations
    root_->queryRange(range, result);
    return result;
}

std::vector<GraphicsObject> QuadTree::queryPoint(float x, float y) const {
    std::vector<GraphicsObject> result;
    root_->queryPoint(x, y, result);
    return result;
}

std::vector<GraphicsObject> QuadTree::frustumCull(const Bounds& cameraView) const {
    return queryRange(cameraView);
}

int QuadTree::getTotalObjects() const {
    return root_->getTotalObjects();
}

void QuadTree::update(const GraphicsObject& oldObj, const GraphicsObject& newObj) {
    remove(oldObj);
    insert(newObj);
}

void QuadTree::rebuild() {
    std::vector<GraphicsObject> allObjects;
    allObjects.reserve(getTotalObjects());
    
    root_->forEachObject([&allObjects](const GraphicsObject& obj) {
        allObjects.push_back(obj);
    });
    
    clear();
    
    for (const auto& obj : allObjects) {
        insert(obj);
    }
}

void QuadTree::forEachObject(const std::function<void(const GraphicsObject&)>& func) const {
    root_->forEachObject(func);
}

void QuadTree::forEachNode(const std::function<void(const QuadTreeNode&)>& func) const {
    root_->forEachNode(func);
}

int QuadTree::getMaxDepth() const {
    int maxDepth = 0;
    
    forEachNode([&maxDepth](const QuadTreeNode& node) {
        maxDepth = std::max(maxDepth, node.getDepth());
    });
    
    return maxDepth;
}

float QuadTree::getAverageObjectsPerLeaf() const {
    int leafCount = 0;
    int totalLeafObjects = 0;
    
    forEachNode([&leafCount, &totalLeafObjects](const QuadTreeNode& node) {
        if (node.isLeaf()) {
            leafCount++;
            totalLeafObjects += static_cast<int>(node.getTotalObjects());
        }
    });
    
    return leafCount > 0 ? static_cast<float>(totalLeafObjects) / leafCount : 0.0f;
}

int QuadTree::getNodeCount() const {
    int nodeCount = 0;
    
    forEachNode([&nodeCount](const QuadTreeNode&) {
        nodeCount++;
    });
    
    return nodeCount;
}