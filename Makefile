ROOTDIR = $(shell git rev-parse --show-toplevel)
BUILDDIR = $(ROOTDIR)/build
LIBFILES = $(BUILDDIR)/lib/libsim.a $(BUILDDIR)/lib/libgis.a

all: lib app test

app: $(BUILDDIR) $(LIBFILES)
	cd apps; make

$(BUILDDIR)/lib/libsim.a: $(BUILDDIR)
	cd src; make

$(BUILDDIR)/lib/libgis.a: $(BUILDDIR)
	cd gis; make

lib/CppWebServer:
	cd lib && git clone https://github.com/dtorban/CppWebServer.git
	cd lib/CppWebServer && mkdir -p build
	cd lib/CppWebServer/build && cmake ..

lib/googletest:
	cd lib && git clone https://github.com/google/googletest.git
	cd lib/googletest && mkdir -p build
	cd lib/googletest/build && cmake ..

lib/CppWebServer/build/install/include: lib/CppWebServer
	cd lib/CppWebServer/build && make install

lib/googletest/build/lib : lib/googletest
	cd lib/googletest/build && make

$(BUILDDIR): lib/CppWebServer/build/install/include lib/googletest/build/lib
	mkdir -p build/obj
	mkdir -p build/bin
	mkdir -p build/lib

test:
	cd tests && make
	$(BUILDDIR)/bin/tests

run:
	$(ROOTDIR)/bin/start.sh

debug:
	$(ROOTDIR)/bin/debug.sh

submission:
	zip -r project.zip include src tests -x "tests/example_test.cpp"

clean:
	rm -rf build
	rm -rf project.zip

clobber:
	rm -rf lib/CppWebServer
	rm -rf lib/googletest