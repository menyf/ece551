CPP=g++
SRCS=$(wildcard *.cpp)
BASE=$(basename $(SRCS))
OBJS=$(addsuffix .o, $(addprefix obj/,$(BASE)))
#OBJS=$(patsubst %.cpp, %.o, $(SRCS))
CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
myShell:$(OBJS)
	CPP -o $@ $(OBJS)
%.o:%.h %.cpp
	CPP $(CPPFLAGS) -c $<

.PHONY:clean
clean:
	rm -f myShell *.o *.cpp~ *.h~ tmp* test

#oneFile.o: oneHeader.h someHeader.h
