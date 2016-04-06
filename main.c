#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.c"

void print_usage() {
    printf("Usage: fourinarow [-p num] [-s num x num] \n");
}

void print_error(int n) {
    switch (n) {
        case 0:
            printf("The '_' character can not be used as a player symbol. Please re run using a different one \n");
            exit(EXIT_FAILURE);
        case 1:
            printf("Multiple declaration of same symbol. Note: Case is ignored. Please re run using unique symbols for each player \n");
            exit(EXIT_FAILURE);
        case 2:
            printf("Column must be in range [1, %d] \n > ", board_w);
            break;
        case 3:
            printf("Please enter a number \n > ");
            break;
        case 4:
            printf("Wrong Choice. Please enter 'd' to display the board or 'm' to do a move\n > ");
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

char *strlwr(char *str) {
    unsigned char *p = (unsigned char *)str;

    while (*p) {
        *p = tolower(*p);
        p++;
    }

    return str;
}

int is_string_unique(char *str) {
    int i = 0;
    int a = 0;
    while (str[i]) {
        a = i + 1;
        while (str[a]) {
            if (str[i] == str[a])
                return 0;
            a++;
        }
        i++;
    }
    return 1;
}

int * get_dimension(char * s) {
    char *size = strtok(s, "x");
    static int dim [2];
    int i = 0;
    while (size && i <=1 ) {
        dim [i] = atoi(size);
        i ++;
        size = strtok(NULL, "");
    }
    return dim;
}

struct player * get_turn(char * p) {

    struct player * turn = malloc(strlen(p) * sizeof(struct player));
    struct player * temp = turn;
    char * s = strdup(p);

    s = strlwr(s);


    // In case no player param was passed or either 2 default players
    if (strcmp(s, "ab") == 0) {
        p = "OX";
    }

    if (!is_string_unique(s)) {
        print_error(1);
    }

    for(; *p; ++p) {
        if (*p == '_') {
            print_error(0);
            exit(EXIT_FAILURE);
        }
        struct player * new_player = turn;
        new_player->id = *p;
        *turn = *new_player;
        turn++;
    }

    return temp;
}

int read_column(char id) {
    char buf[256];
    printf("Player %c enter a column number [1, %d]\n > ", id, board_w);
    while (fgets(buf, sizeof(buf), stdin)) {
        int column;
        if (sscanf(buf, "%d", &column) == 1) {
            if (column >= 1 && column <= board_w) {
                return column - 1;
            } else {
                print_error(2);
            }
        } else {
            print_error(3);
        }
    }
    return -1;
}

int read_choice(char id) {
    char buf[256];
    printf("Player %c enter 'd' to display the board or 'm' to do a move\n > ", id);
    while (fgets(buf, sizeof(buf), stdin)) {
        char choice;
        if (sscanf(buf, "%c", &choice) == 1) {
            switch(tolower(choice)) {
                case 'm':
                    return 1;
                case 'd':
                    return 0;
                default:
                    print_error(4);
            }
        }
    }
    return -1;
}

int main(int argc, char ** argv) {

    // parse the parameters
    int opt, * dim;
    extern int players_n, board_h, board_w;
    char * s,  *p = "AB";
    players_n = 2;
    board_h = 6;
    board_w = 7;
    struct player * players;

    while ((opt = getopt (argc, argv, "p:s:")) != -1) {
        switch (opt) {
            case 'p':
                // parse the string containing all the players
                p = optarg;
                break;
            case 's':
                // parse the string containing the board dimension
                s = optarg;
                dim = get_dimension(s);
                board_w = dim [0];
                board_h = dim [1];
                break;
            default: print_usage();
                exit(EXIT_FAILURE);
        }
    }

    players_n = strlen(p);
    players = get_turn(p);

    printf("\n\t\t  ---- FOUR IN A ROW ---- \n\n");
    printf("Number of players: %d\n", players_n);
    printf("Grid dimension: %d x %d\n", board_w, board_h);

    struct player * null_player = malloc(sizeof(struct player));
    null_player->id = '_';

    struct state * game = malloc(sizeof(struct state));
    game->current_player = null_player;
    game->status = PROGRESS;
    game->next = NULL;

    init_board(game);
    game_print(game);
    int i = 0;
    int change_turn;

    while (game_outcome(game) == PROGRESS) {
        struct move * m = malloc(sizeof(struct move));
        struct state * progress = malloc(sizeof(struct state));
        m->p = &players[i % players_n];
        game->current_player = m->p;

        printf("\n\t\t  ---- PLAYER %c TURN ---- \n\n", m->p->id);

        if (read_choice(m->p->id)) {
            if ((m->column = read_column(m->p->id)) != -1 && game_state_transition(game, m, progress)){
            game = progress;
            ++i;
            }
        }
        game_print(game);
    }

    switch (game->status) {
        case DRAW:
            printf("\n\t\t  ---- DRAW ---- \n\n");
            break;
        case WIN:
            printf("\n\t\t  ---- PLAYER %c WINS ---- \n\n", game->current_player->id);
            break;
        default:
            // this cannot happen
            exit(EXIT_FAILURE);
    }

    return 0;
}
