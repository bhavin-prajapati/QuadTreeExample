# QuadTree Visualization for macOS

A interactive QuadTree data structure visualization built with C++ and Cocoa for macOS. This application demonstrates spatial partitioning with real-time graphics and user interaction.

## Features

- **Interactive QuadTree**: Click to add points and watch the tree subdivide automatically
- **Visual Query System**: Drag to create query rectangles and see matching points highlighted
- **Real-time Graphics**: Smooth Core Graphics rendering with immediate visual feedback
- **Multiple Point Sets**: Add random point distributions of various sizes
- **Clean Interface**: Native macOS application with menu bar integration

## What is a QuadTree?

A QuadTree is a tree data structure where each internal node has exactly four children. It's used to partition 2D space by recursively subdividing regions into four quadrants. This makes spatial queries (like "find all points in this area") much more efficient than checking every point individually.

## Controls

- **Left Click**: Add a point at the cursor location
- **Left Click + Drag**: Create a query rectangle (shows matching points in red)
- **Right Click**: Clear all points
- **Menu Options**: 
  - Add 10/50/200 random points
  - Clear all points
  - Show help instructions

## Visual Elements

- **Green Dots**: Individual points in the QuadTree
- **White Lines**: QuadTree subdivision boundaries
- **Red Rectangle**: Query area when dragging
- **Large Red Dots**: Points found within the query area

## Building and Running

### Prerequisites
- macOS 10.12 or later
- Xcode Command Line Tools
- clang++ with C++17 support

### Quick Start
```bash
# Build the application
make

# Run the application
./QuadTreeVisualization

# Or run the app bundle
open QuadTreeVisualization.app

# Test the core functionality
make test
```

### Build Options
```bash
make          # Build the executable
make bundle   # Create a .app bundle
make clean    # Clean build artifacts
make help     # Show help
```

## Code Structure

- **Point.h**: Basic 2D point and rectangle structures
- **QuadTree.h/cpp**: Core QuadTree implementation with spatial partitioning
- **QuadTreeRenderer.h/mm**: Cocoa view for rendering and user interaction
- **main.mm**: macOS application setup and menu system
- **Makefile**: Build system for compilation

## Algorithm Details

The QuadTree implementation uses:
- **Capacity-based subdivision**: Each node holds up to 4 points before subdividing
- **Recursive spatial queries**: Efficient range searching with boundary checking
- **Dynamic tree structure**: Nodes only subdivide when needed

### Time Complexity
- **Insert**: O(log n) average case
- **Query**: O(log n + k) where k is the number of results
- **Space**: O(n) where n is the number of points

## Performance

This implementation can handle thousands of points with smooth real-time interaction. The visual feedback helps demonstrate how the QuadTree automatically balances itself and improves query efficiency as more points are added.

## Educational Use

Perfect for:
- Learning spatial data structures
- Understanding recursive tree algorithms
- Visualizing algorithmic behavior
- Computer graphics and game development concepts

## License

This project is created for educational purposes. Feel free to use and modify as needed.

## Future Enhancements

Potential improvements:
- Collision detection demonstration
- Performance metrics display
- Different subdivision strategies
- 3D octree visualization
- Export functionality for tree structures
