
CC= gcc

CFLAGS = $(shell pkg-config gtk+-3.0 --cflags)
LFLAGS = $(shell pkg-config gtk+-3.0 --libs) -lm

SRC = game.c game.h instruction.c instruction.h car.c car.h main.c

OBJS = game.o instruction.o car.o main.o

NAME = race

YOU : $(SRC) $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)
	 
	 
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LFLAGS)

clean:
	rm -f $(OBJS) $(NAME)

