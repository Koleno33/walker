#ifndef WALKER_H
#define WALKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>

#define EMPTY_SYMBOL   L' '
#define HERO_SYMBOL    L'o'
#define WALL_SYMBOL    L'X'
#define END_SYMBOL     L'.'
#define MINE_SYMBOL    L'\u2020'
#define FIELD_SIZE     25
#define WALL_CHANCE    FIELD_SIZE / 2
#define WALLS_STEP     6
#define MINES_COUNT    8

struct Position
{
  /* position from -(FIELD_SIZE/2)  to +(FIELD_SIZE/2)  */
	int x;
  int y;
};

enum PlayerDirection
{
	UP    = 'w',
	LEFT  = 'a',
	DOWN  = 's',
	RIGHT = 'd'
};

void fill_field(wchar_t **field, struct Position *player, struct Position *end);
void free_field(wchar_t **field);
void move_player(wchar_t **field, struct Position *player, enum PlayerDirection direction, struct Position *end);
void make_nextgame(wchar_t **field, struct Position *player, struct Position *end);
void make_defeat(wchar_t **field, struct Position *player, struct Position *end);

#endif
