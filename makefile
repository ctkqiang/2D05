run:
	- clang++ -std=c++11 -o bin/2D05 main.cpp src/Controller/TrafficFlood.cpp src/Utilities/logger.cpp -lcurl && ./bin/2D05

clean:
	- rm -rf bin/*

build:
	- clang++ -std=c++11 -o bin/2D05 main.cpp src/Controller/TrafficFlood.cpp src/Utilities/logger.cpp -lcurl
