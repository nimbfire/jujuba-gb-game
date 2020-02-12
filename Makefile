CC	= ../bin/lcc -Wa-l -Wl-m

CFLAGS	= -DGBDK_2_COMPAT

BINS	= game.gb

all:	$(BINS)

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:	%.s
	$(CC) $(CFLAGS) -c -o $@ $<

%.s:	%.c
	$(CC) $(CFLAGS) -S -o $@ $<

%.gb:	%.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.gb

# Link file, and write 0x80 at position 0x143 in header
game.gb:	game.o
	$(CC) $(CFLAGS) -Wl-yp0x143=0x80 -o game.gb game.o