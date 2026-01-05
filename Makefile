# C++ Types Zoo - Makefile
# Generates a README.md showing type deduction behavior

CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = types_zoo
SOURCE = main.cpp
README = README.md

.PHONY: all clean report

# Default target: build and generate report
all: report

# Build the executable
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Generate README.md by running the program
report: $(TARGET)
	./$(TARGET) > $(README)
	@echo "Generated $(README)"

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Clean everything including generated report
distclean: clean
	rm -f $(README)

# Rebuild everything
rebuild: distclean all

