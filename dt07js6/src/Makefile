# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

# Targets
PROGS = memoryserver diskserver newsclient

OBJDIR    =   ../bin

$(OBJDIR):
	mkdir $(OBJDIR)	

all: $(PROGS)

install:  $(OBJDIR) 
	cp $(PROGS) ../bin

memoryserver: mainserver.o server.o connection.o databasememory.o
diskserver: mainserver.o server.o connection.o databasedisk.o
testdatabase: databasememory.o databasedisk.o testdatabase.o
newsclient: connection.o

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o *.d $(PROGS)


# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)