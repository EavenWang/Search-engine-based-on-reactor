TARGET = testDict

OBJS = testDict.o  DictProducer.o DirScanner.o Configuration.o

CC = g++

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -I ../include

testDict.o:testDict.cc
	$(CC) -c testDict.cc -o testDict.o
DictProducer.o:../src/DictProducer.cc
	$(CC) -c ../src/DictProducer.cc -o DictProducer.o
DirScanner.o:../src/DirScanner.cc
	$(CC) -c ../src/DirScanner.cc -o DirScanner.o
Configuration.o:../src/Configuration.cc
	$(CC) -c ../src/Configuration.cc -o Configuration.o

clean:
	rm $(OBJS) $(TARGET)
