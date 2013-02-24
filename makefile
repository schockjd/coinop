
coinop: coinop-core.o coinop-acceptor.o coinop-server.o coinop-gpio.o
	cc -lpthread -o coinop coinop-core.o coinop-acceptor.o coinop-server.o

.c.o:
	cc -c $<

clean:
	rm *.o
	rm coinop
