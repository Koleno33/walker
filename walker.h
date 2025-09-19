#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define EMPTY_SYMBOL   ' '
#define HERO_SYMBOL    'o'
#define WALL_SYMBOL    'X'
#define END_SYMBOL     '.'
#define FIELD_SIZE     25
#define WALL_CHANCE    FIELD_SIZE / 2

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

void fill_field(char **field, struct Position *player, struct Position *end);
void free_field(char **field);
void move_player(char **field, struct Position *player, enum PlayerDirection direction, struct Position *end);
void make_nextgame(char **field, struct Position *player, struct Position *end);

