
ROOTDIR = $(shell git rev-parse --show-toplevel)
BUILDDIR = $(ROOTDIR)/build
BINDIR = $(BUILDDIR)/bin
OBJDIR = $(BUILDDIR)/tests/obj
INCLUDES = -I$(ROOTDIR)/include -I$(ROOTDIR)/lib -I$(ROOTDIR)/lib/googletest/googletest/include
LIBDIRS = -L$(BUILDDIR)/lib -L$(ROOTDIR)/lib/googletest/build/lib
LIBS = -lsim -lgtest_main -lgtest

LIBFILE = $(BUILDDIR)/lib/libsim.a

CXXFLAGS = -fPIC

SOURCES = $(wildcard *.cpp)  $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJFILES = $(notdir $(SOURCES:.cpp=.o))
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJFILES))

$(BINDIR)/tests: $(OBJECTS) $(LIBFILE)
	g++ $(CXXFLAGS) -Wl,-rpath,$(BUILDDIR)/lib $(LIBDIRS) $< $(LIBS) -o $@

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)/tests
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generate dependencies
make-depend-cxx=$(CXX) -MM -MF $3 -MP -MT $2 $(CXXFLAGS) $(INCLUDES) $1
-include $(addprefix $(OBJDIR)/,$(OBJFILES:.o=.d))