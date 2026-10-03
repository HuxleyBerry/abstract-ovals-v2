CXX := g++
CXXFLAGS := -O3 -std=c++17 -MMD -MP

SHARED_SOURCES := \
	AbstractOvalUtils.cpp \
	exact-cover-solvers/dancing-links.cpp \
	AbstractOvalFinder.cpp \
	orderly/MinimalImage.cpp

PROGRAM_SOURCES := \
	$(SHARED_SOURCES) \
	program.cpp

TEST_SOURCES := \
	$(SHARED_SOURCES) \
	$(wildcard tests/*.cpp)

PROGRAM_OBJECTS := $(PROGRAM_SOURCES:.cpp=.o)
TEST_OBJECTS := $(TEST_SOURCES:.cpp=.o)

all: program tests

tests: run-tests

program: $(PROGRAM_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run-tests: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f run-tests program
	find . -name '*.o' -delete
	find . -name '*.d' -delete

-include $(PROGRAM_OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)