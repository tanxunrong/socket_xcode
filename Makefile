Common_cflags= -Wall -g -std=c99
Glib_cflags := $(shell pkg-config --cflags glib-2.0)
Event_cflags := $(shell pkg-config --cflags libevent)
CFLAGS=$(Common_cflags) $(Glib_cflags) $(Event_cflags) 

Objs = echo_client.o echo_server.o glibtest.o
Targets = $(patsubst %.o,%,$(Objs))

install : $(Objs)
	if [ ! -d objs ]; then mkdir objs ; fi;
	cd objs/ ; \
	cc -o echo_client echo_client.o ;\
	cc -o echo_server echo_server.o -levent -lglib-2.0;\
	cc -o glibtest glibtest.o -lglib-2.0

$(Objs) : include/common.h

$(Objs) : %.o : %/main.c
	cc -c $< $(CFLAGS) -o objs/$@

clean:
	rm -rf objs
