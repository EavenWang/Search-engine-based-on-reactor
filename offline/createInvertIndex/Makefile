
a.out : main.o invertIndex.o Configuration.o tinyxml2.o
	g++ main.o invertIndex.o Configuration.o tinyxml2.o -o a.out -I ../include -g

main.o : main.cc
	g++ -c main.cc -o main.o

invertIndex.o : invertIndex.cc
	g++ -c invertIndex.cc -o invertIndex.o

Configuration.o : ../src/Configuration.cc
	g++ -c ../src/Configuration.cc -o Configuration.o

tinyxml2.o : ../../library/tinyxml/tinyxml2.cpp
	g++ -c ../../library/tinyxml/tinyxml2.cpp -o tinyxml2.o

clean:
	rm *.o $(TARGET)