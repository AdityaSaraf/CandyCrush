SRC=Message.cc MessageHandler.cc
HEADERS=Message.h MessageHandler.h
LIBS=../hw2/libhw2.a ./socketCode/libsocket.a
CFLAGS=-Wall -Wpedantic -std=c++11
IFLAGS=-I../hw2 -I../jansson/include -I./socketCode
GTKCFLAGS=`pkg-config --cflags gtk+-3.0`
LDFLAGS=-L../hw2 -L../jansson/lib -L./socketCode -lhw2 -l:libjansson.a -l:libsocket.a
GTKLDFLAGS=`pkg-config --libs gtk+-3.0`

all: hw5-view hw5-model

hw5-view: hw5-view.cc $(SRC) $(HEADERS) $(LIBS)
	g++ $(CFLAGS) -g -O0 $(IFLAGS) $(GTKCFLAGS) -o hw5-view hw5-view.cc $(SRC) $(LDFLAGS) $(GTKLDFLAGS)

hw5-model: hw5-model.cc $(SRC) $(HEADERS) $(LIBS)
	g++ $(CFLAGS) -g -O0 $(IFLAGS) -o hw5-model hw5-model.cc $(SRC) $(LDFLAGS)

clean:
	rm -f *.o *~ hw5-view hw5-model
