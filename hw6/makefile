SRC=../hw5/solution/Message.cc ../hw5/solution/MessageHandler.cc Game.cc SimpleEvaluator.cc Searcher.cc
HEADERS=../hw5/solution/Message.h ../hw5/solution/MessageHandler.h Game.h SimpleEvaluator.h Move.h Searcher.h
LIBS=../hw2/libhw2.a ../hw5/socketCode/libsocket.a
CFLAGS= -Wall -Wpedantic -std=c++11
IFLAGS=-I../hw2 -I../jansson/include -I../hw5/socketCode
GTKCFLAGS=`pkg-config --cflags gtk+-3.0`
LDFLAGS=-L../hw2 -L../jansson/lib -L../hw5/socketCode -lhw2 -l:libjansson.a -l:libsocket.a
GTKLDFLAGS=`pkg-config --libs gtk+-3.0`

all: hw6

hw6: hw6.cc $(SRC) $(HEADERS) $(LIBS)
	g++ $(CFLAGS) -g -O0 $(IFLAGS) $(GTKCFLAGS) -o hw6 hw6.cc $(SRC) $(LDFLAGS) $(GTKLDFLAGS)

clean:
	rm -f *.o *~ hw6
