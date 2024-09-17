TARGET = a.out

OBJS = Engine.o Server.o Acceptor.o EventLoop.o InetAddress.o Socket.o SocketIO.o TaskQueue.o TcpConnection.o \
TcpServer.o ThreadPool.o Configuration.o KeyRecommander.o WebPageQuery.o

CC = g++

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -I ../include -g

Engine.o : Engine.cc
	$(CC) -c Engine.cc -o Engine.o

Server.o : ../src/Server.cc
	$(CC) -c ../src/Server.cc -o Server.o

Acceptor.o : ../src/Acceptor.cc
	$(CC) -c ../src/Acceptor.cc -o Acceptor.o

EventLoop.o : ../src/EventLoop.cc
	$(CC) -c ../src/EventLoop.cc -o EventLoop.o

InetAddress.o : ../src/InetAddress.cc
	$(CC) -c ../src/InetAddress.cc -o InetAddress.o

Socket.o : ../src/Socket.cc
	$(CC) -c ../src/Socket.cc -o Socket.o

SocketIO.o : ../src/SocketIO.cc
	$(CC) -c ../src/SocketIO.cc -o SocketIO.o

TaskQueue.o : ../src/TaskQueue.cc
	$(CC) -c ../src/TaskQueue.cc -o TaskQueue.o

TcpConnection.o : ../src/TcpConnection.cc
	$(CC) -c ../src/TcpConnection.cc -o TcpConnection.o

TcpServer.o : ../src/TcpServer.cc
	$(CC) -c ../src/TcpServer.cc -o TcpServer.o

ThreadPool.o : ../src/ThreadPool.cc
	$(CC) -c ../src/ThreadPool.cc -o ThreadPool.o

Configuration.o : ../src/Configuration.cc
	$(CC) -c ../src/Configuration.cc -o Configuration.o

KeyRecommander.o : ../src/KeyRecommander.cc
	$(CC) -c ../src/KeyRecommander.cc -o KeyRecommander.o

WebPageQuery.o : ../src/WebPageQuery.cc
	$(CC) -c ../src/WebPageQuery.cc -o WebPageQuery.o

clean:
	rm $(OBJS) $(TARGET)