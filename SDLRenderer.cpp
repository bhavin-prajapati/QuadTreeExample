#include "SDLRenderer.h"
#include <iostream>
#include <random>
#include <cmath>

SDLRenderer::SDLRenderer(int width, int height)
    : window(nullptr), renderer(nullptr), quadTree(nullptr),
      windowWidth(width), windowHeight(height), running(false), isDragging(false),
      showQuery(false), queryStartX(0), queryStartY(0),
      backgroundColor(20, 20, 30),      // Dark blue background
      boundaryColor(255, 255, 255),     // White boundaries
      pointColor(100, 255, 100),        // Light green points
      queryColor(255, 100, 100),        // Red query rectangle
      queryResultColor(255, 255, 100)   // Yellow query results
{
    // Initialize query range
    queryRange = Rectangle(0, 0, 0, 0);
}

SDLRenderer::~SDLRenderer() {
    cleanup();
}

bool SDLRenderer::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow("QuadTree Visualization - SDL",
                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             windowWidth, windowHeight,
                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize QuadTree with window bounds
    Rectangle boundary(0, 0, windowWidth, windowHeight);
    quadTree = new QuadTree(boundary);
    
    running = true;
    
    std::cout << "SDL QuadTree Visualization initialized!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Left Click: Add point" << std::endl;
    std::cout << "  Left Drag: Create query rectangle" << std::endl;
    std::cout << "  Right Click: Clear all points" << std::endl;
    std::cout << "  Space: Add 50 random points" << std::endl;
    std::cout << "  R: Add 200 random points" << std::endl;
    std::cout << "  C: Clear points" << std::endl;
    std::cout << "  ESC: Quit" << std::endl;
    
    return true;
}

void SDLRenderer::cleanup() {
    if (quadTree) {
        delete quadTree;
        quadTree = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}

void SDLRenderer::setQuadTree(QuadTree* tree) {
    if (quadTree) {
        delete quadTree;
    }
    quadTree = tree;
}

bool SDLRenderer::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                handleMouseDown(event);
                break;
                
            case SDL_MOUSEBUTTONUP:
                handleMouseUp(event);
                break;
                
            case SDL_MOUSEMOTION:
                handleMouseMotion(event);
                break;
                
            case SDL_KEYDOWN:
                handleKeyDown(event);
                break;
                
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    
                    // Recreate QuadTree with new bounds
                    if (quadTree) {
                        std::vector<QuadPoint> points = quadTree->getAllPoints();
                        delete quadTree;
                        Rectangle boundary(0, 0, windowWidth, windowHeight);
                        quadTree = new QuadTree(boundary);
                        
                        // Re-add points that fit in new bounds
                        for (const QuadPoint& point : points) {
                            if (point.x >= 0 && point.x < windowWidth && 
                                point.y >= 0 && point.y < windowHeight) {
                                quadTree->insert(point);
                            }
                        }
                    }
                }
                break;
        }
    }
    return running;
}

void SDLRenderer::handleMouseDown(const SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        // Start drag for query or add point
        queryStartX = event.button.x;
        queryStartY = event.button.y;
        isDragging = true;
        showQuery = false;
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
        // Clear points
        clearPoints();
    }
}

void SDLRenderer::handleMouseUp(const SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT && isDragging) {
        isDragging = false;
        
        float deltaX = std::abs(event.button.x - queryStartX);
        float deltaY = std::abs(event.button.y - queryStartY);
        
        // If mouse didn't move much, add a point. Otherwise, end query.
        if (deltaX < 5 && deltaY < 5) {
            addPoint(event.button.x, event.button.y);
            showQuery = false;
        } else {
            endQuery();
        }
    }
}

void SDLRenderer::handleMouseMotion(const SDL_Event& event) {
    if (isDragging) {
        updateQuery(event.motion.x, event.motion.y);
    }
}

void SDLRenderer::handleKeyDown(const SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            running = false;
            break;
            
        case SDLK_SPACE:
            addRandomPoints(50);
            break;
            
        case SDLK_r:
            addRandomPoints(200);
            break;
            
        case SDLK_c:
            clearPoints();
            break;
    }
}

void SDLRenderer::addPoint(float x, float y) {
    if (quadTree) {
        QuadPoint point(x, y);
        quadTree->insert(point);
    }
}

void SDLRenderer::addRandomPoints(int count) {
    if (!quadTree) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0, windowWidth);
    std::uniform_real_distribution<float> yDist(0, windowHeight);
    
    for (int i = 0; i < count; i++) {
        QuadPoint point(xDist(gen), yDist(gen));
        quadTree->insert(point);
    }
}

void SDLRenderer::clearPoints() {
    if (quadTree) {
        quadTree->clear();
        showQuery = false;
    }
}

void SDLRenderer::startQuery(float x, float y) {
    queryStartX = x;
    queryStartY = y;
    showQuery = true;
}

void SDLRenderer::updateQuery(float x, float y) {
    float minX = std::min(queryStartX, x);
    float minY = std::min(queryStartY, y);
    float maxX = std::max(queryStartX, x);
    float maxY = std::max(queryStartY, y);
    
    queryRange = Rectangle(minX, minY, maxX - minX, maxY - minY);
    showQuery = true;
}

void SDLRenderer::endQuery() {
    // Query range is already set by updateQuery
}

void SDLRenderer::render() {
    // Draw enhanced background
    drawGradientBackground();
    
    // Draw subtle grid lines
    drawGridLines();
    
    if (quadTree) {
        drawQuadTree();
        drawPoints();
        
        if (showQuery) {
            drawQueryRange();
            drawQueryResults();
        }
    }
    
    // Draw UI elements
    drawStats();
    drawInstructions();
}

