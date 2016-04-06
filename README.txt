Stefano Peverelli - System Programming - Homework 01

Files:

- game.h
- game.c
- main.c
- Makefile

Usage:

$ make
$ ./fourinarow [-p players] [-s columns x rows]

Implementation:

Once I have read the parameters describing the players and the grid
I assigned those values to global variables in order to be access
them whenever I need to.

Each state will handle who is the current player, the board configuration,
the game status (which is an enumerator [PROGRESS, DRAW, WIN]), and a pointer
to the next state, in order to maintain an history of different game states.

The board is a matrix of moves, each move contains the pointer to the corresponding
player and the column in which the piece is inserted into the board. The board
dimensions are passed by command line or are just simply default values (7 x 6).
Note: The game accept even dimension that don't allow victory in a game, like:
      0x0, or 1x1 etc.. obviously this game will end in a draw.

A player is uniquely described by its own char id, which is assigned either by
command line or by default (O and X).
Note: Case is ignored when passing the players (each player id must be different).

References and external material:

http://www.gnu.org/software/libc/manual/html_node/Getopt.html

I have used getopt to read command line parameters in a more convenient way.

NOTE: more details and comments are left in game.h for better understanding.
