#include "SDLRenderer.h"
#include <iostream>
#include <memory>

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

int main(int argc, char* argv[]) {
    std::cout << "Starting SDL QuadTree Visualization..." << std::endl;
    
    // Create renderer
    auto renderer = std::make_unique<SDLRenderer>(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Initialize SDL and create window
    if (!renderer->initialize()) {
        std::cerr << "Failed to initialize SDL renderer!" << std::endl;
        return -1;
    }
    
    // Add some initial points for demonstration
    renderer->addRandomPoints(25);
    
    std::cout << "Application started successfully!" << std::endl;
    
    // Main game loop
    Uint32 frameStart;
    int frameTime;
    
    while (renderer->isRunning()) {
        frameStart = SDL_GetTicks();
        
        // Handle events
        if (!renderer->handleEvents()) {
            break;
        }
        
        // Render everything
        renderer->render();
        renderer->present();
        
        // Cap the frame rate
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    
    std::cout << "Shutting down..." << std::endl;
    return 0;
}
