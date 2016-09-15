all:
	gcc -o server ftps.c capitalFunctions.c
	gcc -o client ftpc.c capitalFunctions.c
	gcc -o tcpd tcpd.c

clean:
	rm server
	rm client
	rm tcpd