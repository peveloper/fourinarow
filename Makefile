all: main.c
		cc -c main.c
		cc main.o -o fourinarow

clean: 
		$(RM) fourinarow
		$(RM) main.o
