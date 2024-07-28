# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Directories
SRCDIR = src
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin

# Files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
TARGET = $(BINDIR)/sfml-app

# SFML Libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Default rule
all: $(TARGET)

# Build the target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the project
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
