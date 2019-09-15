all: main
.PHONY: all

clean:
	rm -rf *.o main

LiquidCrystal.o: LiquidCrystal_I2C.cpp LiquidCrystal_I2C.h
	g++ -std=c++11 -c LiquidCrystal_I2C.cpp

curl_common.o: curl_common.cpp curl_common.h
	g++ -std=c++11 -c curl_common.cpp

dht11.o: dht11.cpp dht11.h
	g++ -std=c++11 -c dht11.cpp

GrafanaModule.o: GrafanaModule.cpp GrafanaModule.h curl_common.o
	g++ -std=c++11 -I/usr/include/jsoncpp curl_common.o -c GrafanaModule.cpp -lcurl -ljsoncpp

button.o: button.cpp button.h
	g++ -std=c++11 -c button.cpp

ourhome.o: curl_common.o ourhome.cpp ourhome.h
	g++ -std=c++11 `pkg-config glib-2.0 --cflags --libs` -I/usr/include/jsoncpp -c ourhome.cpp

main.o: main.cpp ourhome.o LiquidCrystal.o button.o GrafanaModule.o curl_common.o
	g++ -std=c++11 -c main.cpp

main: main.o
	g++ -std=c++11 -lcurl -ljsoncpp -lglib-2.0 -lwiringPi *.o -o main
