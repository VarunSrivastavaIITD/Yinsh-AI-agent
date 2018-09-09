PROGNAME = main
LIBS = 
INCLUDES = -I/usr/local/include
LDFLAGS = -L./
OBJECTS = main.o State.o Board.o
CFLAGS = -Wall -Wextra -ggdb3 -std=c++11 -pedantic

all: $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	@mkdir -p bin
	g++ -ggdb3 -o bin/$(PROGNAME) build/*.o $(LIBS) $(INCLUDES) $(LDFLAGS)

$(OBJECTS): Makefile

%.o: %.cpp
	@mkdir -p build
	g++ -c $(CFLAGS) $(INCLUDES) -o build/$@ $<

clean:
	rm -rf build *.o $(PROGNAME)
