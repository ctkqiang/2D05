run:
	- clang++ -std=c++11 -o bin/2D05 main.cpp src/Utilities/*.cpp src/Controller/*.cpp -lcurl -lsqlite3 && ./bin/2D05

clean:
	- rm -rf bin/*

build:
	- clang++ -std=c++11 -o bin/2D05 main.cpp src/Controller/*.cpp src/Utilities/*.cpp  -lcurl -lsqlite3
