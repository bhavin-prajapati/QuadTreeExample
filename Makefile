# Compiler settings
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2 -march=native
DEBUG_FLAGS = -std=c++14 -Wall -Wextra -g -O0 -DDEBUG
TARGET = QuadTreeExample
SOURCES = QuadTree.cpp QuadTreeExample.cpp
HEADERS = QuadTree.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Release build
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Debug build
debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(TARGET)

# Individual object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

# Install dependencies (for development)
install-deps:
	@echo "No external dependencies required for this project."
	@echo "Make sure you have a C++14 compatible compiler installed."

# Run the example
run: $(TARGET)
	./$(TARGET)

# Run with performance profiling (Linux/Mac)
profile: $(TARGET)
	time ./$(TARGET)

# Windows-specific targets
windows: CXX = g++
windows: TARGET = QuadTreeExample.exe
windows: $(TARGET)

run-windows: windows
	$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the release version (default)"
	@echo "  debug     - Build debug version with symbols"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the example"
	@echo "  profile   - Run with performance timing"
	@echo "  windows   - Build for Windows"
	@echo "  help      - Show this help message"

# Static analysis (if cppcheck is available)
analyze:
	@echo "Running static analysis..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --std=c++14 $(SOURCES) $(HEADERS); \
	else \
		echo "cppcheck not found. Install it for static analysis."; \
	fi

# Format code (if clang-format is available)
format:
	@echo "Formatting code..."
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i $(SOURCES) $(HEADERS); \
		echo "Code formatted successfully."; \
	else \
		echo "clang-format not found. Install it for code formatting."; \
	fi

# Documentation generation (if doxygen is available)
docs:
	@echo "Generating documentation..."
	@if command -v doxygen >/dev/null 2>&1; then \
		echo "PROJECT_NAME = QuadTree" > Doxyfile.tmp; \
		echo "INPUT = $(SOURCES) $(HEADERS)" >> Doxyfile.tmp; \
		echo "OUTPUT_DIRECTORY = docs" >> Doxyfile.tmp; \
		echo "GENERATE_HTML = YES" >> Doxyfile.tmp; \
		echo "GENERATE_LATEX = NO" >> Doxyfile.tmp; \
		doxygen Doxyfile.tmp; \
		rm Doxyfile.tmp; \
		echo "Documentation generated in docs/ directory"; \
	else \
		echo "doxygen not found. Install it for documentation generation."; \
	fi

# Benchmark target
benchmark: $(TARGET)
	@echo "Running benchmark..."
	@for i in 1 2 3; do \
		echo "Run $$i:"; \
		time ./$(TARGET) | grep -E "(Insertion time|Query time|Queries per second)"; \
		echo ""; \
	done

.PHONY: all debug clean run profile windows run-windows help analyze format docs benchmark install-deps