void SDLRenderer::present() {
    SDL_RenderPresent(renderer);
}

void SDLRenderer::drawRectangle(const Rectangle& rect, const Color& color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Rect sdlRect = {
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.width),
        static_cast<int>(rect.height)
    };
    
    if (filled) {
        SDL_RenderFillRect(renderer, &sdlRect);
    } else {
        SDL_RenderDrawRect(renderer, &sdlRect);
    }
}

void SDLRenderer::drawPoint(const QuadPoint& point, const Color& color, float radius) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Draw a filled circle using SDL_RenderDrawPoints
    int x = static_cast<int>(point.x);
    int y = static_cast<int>(point.y);
    int r = static_cast<int>(radius);
    
    // Simple circle drawing algorithm
    for (int dx = -r; dx <= r; dx++) {
        for (int dy = -r; dy <= r; dy++) {
            if (dx * dx + dy * dy <= r * r) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void SDLRenderer::drawQuadTree() {
    std::vector<Rectangle> boundaries = quadTree->getBoundaries();
    
    for (const Rectangle& boundary : boundaries) {
        drawRectangle(boundary, boundaryColor, false);
    }
}

void SDLRenderer::drawPoints() {
    std::vector<QuadPoint> points = quadTree->getAllPoints();
    
    for (const QuadPoint& point : points) {
        drawPoint(point, pointColor, 3.0f);
    }
}

void SDLRenderer::drawQueryRange() {
    if (queryRange.width > 0 && queryRange.height > 0) {
        // Draw query rectangle with thicker border
        SDL_SetRenderDrawColor(renderer, queryColor.r, queryColor.g, queryColor.b, queryColor.a);
        
        SDL_Rect rect = {
            static_cast<int>(queryRange.x),
            static_cast<int>(queryRange.y),
            static_cast<int>(queryRange.width),
            static_cast<int>(queryRange.height)
        };
        
        // Draw multiple rectangles for thicker border
        for (int i = 0; i < 2; i++) {
            SDL_RenderDrawRect(renderer, &rect);
            rect.x -= 1;
            rect.y -= 1;
            rect.w += 2;
            rect.h += 2;
        }
    }
}

void SDLRenderer::drawQueryResults() {
    if (queryRange.width > 0 && queryRange.height > 0) {
        std::vector<QuadPoint> queryPoints = quadTree->query(queryRange);
        
        for (const QuadPoint& point : queryPoints) {
            drawPoint(point, queryResultColor, 5.0f);
        }
    }
}

void SDLRenderer::drawGradientBackground() {
    // Create a subtle gradient from top to bottom
    for (int y = 0; y < windowHeight; y++) {
        float t = static_cast<float>(y) / windowHeight;
        Uint8 r = static_cast<Uint8>(15 + t * 10);  // 15 to 25
        Uint8 g = static_cast<Uint8>(15 + t * 10);  // 15 to 25
        Uint8 b = static_cast<Uint8>(25 + t * 15);  // 25 to 40
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }
}

void SDLRenderer::drawGridLines() {
    // Draw subtle grid lines
    SDL_SetRenderDrawColor(renderer, 40, 40, 50, 128);
    
    int gridSize = 50;
    
    // Vertical lines
    for (int x = gridSize; x < windowWidth; x += gridSize) {
        SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
    }
    
    // Horizontal lines
    for (int y = gridSize; y < windowHeight; y += gridSize) {
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }
}

void SDLRenderer::drawStats() {
    if (!quadTree) return;
    
    std::vector<QuadPoint> allPoints = quadTree->getAllPoints();
    std::vector<Rectangle> boundaries = quadTree->getBoundaries();
    
    int totalPoints = allPoints.size();
    int subdivisions = boundaries.size();
    int queryResults = 0;
    
    if (showQuery && queryRange.width > 0 && queryRange.height > 0) {
        queryResults = quadTree->query(queryRange).size();
    }
    
    // Draw a semi-transparent background for stats
    SDL_Rect statsBg = {10, 10, 200, 80};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderFillRect(renderer, &statsBg);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &statsBg);
    
    // Note: For simplicity, we're drawing basic text info as rectangles
    // In a real implementation, you'd use SDL_ttf for proper text rendering
    SDL_SetRenderDrawColor(renderer, 150, 255, 150, 255);
    
    // Draw simple indicators (dots) representing stats
    int y = 25;
    for (int i = 0; i < totalPoints && i < 50; i++) {
        int x = 20 + (i % 10) * 3;
        int py = y + (i / 10) * 3;
        SDL_RenderDrawPoint(renderer, x, py);
    }
}

void SDLRenderer::drawInstructions() {
    // Draw instructions background
    SDL_Rect instructionsBg = {windowWidth - 220, 10, 210, 120};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderFillRect(renderer, &instructionsBg);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &instructionsBg);
    
    // Draw simple instruction indicators
    SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
    
    // Left click indicator (small filled circle)
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx * dx + dy * dy <= 4) {
                SDL_RenderDrawPoint(renderer, windowWidth - 200 + dx, 30 + dy);
            }
        }
    }
    
    // Drag indicator (rectangle)
    SDL_Rect dragRect = {windowWidth - 200, 50, 20, 10};
    SDL_RenderDrawRect(renderer, &dragRect);
    
    // Right click indicator
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx * dx + dy * dy <= 4) {
                SDL_RenderDrawPoint(renderer, windowWidth - 200 + dx, 70 + dy);
            }
        }
    }
}
