CXX := g++
CXXFLAGS := -O3 -std=c++17

PROGRAM_SOURCES := \
	AbstractOvalUtils.cpp \
	AbstractOvalUtils.h \
	SymmetricGroupElement.h \
	SymmetricGroupUtils.h \
	exact-cover-solvers/dancing-links.cpp \
	exact-cover-solvers/dancing-links.h \
	AbstractOvalFinder.h \
	AbstractOvalFinder.cpp

TEST_SOURCES := \
	$(PROGRAM_SOURCES) \
	$(wildcard tests/*.cpp) \
	$(wildcard tests/*.h)

all: program tests

tests: run-tests

program: $(PROGRAM_SOURCES)
	$(CXX) $(CXXFLAGS) program.cpp $(filter %.cpp,$(PROGRAM_SOURCES)) -o $@

run-tests: $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(filter %.cpp,$(TEST_SOURCES)) -o $@

clean:
	rm -f run-tests program