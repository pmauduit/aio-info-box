all: main
.PHONY: all

clean:
	rm -rf *.o main

LiquidCrystal.o: LiquidCrystal_I2C.cpp LiquidCrystal_I2C.h
	g++ -c LiquidCrystal_I2C.cpp

curl_common.o: curl_common.cpp curl_common.h
	g++ -c curl_common.cpp

GrafanaModule: GrafanaModule.cpp GrafanaModule.h curl_common.o
	g++ -std=c++11 -o GrafanaModule GrafanaModule.cpp

button.o: button.cpp button.h
	g++ -c button.cpp

ourhome.o: curl_common.o ourhome.cpp ourhome.h
	g++ -ggdb `pkg-config glib-2.0 --cflags --libs` -I/usr/include/jsoncpp -c ourhome.cpp

main.o: main.cpp ourhome.o LiquidCrystal.o button.o curl_common.o
	g++ -ggdb -c main.cpp

main: main.o
	g++ -ggdb -lcurl -ljsoncpp -lglib-2.0 -lwiringPi *.o -o main
