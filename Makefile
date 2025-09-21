# Makefile for QuadTree Visualization
# macOS Cocoa Application

# Compiler settings
CXX = xcrun clang++
SDK_PATH = $(shell xcrun --show-sdk-path)
CXXFLAGS = -std=c++17 -Wall -O2 -stdlib=libc++ -isystem $(SDK_PATH)/usr/include/c++/v1
OBJCFLAGS = -fobjc-arc
FRAMEWORKS = -framework Cocoa -framework CoreGraphics

# Source files
CPP_SOURCES = QuadTree.cpp
MM_SOURCES = QuadTreeRenderer.mm main.mm
HEADERS = Point.h QuadTree.h QuadTreeRenderer.h

# Object files
CPP_OBJECTS = $(CPP_SOURCES:.cpp=.o)
MM_OBJECTS = $(MM_SOURCES:.mm=.o)
ALL_OBJECTS = $(CPP_OBJECTS) $(MM_OBJECTS)

# Target executable
TARGET = QuadTreeVisualization

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(ALL_OBJECTS)
	$(CXX) $(ALL_OBJECTS) $(FRAMEWORKS) -o $(TARGET)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile C++ source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Objective-C++ source files
%.o: %.mm $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJCFLAGS) -c $< -o $@

# Create application bundle (optional)
bundle: $(TARGET)
	@mkdir -p $(TARGET).app/Contents/MacOS
	@mkdir -p $(TARGET).app/Contents/Resources
	@cp $(TARGET) $(TARGET).app/Contents/MacOS/
	@echo '<?xml version="1.0" encoding="UTF-8"?>' > $(TARGET).app/Contents/Info.plist
	@echo '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> $(TARGET).app/Contents/Info.plist
	@echo '<plist version="1.0">' >> $(TARGET).app/Contents/Info.plist
	@echo '<dict>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<key>CFBundleExecutable</key>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<string>$(TARGET)</string>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<key>CFBundleIdentifier</key>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<string>com.example.quadtree</string>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<key>CFBundleName</key>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<string>QuadTree Visualization</string>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<key>CFBundleVersion</key>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<string>1.0</string>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<key>LSMinimumSystemVersion</key>' >> $(TARGET).app/Contents/Info.plist
	@echo '	<string>10.12</string>' >> $(TARGET).app/Contents/Info.plist
	@echo '</dict>' >> $(TARGET).app/Contents/Info.plist
	@echo '</plist>' >> $(TARGET).app/Contents/Info.plist
	@echo "App bundle created: $(TARGET).app"

# Test the QuadTree implementation
test: test_quadtree
	./test_quadtree

test_quadtree: test_quadtree.cpp $(CPP_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) test_quadtree.cpp QuadTree.cpp -o test_quadtree

# Clean build artifacts
clean:
	rm -f $(ALL_OBJECTS) $(TARGET) test_quadtree
	rm -rf $(TARGET).app
	@echo "Cleaned build artifacts"

# Show help
help:
	@echo "QuadTree Visualization Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all     - Build the executable (default)"
	@echo "  bundle  - Create macOS app bundle"
	@echo "  test    - Run QuadTree functionality tests"
	@echo "  clean   - Remove build artifacts"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make        # Build the application"
	@echo "  make bundle # Create .app bundle"
	@echo "  make test   # Run tests"
	@echo "  make clean  # Clean up"

# Declare phony targets
.PHONY: all clean bundle help test
