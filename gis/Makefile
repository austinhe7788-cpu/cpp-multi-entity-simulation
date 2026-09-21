ROOTDIR = $(shell git rev-parse --show-toplevel)
BUILDDIR = $(ROOTDIR)/build
BINDIR = $(BUILDDIR)/bin
OBJDIR = $(BUILDDIR)/obj

SOURCES = $(wildcard *.cpp)  $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJFILES = $(notdir $(SOURCES:.cpp=.o))
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJFILES))

SHARED_LIB = $(BUILDDIR)/lib/libgisShared.so
LIBFILE = $(BUILDDIR)/lib/libgis.a

CXXFLAGS = -g -fPIC
INCLUDES = -I$(ROOTDIR)/include -I$(ROOTDIR)/lib -I$(ROOTDIR)/lib -I$(ROOTDIR)/gis/include
LIBDIRS = 
LIBS = -lcurl

all: $(LIBFILE) $(SHARED_LIB) 

$(LIBFILE): $(OBJECTS)
	ar rvs $@ $(OBJECTS)

$(SHARED_LIB): $(OBJECTS)
	g++ $(CXXFLAGS) -shared $(LIBDIRS) $(LIBS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cpp
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: */%.cpp
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: */*/%.cpp
	$(call make-depend-cxx,$<,$@,$(subst .o,.d,$@))
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generate dependencies
make-depend-cxx=$(CXX) -MM -MF $3 -MP -MT $2 $(CXXFLAGS) $(INCLUDES) $1
-include $(addprefix $(OBJDIR)/,$(OBJFILES:.o=.d))