

#Object files to build
OBJS=coinop-core.o \
     coinop-gpio.o \
     coinop-acceptor.o \
     coinop-switcher.o \
     coinop-server.o
  

coinop: $(OBJS) coinop.h coinop-config.h
	cc -lpthread -o coinop $(OBJS)

.c.o:
	cc -c $<

clean:
	rm -f $(OBJS) coinop
