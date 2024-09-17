TARGET = a.out

OBJS = testWeb.o  DirScanner.o PageLib.o tinyxml2.o Configuration.o

CC = g++

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm -I ../include -g

testWeb.o:testWeb.cc
	$(CC) -c testWeb.cc -o testWeb.o
DirScanner.o:../src/DirScanner.cc
	$(CC) -c ../src/DirScanner.cc -o DirScanner.o
PageLib.o:../src/PageLib.cc
	$(CC) -c ../src/PageLib.cc -o PageLib.o
tinyxml2.o:../../library/tinyxml/tinyxml2.cpp
	$(CC) -c ../../library/tinyxml/tinyxml2.cpp -o tinyxml2.o
Configuration.o:../src/Configuration.cc
	$(CC) -c ../src/Configuration.cc -o Configuration.o

clean:
	rm $(OBJS) $(TARGET)
