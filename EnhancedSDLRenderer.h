#ifndef ENHANCED_SDL_RENDERER_H
#define ENHANCED_SDL_RENDERER_H

#include "SDLRenderer.h"
#include <chrono>
#include <unordered_map>

class EnhancedSDLRenderer : public SDLRenderer {
public:
    EnhancedSDLRenderer(int width, int height);
    
    void render() override;
    
private:
    // Animation system
    struct PointAnimation {
        QuadPoint position;
        float scale;
        float targetScale;
        std::chrono::steady_clock::time_point startTime;
        bool isActive;
        
        PointAnimation() : scale(0.0f), targetScale(1.0f), isActive(true) {
            startTime = std::chrono::steady_clock::now();
        }
    };
    
    // Enhanced visual features
    std::unordered_map<size_t, PointAnimation> pointAnimations;
    
    // Statistics display
    struct Stats {
        int totalPoints;
        int totalSubdivisions;
        int queryResults;
        float averageDepth;
        
        Stats() : totalPoints(0), totalSubdivisions(0), queryResults(0), averageDepth(0) {}
    };
    Stats currentStats;
    
    // Enhanced drawing methods
    void drawAnimatedPoints();
    void drawEnhancedBoundaries();
    void drawStats();
    void drawInstructions();
    
    // Animation helpers
    void updatePointAnimations();
    void addPointAnimation(const QuadPoint& point);
    size_t hashPoint(const QuadPoint& point);
    
    // Visual effects
    void drawGradientBackground();
    void drawGridPattern();
    
    // Color utilities
    struct ColorF {
        float r, g, b, a;
        ColorF(float r = 0, float g = 0, float b = 0, float a = 1.0f) 
            : r(r), g(g), b(b), a(a) {}
        
        Color toColor() const {
            return Color(static_cast<Uint8>(r * 255), 
                        static_cast<Uint8>(g * 255), 
                        static_cast<Uint8>(b * 255), 
                        static_cast<Uint8>(a * 255));
        }
    };
    
    ColorF interpolateColor(const ColorF& a, const ColorF& b, float t);
};

#endif // ENHANCED_SDL_RENDERER_H
