
ROOTDIR = $(shell git rev-parse --show-toplevel)
BUILDDIR = $(ROOTDIR)/build
BINDIR = $(BUILDDIR)/bin
OBJDIR = $(BUILDDIR)/web-app/obj
INCLUDES = -I$(ROOTDIR)/include -I$(ROOTDIR)/lib -I$(ROOTDIR)/lib/CppWebServer/build/install/include
LIBDIRS = -L$(BUILDDIR)/lib -L$(ROOTDIR)/lib/CppWebServer/build/install/lib
LIBS = -lsim -lCppWebServer -lwebsockets -lcap -lcrypto -lssl -lz

LIBFILE = $(BUILDDIR)/lib/libsim.a

CXXFLAGS = -g -fPIC

SOURCES = $(wildcard *.cpp)  $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJFILES = $(notdir $(SOURCES:.cpp=.o))
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJFILES))

$(BINDIR)/web-app: $(OBJECTS) $(LIBFILE)
	g++ $(CXXFLAGS) -Wl,-rpath,$(BUILDDIR)/lib $(LIBDIRS) $< $(LIBS) -o $@

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)/web-app
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generate dependencies
make-depend-cxx=$(CXX) -MM -MF $3 -MP -MT $2 $(CXXFLAGS) $(INCLUDES) $1
-include $(addprefix $(OBJDIR)/,$(OBJFILES:.o=.d))