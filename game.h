#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

int board_h;
int board_w;
int players_n;

/* The game status.
 * Simply enumerator for the possible situations during the game.
*/
enum game_status {
    PROGRESS,
    DRAW,
    WIN,
};

/* A player.
 * Described simply by a character (matching the input one).
*/
struct player {
    char id;
};

/* A move.
 * Simply contains the column, and the reference to the player.
*/
struct move {
    int column;
    struct player * p;
};

/* The state of a game.
 * Handles the current player, the board configuration, the game status,
 * and a pointer to the next state.
 * Note: each board's cell is a move, even an empty space;
 *       the empty space has a char id equal to the _ character.
*/
struct state {
    struct player * current_player;
    struct move *** board;
    enum game_status status;
    const struct state * next;
};

/* Initialize the board.
 * Each cell is initialize by a move pointer, when empty in ASCII simply a '_'.
*/
void init_board(struct state * s);

/* Display the state of a game by drawing the grid on the terminal
 * using ASCII characters.
*/
void game_print(const struct state * s);


/* Check whether a specific move is compatible with the given game
 * state.
*/
int move_is_valid(const struct move * m, const struct state * s);


/* Executes move m on the game state pointed to by s. If the move is
 * legal in state s then update s and return a non-zero result.
 * Otherwise return zero without changing state s.
*/
int game_update(struct state * s, const struct move * m);


/* Apply move m to game state s0 and writes the state of the game
 * after the move into s1. Return non-zero upon success, or zero if
 * the move is not valid in s0.
*/
int game_state_transition(const struct state * s0, const struct move * m, struct state * s1);


/* Evaluates the given game state. The return value must indicate
 * whether the game is still undetermined or whether the game is
 * over, and in that case which player won the game or whether the
 * game is a tie.
*/
int game_outcome(struct state * s);

#endif
