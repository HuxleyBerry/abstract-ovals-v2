make: AbstractOvalUtils.cpp AbstractOvalUtils.h SymmetricGroupElement.h SymmetricGroupUtils.h program.cpp exact-cover-solvers/dancing-links.cpp exact-cover-solvers/dancing-links.h
	g++ program.cpp AbstractOvalUtils.cpp exact-cover-solvers/dancing-links.cpp -O3 -o program

tests: exact-cover-solvers/dancing-links.cpp exact-cover-solvers/dancing-links.h tests/test-algo-x.cpp
	g++ tests/test-algo-x.cpp exact-cover-solvers/dancing-links.cpp -O3 -o run-tests