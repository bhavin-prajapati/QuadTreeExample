# QuadTree for Graphics Applications

A high-performance C++ implementation of a QuadTree data structure optimized for graphics applications, game engines, and spatial queries.

## Features

- **Fast Spatial Queries**: Efficient range queries and point lookups for 2D graphics objects
- **Frustum Culling**: Perfect for rendering optimizations - only process visible objects
- **Dynamic Updates**: Support for moving objects with efficient update operations
- **Memory Efficient**: Uses smart pointers and optimized data structures
- **Customizable**: Configurable maximum depth and objects per node
- **Graphics-Focused**: Designed specifically for graphics and game development use cases

## Key Benefits for Graphics

1. **Frustum Culling**: Only render objects visible in the camera view
2. **Collision Detection**: Quickly find objects in a spatial region
3. **Mouse Picking**: Efficient point-in-object queries for UI interaction
4. **Level of Detail**: Different processing based on spatial regions
5. **Performance**: O(log n) average case for insertions and queries

## Quick Start

### Compilation

```bash
# Build and run the example
make run

# Build release version
make

# Build debug version
make debug

# Windows build
make windows
```

### Basic Usage

```cpp
#include "QuadTree.h"

// Create a world space
Bounds worldBounds(0.0f, 0.0f, 1920.0f, 1080.0f);
QuadTree quadTree(worldBounds);

// Add graphics objects
GraphicsObject sprite(100.0f, 100.0f, 64.0f, 64.0f, SPRITE_TYPE);
quadTree.insert(sprite);

// Query for visible objects (frustum culling)
Bounds cameraView(0.0f, 0.0f, 800.0f, 600.0f);
auto visibleObjects = quadTree.frustumCull(cameraView);

// Mouse picking
auto objectsAtPoint = quadTree.queryPoint(mouseX, mouseY);

// Update moving objects
quadTree.update(oldPosition, newPosition);
```

## API Reference

### Core Classes

#### `Bounds`
Represents a rectangular boundary in 2D space.

```cpp
Bounds(float x, float y, float width, float height);
bool containsPoint(float x, float y) const;
bool intersects(const Bounds& other) const;
```

#### `GraphicsObject`
Represents a graphics object that can be stored in the quad tree.

```cpp
GraphicsObject(float x, float y, float width, float height, int type = 0, void* userData = nullptr);
Bounds getBounds() const;
```

#### `QuadTree`
Main quad tree class for spatial partitioning.

```cpp
QuadTree(const Bounds& bounds, int maxObjects = 10, int maxDepth = 5);

// Core operations
bool insert(const GraphicsObject& obj);
bool remove(const GraphicsObject& obj);
void clear();

// Spatial queries
std::vector<GraphicsObject> queryRange(const Bounds& range) const;
std::vector<GraphicsObject> queryPoint(float x, float y) const;
std::vector<GraphicsObject> frustumCull(const Bounds& cameraView) const;

// Utilities
int getTotalObjects() const;
void update(const GraphicsObject& oldObj, const GraphicsObject& newObj);
void rebuild();
```

## Performance Characteristics

- **Insertion**: O(log n) average case
- **Query**: O(log n + k) where k is the number of results
- **Memory**: O(n) where n is the number of objects
- **Tree Depth**: Configurable maximum depth prevents degenerate cases

## Graphics Use Cases

### 1. Frustum Culling
```cpp
// Only render objects visible in camera
Bounds cameraFrustum(cameraX, cameraY, screenWidth, screenHeight);
auto visibleObjects = quadTree.frustumCull(cameraFrustum);

for (const auto& obj : visibleObjects) {
    renderer.draw(obj);
}
```

### 2. Collision Detection
```cpp
// Find all objects in collision area
Bounds collisionArea(playerX - 50, playerY - 50, 100, 100);
auto nearbyObjects = quadTree.queryRange(collisionArea);

for (const auto& obj : nearbyObjects) {
    if (checkCollision(player, obj)) {
        handleCollision(player, obj);
    }
}
```

### 3. Mouse Picking
```cpp
// Find objects under mouse cursor
auto objectsUnderMouse = quadTree.queryPoint(mouseX, mouseY);

if (!objectsUnderMouse.empty()) {
    // Handle click on topmost object
    handleClick(objectsUnderMouse.front());
}
```

### 4. Dynamic Object Updates
```cpp
// Update object position efficiently
GraphicsObject oldState = movingObject;
movingObject.x += velocityX * deltaTime;
movingObject.y += velocityY * deltaTime;

quadTree.update(oldState, movingObject);
```

## Configuration

### Constructor Parameters

- **maxObjects**: Maximum objects per node before subdivision (default: 10)
- **maxDepth**: Maximum tree depth to prevent infinite subdivision (default: 5)

```cpp
// High-performance setup for many small objects
QuadTree quadTree(bounds, 15, 8);

// Memory-conservative setup for fewer objects  
QuadTree quadTree(bounds, 5, 4);
```

### Optimization Tips

1. **Choose appropriate bounds**: Make the world bounds as tight as possible
2. **Tune maxObjects**: Higher values reduce tree depth but increase query time
3. **Monitor tree statistics**: Use `getMaxDepth()` and `getAverageObjectsPerLeaf()`
4. **Batch operations**: Group insertions and updates when possible
5. **Consider rebuilding**: Call `rebuild()` periodically for heavily modified trees

## Example Application

Run the included example to see the quad tree in action:

```bash
make run
```

The example demonstrates:
- Performance benchmarking with 10,000+ objects
- Graphics simulation with sprites, tiles, and particles
- Camera movement and frustum culling
- Mouse picking simulation
- Dynamic object updates

## Performance Results

On a typical modern system, you can expect:
- **10,000 objects**: Insert in ~2-5 milliseconds
- **1,000 range queries**: Complete in ~1-2 milliseconds
- **Average query**: 50-100 objects found in microseconds
- **Memory usage**: ~200-500 KB for 10,000 objects

## Integration with Graphics Libraries

The quad tree works well with popular graphics libraries:

- **OpenGL/Vulkan**: Use for frustum culling and LOD selection
- **SDL/SFML**: Integrate with sprite management and collision detection
- **Game Engines**: Drop-in replacement for basic spatial partitioning
- **Custom Renderers**: Optimize draw calls and state changes

## Thread Safety

The current implementation is **not thread-safe**. For multi-threaded applications:
- Use separate quad trees per thread
- Implement external synchronization
- Consider read-only queries with occasional rebuilds

## Contributing

Feel free to extend the implementation for your specific needs:
- Add 3D support (Octree)
- Implement thread-safety
- Add serialization support
- Optimize for specific object types

## License

This implementation is provided as example code for educational and commercial use.

---

**Happy coding!** 🎮✨