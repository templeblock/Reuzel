Reuzel: test.o Thread.o ThreadPool.o
	g++ -o Reuzel test.o Thread.o ThreadPool.o -lpthread
	rm *.o
test.o: ./test/test.cpp ./src/ThreadPool.h
	g++ -c -std=c++11 ./test/test.cpp
Thread.o: ./src/Thread.cpp
	g++ -c -std=c++11 ./src/Thread.cpp
ThreadPool.o: ./src/ThreadPool.cpp ./src/Thread.h
	g++ -c -std=c++11 ./src/ThreadPool.cpp
