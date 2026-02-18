#ifndef WALKER_H
#define WALKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>

#define EMPTY_SYMBOL     L' '
#define HERO_SYMBOL      L'o'
#define WALL_SYMBOL      L'X'
#define END_SYMBOL       L'.'
#define MINE_SYMBOL      L'\u2020'
#define TEST_SYMBOL      L'T'
#define FIELD_SIZE       49
#define WALLS_STEP       6
#define WALL_CHANCE      (FIELD_SIZE / WALLS_STEP)
#define ADD_WALL_CHANCE  2
#define WAY_LENGTH       ((int)((FIELD_SIZE / WALLS_STEP) - 1) * 2)
#define WALLS_TO_ADD     WALL_CHANCE - 1
#define MINES_COUNT      8

typedef struct
{
  /* position from -(FIELD_SIZE/2)  to +(FIELD_SIZE/2)  */
  int x;
  int y;
} Position;

enum PlayerDirection
{
  UP    = 'w',
  LEFT  = 'a',
  DOWN  = 's',
  RIGHT = 'd'
};

typedef struct
{ 
  Position  *playerpos;
  Position  *endpos;
  wchar_t  **field;
  unsigned   player_score;
} Game;

void fill_field(Game *game);
void make_doors(Game *game);
void add_doors(Game *game);
void free_field(Game *game);
void move_player(Game *game, enum PlayerDirection direction);
void make_nextgame(Game *game);
void make_defeat(Game *game);

#endif
