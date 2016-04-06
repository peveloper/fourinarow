#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_board(struct state * s) {
    extern int board_h, board_w;
    s->board =  malloc(sizeof(struct move *) * board_h);
    for (int i = 0; i < board_h; i++)
        s->board[i] = malloc(sizeof(struct move) * board_w);
    for (int i=0; i < board_h; i++) {
        for(int j = 0; j < board_w; j++) {
            s->board[i][j] = malloc(sizeof(struct move));
            s->board[i][j]->p = malloc(sizeof(struct player));
            s->board[i][j]->p->id = s->current_player->id;
        }
    }
}

int is_column_full(int column, const struct state * s) {
    for (int j=0; j < board_h; j++){
        if(s->board[j][column]->p->id == '_') return 0;
    }
    return 1;
}

void insert(const struct move * m, const struct state * s) {
    for (int j=0; j < board_h; j++) {
        if(s->board[j][m->column]->p->id == '_') {
            s->board[j][m->column]->p->id = m->p->id;
            break;
        }
    }
}

void game_print(const struct state * s) {
    extern int board_h, board_w;
    printf("\n");
    for (int i=board_h-1; i >= 0; i--) {
        for(int j=0; j < board_w ; j++)
            printf("%c  ", s->board[i][j]->p->id);
        printf("\n");
    }
    printf("\n");
}

int move_is_valid(const struct move * m, const struct state * s) {
    if (m->column < board_w && m->column >= 0)
        if (is_column_full(m->column, s) == 0)
            return 1;
    return 0;
}

int game_update(struct state * s, const struct move * m) {
    if (move_is_valid(m, s)){
        insert(m,s);
        return 1;
    }
    return 0;
}

int game_state_transition(const struct state * s0, const struct move * m, struct state * s1) {
    memcpy(s1, s0, sizeof(struct state));
    if (game_update(s1, m)){
        s1->next = s0;
        return 1;
    }
    return 0;
}

int is_valid_coordinate(int w, int h) {
    return 0 <= w && w < board_h
        && 0 <= h && h < board_w;
}

int check_draw(struct state * s) {
    int draw = 1;
    for (int i=0; i < board_w; i++)
        if (s->board[board_h - 1][i]->p->id == '_') return 0;
    s->status = DRAW;
    return 1;
}

int check_win(struct state * s) {
    for (int i=0; i < board_h; i++) {
        for (int j=0; j < board_w; j++) {

            char symbol = s->board[i][j]->p->id;

            if (symbol == '_') continue;

            int h, w;

            for(w=-1; w <= 1; ++w) {
                for(h=-1; h <= 1; ++h) {
                    int delta;
                    int win;

                    if (w == 0 && h == 0) continue;

                    win = 1;

                    for(delta=1; delta < 4; ++delta) {
                        if ((!is_valid_coordinate(i + h * delta, j + w * delta) || symbol != s->board[i + h * delta][j + w * delta]->p->id)){
                            win = 0;
                            break;
                        }
                    }

                    if (win) {
                        s->status = WIN;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int game_outcome(struct state * s) {
    if (s->status != PROGRESS) return -1;
    if (check_draw(s)) return DRAW;
    if (check_win(s)) return WIN;
    return 0;
}

