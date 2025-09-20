#include "QuadTree.h"
#include <iostream>
#include <random>
#include <chrono>
#include <vector>

// Simulation of different object types
enum ObjectType {
    SPRITE = 0,
    TILE = 1,
    PARTICLE = 2,
    UI_ELEMENT = 3
};

// Helper function to generate random float
float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

// Performance benchmark function
void benchmarkQuadTree() {
    std::cout << "=== QuadTree Performance Benchmark ===" << std::endl;
    
    // Create a large world space (e.g., a 2D game world)
    Bounds worldBounds(0.0f, 0.0f, 10000.0f, 10000.0f);
    QuadTree quadTree(worldBounds, 10, 6); // max 10 objects per node, max depth 6
    
    // Generate random objects
    const int numObjects = 10000;
    std::vector<GraphicsObject> objects;
    objects.reserve(numObjects);
    
    std::cout << "Generating " << numObjects << " random objects..." << std::endl;
    
    for (int i = 0; i < numObjects; ++i) {
        float x = randomFloat(0.0f, 9900.0f);
        float y = randomFloat(0.0f, 9900.0f);
        float size = randomFloat(5.0f, 50.0f);
        int type = static_cast<int>(randomFloat(0, 4));
        
        GraphicsObject obj(x, y, size, size, type);
        objects.push_back(obj);
    }
    
    // Benchmark insertion
    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& obj : objects) {
        quadTree.insert(obj);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "Insertion time: " << insertTime << " microseconds" << std::endl;
    std::cout << "Objects per microsecond: " << static_cast<double>(numObjects) / insertTime << std::endl;
    
    // Print tree statistics
    std::cout << "\\nTree Statistics:" << std::endl;
    std::cout << "Total objects: " << quadTree.getTotalObjects() << std::endl;
    std::cout << "Max depth: " << quadTree.getMaxDepth() << std::endl;
    std::cout << "Node count: " << quadTree.getNodeCount() << std::endl;
    std::cout << "Average objects per leaf: " << quadTree.getAverageObjectsPerLeaf() << std::endl;
    
    // Benchmark range queries (frustum culling simulation)
    const int numQueries = 1000;
    std::vector<Bounds> queryRanges;
    queryRanges.reserve(numQueries);
    
    // Generate random camera views
    for (int i = 0; i < numQueries; ++i) {
        float x = randomFloat(0.0f, 8000.0f);
        float y = randomFloat(0.0f, 8000.0f);
        queryRanges.emplace_back(x, y, 800.0f, 600.0f); // Typical screen resolution area
    }
    
    start = std::chrono::high_resolution_clock::now();
    
    int totalFound = 0;
    for (const auto& range : queryRanges) {
        auto result = quadTree.queryRange(range);
        totalFound += static_cast<int>(result.size());
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "\\nQuery Performance:" << std::endl;
    std::cout << "Query time for " << numQueries << " range queries: " << queryTime << " microseconds" << std::endl;
    std::cout << "Average objects found per query: " << static_cast<double>(totalFound) / numQueries << std::endl;
    std::cout << "Queries per second: " << static_cast<double>(numQueries) * 1000000 / queryTime << std::endl;
}

// Demonstration of graphics-specific usage
void demonstrateGraphicsUsage() {
    std::cout << "\\n=== Graphics Usage Demonstration ===" << std::endl;
    
    // Create a game world (1920x1080 screen with extended boundaries)
    Bounds gameWorld(0.0f, 0.0f, 3840.0f, 2160.0f);
    QuadTree sceneGraph(gameWorld);
    
    // Add various graphics objects
    std::cout << "Adding graphics objects to scene..." << std::endl;
    
    // Background tiles
    for (float x = 0; x < 3840; x += 64) {
        for (float y = 0; y < 2160; y += 64) {
            GraphicsObject tile(x, y, 64.0f, 64.0f, TILE);
            sceneGraph.insert(tile);
        }
    }
    
    // Sprites (game entities)
    std::vector<GraphicsObject> sprites;
    for (int i = 0; i < 500; ++i) {
        float x = randomFloat(100.0f, 3740.0f);
        float y = randomFloat(100.0f, 2060.0f);
        float size = randomFloat(16.0f, 128.0f);
        
        GraphicsObject sprite(x, y, size, size, SPRITE, reinterpret_cast<void*>(i + 1));
        sprites.push_back(sprite);
        sceneGraph.insert(sprite);
    }
    
    // Particles (smaller, numerous objects)
    for (int i = 0; i < 2000; ++i) {
        float x = randomFloat(0.0f, 3840.0f);
        float y = randomFloat(0.0f, 2160.0f);
        
        GraphicsObject particle(x, y, 2.0f, 2.0f, PARTICLE);
        sceneGraph.insert(particle);
    }
    
    std::cout << "Scene setup complete!" << std::endl;
    std::cout << "Total objects in scene: " << sceneGraph.getTotalObjects() << std::endl;
    
    // Simulate camera movement and frustum culling
    std::cout << "\\nSimulating camera movement and frustum culling..." << std::endl;
    
    float cameraWidth = 1920.0f;
    float cameraHeight = 1080.0f;
    
    for (int frame = 0; frame < 10; ++frame) {
        // Simulate camera moving across the world
        float cameraX = (frame * 300.0f) % (3840.0f - cameraWidth);
        float cameraY = (frame * 200.0f) % (2160.0f - cameraHeight);
        
        Bounds cameraView(cameraX, cameraY, cameraWidth, cameraHeight);
        
        // Perform frustum culling
        auto visibleObjects = sceneGraph.frustumCull(cameraView);
        
        // Count objects by type
        int tileCount = 0, spriteCount = 0, particleCount = 0;
        
        for (const auto& obj : visibleObjects) {
            switch (obj.objectType) {
                case TILE: tileCount++; break;
                case SPRITE: spriteCount++; break;
                case PARTICLE: particleCount++; break;
            }
        }
        
        std::cout << "Frame " << frame << ": Camera at (" << cameraX << ", " << cameraY 
                  << ") - Visible: " << visibleObjects.size() << " objects "
                  << "(Tiles: " << tileCount << ", Sprites: " << spriteCount 
                  << ", Particles: " << particleCount << ")" << std::endl;
    }
    
    // Demonstrate point queries (mouse picking)
    std::cout << "\\nDemonstrating mouse picking..." << std::endl;
    
    for (int i = 0; i < 5; ++i) {
        float mouseX = randomFloat(0.0f, 3840.0f);
        float mouseY = randomFloat(0.0f, 2160.0f);
        
        auto objectsAtPoint = sceneGraph.queryPoint(mouseX, mouseY);
        
        std::cout << "Mouse at (" << mouseX << ", " << mouseY << "): Found " 
                  << objectsAtPoint.size() << " objects" << std::endl;
        
        // Show details of first few objects
        for (size_t j = 0; j < std::min(size_t(3), objectsAtPoint.size()); ++j) {
            const auto& obj = objectsAtPoint[j];
            std::string typeName;
            switch (obj.objectType) {
                case TILE: typeName = "Tile"; break;
                case SPRITE: typeName = "Sprite"; break;
                case PARTICLE: typeName = "Particle"; break;
                default: typeName = "Unknown"; break;
            }
            
            std::cout << "  - " << typeName << " at (" << obj.x << ", " << obj.y 
                      << ") size: " << obj.width << "x" << obj.height << std::endl;
        }
    }
    
    // Demonstrate dynamic object updates
    std::cout << "\\nDemonstrating dynamic updates..." << std::endl;
    
    if (!sprites.empty()) {
        GraphicsObject& sprite = sprites[0];
        float oldX = sprite.x, oldY = sprite.y;
        
        // Move sprite to new location
        GraphicsObject oldSprite = sprite;
        sprite.x = randomFloat(100.0f, 3740.0f);
        sprite.y = randomFloat(100.0f, 2060.0f);
        
        // Update in quad tree
        sceneGraph.update(oldSprite, sprite);
        
        std::cout << "Moved sprite from (" << oldX << ", " << oldY 
                  << ") to (" << sprite.x << ", " << sprite.y << ")" << std::endl;
        
        // Verify the update worked
        auto foundObjects = sceneGraph.queryPoint(sprite.x + sprite.width/2, sprite.y + sprite.height/2);
        bool found = false;
        for (const auto& obj : foundObjects) {
            if (obj.userData == sprite.userData && obj.objectType == SPRITE) {
                found = true;
                break;
            }
        }
        
        std::cout << "Sprite successfully " << (found ? "found" : "not found") << " at new location" << std::endl;
    }
}

int main() {
    std::cout << "QuadTree for Graphics - Example Application" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    try {
        // Run graphics usage demonstration
        demonstrateGraphicsUsage();
        
        // Run performance benchmark
        benchmarkQuadTree();
        
        std::cout << "\\n=== Key Benefits of QuadTree for Graphics ===" << std::endl;
        std::cout << "1. Frustum Culling: Only render visible objects" << std::endl;
        std::cout << "2. Collision Detection: Quickly find nearby objects" << std::endl;
        std::cout << "3. Mouse Picking: Efficient point-in-object queries" << std::endl;
        std::cout << "4. Level of Detail: Different rendering for different zoom levels" << std::endl;
        std::cout << "5. Spatial Partitioning: Organize objects by location" << std::endl;
        
        std::cout << "\\nExample completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}