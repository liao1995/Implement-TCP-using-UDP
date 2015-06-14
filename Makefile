all:	server client

server: server.o mytcp.o
	cc server.o mytcp.o -o server

client: client.o mytcp.o
	cc client.o mytcp.o -o client

server.o: server.c
	cc -c server.c

client.o: client.c
	cc -c client.c

mytcp.o: mytcp.c mytcp.h
	cc -c mytcp.c mytcp.h

clean:
	rm *.o
	rm server
	rm client
