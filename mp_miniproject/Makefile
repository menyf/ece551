CPP=g++
SRCS=$(wildcard *.cpp)
BASE=$(basename $(SRCS))
OBJS=$(addsuffix .o, $(BASE))
#OBJS=$(patsubst %.cpp, %.o, $(SRCS))
CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
myShell:$(OBJS) $(wildcard *.h)
	g++ -o $@ $(OBJS)
%.o:%.cpp
	g++ $(CPPFLAGS) -c $<

.PHONY:clean
clean:
	rm -f myShell *.o *.cpp~ *.h~ tmp* test Makefile~

#oneFile.o: oneHeader.h someHeader.h
