OBJS	=main.o TinyWareHouse.o Prompt.o
SOURCE	=main.c TinyWareHouse.c Prompt.c
HEADER	=TinyWareHouse.h Promt.h
OUT	=twa
CC	=gcc
FLAGS	=-g3 -c
MATH	=-lm

all: $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(MATH)
main.o: main.c
	$(CC) $(FLAGS) main.c
TinyWareHouse.o: TinyWareHouse.c
	$(CC) $(FLAGS) TinyWareHouse.c
Prompt.o: Prompt.c
	$(CC) $(FLAGS) Prompt.c
