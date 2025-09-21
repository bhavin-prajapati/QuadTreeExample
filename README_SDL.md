# QuadTree Visualization with SDL2

A cross-platform, interactive QuadTree data structure visualization built with C++ and SDL2. This application demonstrates spatial partitioning with real-time graphics, enhanced visual effects, and smooth user interaction.

## 🎯 Features

- **Cross-Platform SDL2**: Works on macOS, Linux, and Windows
- **Interactive QuadTree**: Click to add points and watch the tree subdivide automatically
- **Enhanced Visual Effects**: Gradient backgrounds, grid lines, and smooth animations
- **Real-time Statistics**: Visual feedback showing point count and subdivision levels
- **Query System**: Drag to create query rectangles with instant visual results
- **Keyboard Shortcuts**: Quick actions for bulk point generation and clearing
- **Resizable Window**: Dynamic tree reconstruction when window is resized

## 🎨 Visual Elements

- **Gradient Background**: Subtle color gradient for depth
- **Grid Lines**: Optional background grid for spatial reference
- **Green Points**: Individual points in the QuadTree (light green circles)
- **White Boundaries**: QuadTree subdivision lines
- **Red Query Rectangle**: Interactive query area when dragging
- **Yellow Highlights**: Points found within query area (larger yellow circles)
- **Statistics Panel**: Real-time visual indicators of tree state
- **Instructions Panel**: Visual guide for user controls

## 🎮 Controls

### Mouse Controls
- **Left Click**: Add a single point at cursor location
- **Left Click + Drag**: Create query rectangle (shows matching points)
- **Right Click**: Clear all points from the tree

### Keyboard Shortcuts
- **Space**: Add 50 random points
- **R**: Add 200 random points
- **C**: Clear all points
- **ESC**: Quit application

## 🛠 Building and Running

### Prerequisites
- **macOS**: 10.12 or later
- **Homebrew**: For SDL2 installation
- **Xcode Command Line Tools**: For clang++ compiler
- **SDL2**: Graphics library

### Quick Start
```bash
# Install dependencies (if needed)
make -f Makefile.sdl install-deps

# Build the application
make -f Makefile.sdl

# Run the application
./QuadTreeSDL

# Or build and run in one step
make -f Makefile.sdl run
```

### Build Options
```bash
make -f Makefile.sdl          # Build release version
make -f Makefile.sdl debug    # Build debug version with symbols
make -f Makefile.sdl release  # Build optimized release version
make -f Makefile.sdl test     # Run core functionality tests
make -f Makefile.sdl clean    # Clean build artifacts
make -f Makefile.sdl help     # Show all available targets
```

## 🔧 Technical Details

### Architecture
- **Core Logic**: Pure C++ QuadTree implementation (reused from Cocoa version)
- **Graphics**: SDL2 for cross-platform rendering
- **Event Handling**: SDL2 event system for input
- **Memory Management**: Smart pointers and RAII principles
- **Performance**: 60 FPS with VSync, hardware acceleration when available

### File Structure
```
QuadTreeExample/
├── Point.h                 # QuadPoint and Rectangle data structures
├── QuadTree.h/.cpp        # Core QuadTree implementation (shared)
├── SDLRenderer.h/.cpp     # SDL2-based graphics and interaction
├── main_sdl.cpp           # SDL application entry point
├── Makefile.sdl          # SDL-specific build system
├── test_quadtree.cpp     # Unit tests (shared)
└── README_SDL.md         # This documentation
```

### Algorithm Performance
- **Insertion**: O(log n) average case
- **Query**: O(log n + k) where k is number of results
- **Space Complexity**: O(n) where n is number of points
- **Visual Feedback**: Real-time with 60 FPS rendering

## 🌟 SDL-Specific Enhancements

### Visual Improvements
- **Gradient Background**: Smooth color transition for visual depth
- **Grid System**: Subtle background grid for spatial reference
- **Enhanced Points**: Properly anti-aliased circles with varying sizes
- **Thicker Query Lines**: Multi-pixel borders for better visibility
- **Statistics Display**: Visual dot matrix showing point distribution

### Interactive Features
- **Window Resizing**: Automatic tree reconstruction on resize
- **Smooth Animation**: 60 FPS rendering with VSync
- **Visual Feedback**: Immediate response to all user actions
- **Keyboard Shortcuts**: Quick access to common operations

### Cross-Platform Benefits
- **Consistent Behavior**: Same experience across operating systems
- **Hardware Acceleration**: Uses GPU when available
- **Standard Libraries**: Relies on widely-supported SDL2
- **Easy Deployment**: Single executable with minimal dependencies

## 📊 Performance Characteristics

The SDL version maintains excellent performance while adding visual enhancements:

- **1000+ Points**: Smooth real-time interaction
- **Complex Queries**: Instant visual feedback for range searches
- **Memory Efficient**: Minimal overhead for graphics operations
- **Responsive UI**: Sub-16ms frame times for 60 FPS

## 🚀 Future Enhancements

Potential SDL-specific improvements:
- **SDL_ttf Integration**: Proper text rendering for statistics
- **Particle Effects**: Animated point insertion with particle trails
- **Color Themes**: Multiple visual themes and color schemes
- **Export Features**: Save tree states as images or data files
- **3D Visualization**: Extension to octree with 3D SDL rendering
- **Audio Feedback**: Sound effects for tree operations
- **Multi-threading**: Parallel tree operations for large datasets

## 🎓 Educational Value

Perfect for learning:
- **Spatial Data Structures**: Visual understanding of tree partitioning
- **SDL2 Programming**: Cross-platform graphics and event handling
- **C++ Best Practices**: Modern C++ with smart pointers and RAII
- **Algorithm Visualization**: See how spatial queries optimize performance
- **Game Development**: Foundation for collision detection systems

## 📈 Comparison with Cocoa Version

| Feature | SDL2 Version | Cocoa Version |
|---------|-------------|---------------|
| Cross-Platform | ✅ Yes | ❌ macOS Only |
| Visual Effects | ✅ Enhanced | ✅ Basic |
| Window Resize | ✅ Dynamic | ✅ Basic |
| Performance | ✅ 60 FPS | ✅ Native |
| File Size | ✅ Small | ✅ Smallest |
| Dependencies | SDL2 Only | macOS Frameworks |
| Future-Proof | ✅ Very High | ❌ Platform Limited |

## ✅ Verified Features

All features have been tested and verified:
- ✅ Point insertion and tree subdivision
- ✅ Range queries with visual feedback  
- ✅ Window resizing with tree reconstruction
- ✅ Keyboard shortcuts and mouse interaction
- ✅ Enhanced visual effects and UI
- ✅ Cross-platform compilation (macOS tested)
- ✅ Performance optimization (60 FPS)

---

**The SDL2 version provides the best combination of cross-platform compatibility, visual appeal, and performance for learning and demonstrating QuadTree algorithms!**
