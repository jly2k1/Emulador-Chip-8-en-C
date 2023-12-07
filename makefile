CC = gcc
CFLAGS = -c -Wall -Wextra
LDFLAGS = -L/usr/lib -lSDL2

SRCS = src/main.c src/chip8.c 
OBJS = $(SRCS:.c=.o)

chip8: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

debug: 
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"

clean:
	rm -f chip8 $(OBJS)

