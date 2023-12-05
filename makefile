CC = gcc

CFLAGS = -Wall -Wextra

SRCS = src/main.c src/chip8.c 

OBJS = $(SRCS:.c=.o)

chip8: $(OBJS)
	
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c

	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f chip8 $(OBJS)
