all: main.c
		cc -c main.c game.c
		cc -o fourinarow main.o game.o
clean: 
		$(RM) fourinarow
		$(RM) main.o game.o
