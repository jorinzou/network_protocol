CC=gcc

OBJ=server
all:$(OBJ)

HERE=$(shell pwd)
LIB_EVENT_DIR=$(HERE)/libevent-2.0.22-stable/.libs/

incs=-I$(HERE)
incs+=-I$(HERE)/libevent-2.0.22-stable/include/event2/
incs+=-I$(HERE)/libevent-2.0.22-stable/include/

SRC=$(HERE)/src/
files:=$(foreach dir,$(SRC),$(wildcard $(SRC)/*.c))

OBJS_C := $(patsubst %.c,%.o,$(files))

CFLAGS=-O0 -g 
CFLAGS+=$(incs) 

LDFLAGS:=-lpthread 
LDFLAGS+=-lm
LDFLAGS+=-ldl
LDFLAGS+=-L$(LIB_EVENT_DIR) -levent

$(OBJ):$(OBJS_C)
	$(CC)  $(LDFLAGS) -o $@ $^ 
			
$(OBJS_C):%.o:%.c
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	rm -rf $(HERE)/*.o
	rm -rf $(HERE)/$(OBJ)

