#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL.h>
#include "QuadTree.h"
#include <vector>

class SDLRenderer {
public:
    SDLRenderer(int width, int height);
    ~SDLRenderer();
    
    bool initialize();
    void cleanup();
    
    // Main rendering loop
    bool handleEvents();
    void render();
    void present();
    
    // QuadTree interaction
    void setQuadTree(QuadTree* tree);
    void addRandomPoints(int count);
    void clearPoints();
    void addPoint(float x, float y);
    
    // Query functionality
    void startQuery(float x, float y);
    void updateQuery(float x, float y);
    void endQuery();
    
    // Getter for running status
    bool isRunning() const { return running; }
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    QuadTree* quadTree;
    
    int windowWidth, windowHeight;
    bool running;
    bool isDragging;
    
    // Query rectangle
    bool showQuery;
    Rectangle queryRange;
    float queryStartX, queryStartY;
    
    // Colors
    struct Color {
        Uint8 r, g, b, a;
        Color(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255) 
            : r(r), g(g), b(b), a(a) {}
    };
    
    Color backgroundColor;
    Color boundaryColor;
    Color pointColor;
    Color queryColor;
    Color queryResultColor;
    
    // Helper methods
    void drawRectangle(const Rectangle& rect, const Color& color, bool filled = false);
    void drawPoint(const QuadPoint& point, const Color& color, float radius = 2.0f);
    void drawQuadTree();
    void drawPoints();
    void drawQueryRange();
    void drawQueryResults();
    void drawStats();
    void drawInstructions();
    
    // Enhanced visual features
    void drawGradientBackground();
    void drawGridLines();
    
    // Event handlers
    void handleMouseDown(const SDL_Event& event);
    void handleMouseUp(const SDL_Event& event);
    void handleMouseMotion(const SDL_Event& event);
    void handleKeyDown(const SDL_Event& event);
};

#endif // SDL_RENDERER_H
