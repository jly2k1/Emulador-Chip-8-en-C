CC = gcc

CFLAGS = -g -Wall -Wextra

SRCS = src/main.c src/chip8.c 

OBJS = $(SRCS:.c=.o)

chip8: $(OBJS)
	
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c

	$(CC) $(CFLAGS) -c -o $@ $<

debug: 
    @echo "CC: $(CC)"
    @echo "CFLAGS: $(CFLAGS)"
    @echo "SRCS: $(SRCS)"
    @echo "OBJS: $(OBJS)"

clean:
	rm -f chip8 $(OBJS)
