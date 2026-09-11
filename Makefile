make: AbstractOvalUtils.cpp AbstractOvalUtils.h SymmetricGroupElement.h SymmetricGroupUtils.h program.cpp exact-cover-solvers/dancing-links.cpp exact-cover-solvers/dancing-links.h AbstractOvalFinder.h AbstractOvalFinder.cpp
	g++ program.cpp AbstractOvalUtils.cpp AbstractOvalFinder.cpp exact-cover-solvers/dancing-links.cpp -O3 -o program

tests: exact-cover-solvers/dancing-links.cpp exact-cover-solvers/dancing-links.h tests/test-algo-x.cpp tests/main-test-runner.cpp tests/test-abstract-oval-finder.cpp tests/test-abstract-oval-utils.cpp
	g++ tests/main-test-runner.cpp tests/test-algo-x.cpp tests/test-abstract-oval-finder.cpp tests/test-abstract-oval-utils.cpp exact-cover-solvers/dancing-links.cpp AbstractOvalUtils.cpp -O3 -o run-tests