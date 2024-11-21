# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Igoogletest/googletest/include
COVFLAGS = -fprofile-arcs -ftest-coverage

# Directories
SRCDIR = .
TESTDIR = test
BUILDDIR = build
BINDIR = bin
GTESTDIR = googletest/googletest

# Files
GTEST_SOURCES = $(GTESTDIR)/src/gtest-all.cc
GTEST_OBJS = $(BUILDDIR)/gtest-all.o
MAIN_SOURCES = $(SRCDIR)/main.cpp
TEST_SOURCES = $(TESTDIR)/test_cases.cpp

# Targets
MAIN_TARGET = $(BINDIR)/ToyRobot
TEST_TARGET = $(BINDIR)/TestingToyRobot
MAIN_TARGET_COV = $(BINDIR)/ToyRobotCoverage
TEST_TARGET_COV = $(BINDIR)/TestingToyRobotCoverage

# Ensure directories exist
$(shell mkdir -p $(BUILDDIR) $(BINDIR))

# Compile Google Test
$(GTEST_OBJS): $(GTEST_SOURCES)
	$(CXX) $(CXXFLAGS) -I$(GTESTDIR) -c $< -o $@

# Build the main application
$(MAIN_TARGET): $(MAIN_SOURCES)
	$(CXX) $(CXXFLAGS) $(MAIN_SOURCES) -o $@

# Build the test executable
$(TEST_TARGET): $(TEST_SOURCES) $(GTEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) $(GTEST_OBJS) -o $@ -pthread -I$(SRCDIR)

# Build the main application with coverage flags
$(MAIN_TARGET_COV): $(MAIN_SOURCES)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(MAIN_SOURCES) -o $@

# Build the test executable with coverage flags
$(TEST_TARGET_COV): $(TEST_SOURCES) $(GTEST_OBJS)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(TEST_SOURCES) $(GTEST_OBJS) -o $@ -pthread -I$(SRCDIR)

# Run the main program
run: $(MAIN_TARGET)
	./$(MAIN_TARGET) $(TESTDIR)/input.txt

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run the main program with coverage flags
coverage: $(MAIN_TARGET_COV)
	./$(MAIN_TARGET_COV) $(TESTDIR)/input.txt

# Run the main program with coverage flags
test_coverage: $(TEST_TARGET_COV)
	./$(TEST_TARGET_COV)

# Clean build and binary directories
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: run test clean
