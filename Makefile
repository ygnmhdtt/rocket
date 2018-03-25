PROG := rocket
SRCS := main.c error.c ast.c
OBJS := $(SRCS:%.c=%.o)

CC := gcc

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<

clean:
	@rm -f $(PROG) *.o tmp.*
