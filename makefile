CC=gcc
INCLUDE=`zx11-config -I`
LIB=`zx11-config -L`

CFLAGS=-Wall -O3 -funroll-loops $(INCLUDE) $(LIB)

LINK=-lcairo `zx11-config -l` -lpthread

TARGET=hos

all: $(TARGET)
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean :
	rm -f *.o *~ core $(TARGET)
