
all: main
.PHONY: all

clean:
	rm -rf *.o main unaccent

ourhome.o: ourhome.cpp ourhome.h 
	g++ -ggdb -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/jsoncpp -c ourhome.cpp

main.o: main.cpp ourhome.o
	g++ -ggdb -c main.cpp

main: main.o
	g++ -ggdb -lcurl -ljsoncpp -lglib-2.0 *.o -o main
