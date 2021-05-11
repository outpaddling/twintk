
PREFIX  ?= /usr/local
LOCALBASE       ?= /usr/local
OBJ = atibus.o microsoft.o mouselib.o mousesys.o scobus.o tmouse.o ps2.o
CFLAGS  += -Wall -I${LOCALBASE}/include -I.

all:    tmouse

tmouse: $(OBJ)
	$(CC) -o tmouse $(OBJ) -L.. -ltwin -L${LOCALBASE}/lib -lxtend

%.o:    %.c ../Image/image.h ../Terminal/direct.h ../Windows/windows.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f *.o tmouse
