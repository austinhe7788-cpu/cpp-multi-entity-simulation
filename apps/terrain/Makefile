
ROOTDIR = $(shell git rev-parse --show-toplevel)
BUILDDIR = $(ROOTDIR)/build
BINDIR = $(BUILDDIR)/bin
OBJDIR = $(BUILDDIR)/terrain/obj
INCLUDES = -I$(ROOTDIR)/include -I$(ROOTDIR)/lib -I$(ROOTDIR)/gis/include
LIBDIRS = -L$(BUILDDIR)/lib
LIBS = -lgis -lcurl

LIBGISFILE = $(BUILDDIR)/lib/libgis.a

CXXFLAGS = -g -fPIC

SOURCES = $(wildcard *.cpp)  $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJFILES = $(notdir $(SOURCES:.cpp=.o))
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJFILES))

$(BINDIR)/terrain: $(OBJECTS) $(LIBGISFILE)
	g++ $(CXXFLAGS) -Wl,-rpath,$(BUILDDIR)/lib $(LIBDIRS) $(OBJECTS) $(LIBS) -o $@

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)/web-app
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generate dependencies
make-depend-cxx=$(CXX) -MM -MF $3 -MP -MT $2 $(CXXFLAGS) $(INCLUDES) $1
-include $(addprefix $(OBJDIR)/,$(OBJFILES:.o=.d))