SRC=hw4.cc Game.cc # change these to your source file names
HEADERS=Game.h # change these to your header file names
LIBS=../HW2/libhw2.a  # you should use your hw2 code as a library
CFLAGS = -Wall -Wpedantic -std=c++11 
IFLAGS =  -I../HW2 -I../jansson/include
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS =  -L../HW2 -L../jansson/lib -lhw2 -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

all: hw4

hw4: $(SRC) $(HEADERS) $(LIBS)
	g++ $(CFLAGS) -g -O0 $(IFLAGS) $(GTKCFLAGS) -o hw4 $(SRC) $(LDFLAGS) $(GTKLDFLAGS)

clean:
	rm -f *.o *~ hw4
