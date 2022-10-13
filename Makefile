.PHONY: clean clean-build build install uninstall
.DEFAULT_GOAL := build

CXX = g++
CXXFLAGS = -Wno-deprecated -D LINUX -std=c++0x
PREFIX = /usr/local
SOURCES = src/Main.cpp src/ReconstructOctree.cpp src/InitPara.cpp src/Octree.cpp src/TriSurfMesh.cpp
INCLUDES = -Iinclude/
DEPS = -lpng -lX11 -lpthread


clean: clean-build ## remove all build artifacts

clean-build: ## remove build artifacts
	rm -fr build/
	rm -fr dist/

build: clean ## builds source and wheel package
	mkdir -pv build/bin

	$(CXX) $(CXXFLAGS) $(SOURCES) $(INCLUDES) $(DEPS) -o build/bin/reconstruction3Dpers

install: build
	mkdir -pv $(PREFIX)/bin
	cp -v build/bin/reconstruction3Dpers $(PREFIX)/bin/reconstruction3Dpers

.PHONY: uninstall
uninstall:
	rm -vf $(PREFIX)/bin/reconstruction3Dpers