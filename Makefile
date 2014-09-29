CXX = g++

LDFLAGS = -lglut -lpng -Wall -ggdb -lGLEW -lGL -lGLU
INC=-I/usr/include


OBJS = parts.o segment.o bud.o glLogger.o glsl.o leaf.o main.o objloader.o texture.o tree.o
SRCS = $(OBJS:.o=.cpp)

tree: $(OBJS)
	$(CXX) $(LDFLAGS) $(INC) $(OBJS) -o tree 

clean:
	rm -f *.o *~ .*swp 
