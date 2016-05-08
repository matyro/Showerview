
SRCDIR = $(shell pwd)
BUILDDIR = $(SRCDIR)/build
TESTDIR = $(SRCDIR)/test


RAY_EXECUTABLE := Raytrace
RAY := src

CC = gcc-5
CFLAGS = -Wall -O2 -g -m64
CXX = g++-5
CXXFLAGS = -std=c++1y -Wall -O2 -m64 -g 
F77 = gfortran
FFLAGS = -O2 -g 

CINCLUDE = -I$(SRCDIR)/$(RAY) -I$(SRCDIR)/glm/ -I. -Iquote
CXXINCLUDE = -I$(SRCDIR)/$(RAY) -I$(SRCDIR)/glm/ -I. -Iquote

LIB_DIR := -L/usr/lib/ 
LDFLAGS := -lglfw -lGL -lGLU -lGLEW -lOpenCL


CFILES := c
CXXFILES := cpp
OFILES := o

RAY_DIR := $(addprefix $(SRCDIR)/, $(RAY))
RAY_BUILD := $(addprefix $(BUILDDIR)/, $(RAY))
RAY_SRCS := $(shell find $(RAY_DIR) -name '*.cpp')
RAY_OBJ  := $(patsubst $(SRCDIR)/%.$(CXXFILES),$(BUILDDIR)/%.o,$(RAY_SRCS))




all: Ray

Ray: $(RAY_OBJ)
	@echo 'Compile UnitTest'
	$(CXX) $(CXX_FLAGS) $(LIB_DIR) -pthread -o build/$(RAY_EXECUTABLE) $(RAY_OBJ) $(LDFLAGS)
##	$(shell ./build/UnitTest)

.PHONY: Run
Run: Ray
	@echo 'Run Program: '
	cd build; ./Raytrace


$(BUILDDIR)/%.$(OFILES): %.$(CFILES)
	@echo 'other compile'
	$(CC) $(CINCLUDE) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.$(OFILES): %.$(CXXFILES)
	@echo 'Compile CPP'
	$(shell mkdir -p $(shell dirname $@))
	$(CXX) $(CXXINCLUDE) $(CXXFLAGS) -lglfw -lglew -lopengl32 -c $< -o $@


clean: 
	rm -f build/Raytrace
	find build/ -name "*.o" -type f | xargs rm -f	
