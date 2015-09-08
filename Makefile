TARGET=OpenGL-Viewer
SOURCE=OpenGL-Viewer.cpp LineRender.cpp Shader.cpp TextRender.cpp

VPATH=src
BUILDDIR=bin

OBJ=$(subst .cpp,.o,$(SOURCE)) 

CC=gcc
CXX=g++
CPPFLAGS=	-std=c++1y -O2 -Wall 
LDFLAGS	=	-Wall

INC =	-I/usr/include/freetype2
LIB =	-lGL -lfreetype -lglfw -lm -lGLU -lGLEW


all: $(TARGET)
	@echo Program compiled

$(TARGET):$(OBJ)
	echo "============="
	echo "Linking the target $@"
	echo "============="
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LIB)
	echo -- Link finished --

$(BUILDDIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	echo "============="
	echo "Compiling $<"
	$(CXX) $(CPPFLAGS) -I/usr/include/freetype2 $(INC) -c $< -o $@ 

clean:
	rm -f *.o
	rm -f *.out
	rm -f main
	rm -f *.a
