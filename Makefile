IDIR =./
CXX = g++
CXXFLAGS = -Wno-deprecated -std=c++0x
PREFIX = /usr/local
DEPS = -lpng -lX11 -lpthread

SOURCES = src/Main.cpp src/ReconstructOctree.cpp src/Octree.cpp src/InitPara.cpp src/TriSurfMesh.cpp

3dreconstruction:
	mkdir -pv bin/
	$(CXX) $(CXXFLAGS) $(SOURCES) -I$(IDIR) $(DEPS) -o bin/$@ $^

.PHONY: clean

clean:
	rm -rvf bin/
	rm -vf 3dreconstruction
	find . -type f -iname "*.o" -delete

.PHONY: install
install: 3dreconstruction
	mkdir -pv $(DESTDIR)$(PREFIX)/bin
	cp -v 3dreconstruction $(DESTDIR)$(PREFIX)/bin/3dreconstruction

.PHONY: uninstall
uninstall:
	rm -vf $(DESTDIR)$(PREFIX)/bin/3dreconstruction