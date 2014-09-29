CXX = g++

LDFLAGS = -lglut -Wall -ggdb


OBJS = parts.o segment.o bud.o leaf.o main.o meristem.o tree.o
SRCS = $(OBJS:.o=.cpp)

tree: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o tree 

clean:
	rm -f *.o *~ .*swp 
