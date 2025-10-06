# Makefile for Ray Tracer C++ Application
# Optimized for Render.com deployment

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -O3
LDFLAGS = -lm

# Target executable
TARGET = calcus

# Source files
SOURCES = calcus.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS = -std=c++17 -g -O0 -Wall -Wextra -DDEBUG
debug: $(TARGET)
	@echo "Debug build complete"

# Release build with maximum optimization
release: CXXFLAGS = -std=c++17 -O3 -DNDEBUG -march=native -mtune=native -flto
release: clean $(TARGET)
	@echo "Release build complete"

# Profile build for performance analysis
profile: CXXFLAGS = -std=c++17 -O2 -g -pg -march=native
profile: LDFLAGS = -lm -pg
profile: $(TARGET)
	@echo "Profile build complete"

# Static build for deployment
static: CXXFLAGS = -std=c++17 -O3 -static -march=native
static: LDFLAGS = -static -lm
static: $(TARGET)
	@echo "Static build complete"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Install target (for deployment)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Uninstall target
uninstall:
	@echo "Removing $(TARGET) from /usr/local/bin..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete"

# Test the executable
test: $(TARGET)
	@echo "Testing $(TARGET)..."
	@echo '{"receiver_planes":{"plane1":{"width":2,"height":2,"points":[{"origin":[0,0,0],"normal":[0,1,0]}]}},"polygons":[{"polygon":[[0,1,0],[1,1,0],[1,1,1],[0,1,1]],"temperature":100}],"num_rays":1000}' | ./$(TARGET)
	@echo "Test complete"

# Performance test
perf-test: $(TARGET)
	@echo "Running performance test..."
	@echo '{"receiver_planes":{"plane1":{"width":2,"height":2,"points":[{"origin":[0,0,0],"normal":[0,1,0]}]}},"polygons":[{"polygon":[[0,1,0],[1,1,0],[1,1,1],[0,1,1]],"temperature":100}],"num_rays":100000}' | time ./$(TARGET)
	@echo "Performance test complete"

# Memory check with valgrind (if available)
memcheck: $(TARGET)
	@echo "Running memory check..."
	@echo '{"receiver_planes":{"plane1":{"width":2,"height":2,"points":[{"origin":[0,0,0],"normal":[0,1,0]}]}},"polygons":[{"polygon":[[0,1,0],[1,1,0],[1,1,1],[0,1,1]],"temperature":100}],"num_rays":1000}' | valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)
	@echo "Memory check complete"

# Show build information
info:
	@echo "Build Information:"
	@echo "  Compiler: $(CXX)"
	@echo "  Flags: $(CXXFLAGS)"
	@echo "  Linker Flags: $(LDFLAGS)"
	@echo "  Target: $(TARGET)"
	@echo "  Sources: $(SOURCES)"

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the application (default)"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized release version"
	@echo "  profile    - Build for profiling"
	@echo "  static     - Build static executable"
	@echo "  clean      - Remove build artifacts"
	@echo "  install    - Install to /usr/local/bin"
	@echo "  uninstall  - Remove from /usr/local/bin"
	@echo "  test       - Run basic functionality test"
	@echo "  perf-test  - Run performance test"
	@echo "  memcheck   - Run memory check with valgrind"
	@echo "  info       - Show build information"
	@echo "  help       - Show this help message"

# Phony targets
.PHONY: all debug release profile static clean install uninstall test perf-test memcheck info help