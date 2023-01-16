#
#	MAKEFILE
#
#

CC = gcc
CFLAGS = -g

#LIBS = -lsocket -lns -lnsl
LIBS = -lpthread
all: simulador monitor
# unix-stream-client unix-stream-server
monitor: monitor.o 
	$(CC) -g -o monitor monitor.o util.o $(LIBS)

simulador: simulador.o util.o
	$(CC) -g -o simulador simulador.o util.o $(LIBS)



#unix-stream-client: unix-stream-client.o util.o util-stream-client.o
#	gcc -o unix-stream-client unix-stream-client.o util.o util-stream-client.o 

#unix-stream-server: unix-stream-server.o util.o util-stream-server.o
#	gcc -o unix-stream-server unix-stream-server.o util.o util-stream-server.o 

clean:
	rm -f *.o *~ monitor simulador
