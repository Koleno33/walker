#include "walker.h"
#include <stdlib.h>

void make_nextgame(char **field, struct Position *player, struct Position *end)
{
  /* player start position */
	player->x = -(FIELD_SIZE/2) + 1;
	player->y = -(FIELD_SIZE/2) + 1;

	fill_field(field, player, end);
}

void fill_field(char **field, struct Position *player, struct Position *end)
{
	int i, j;
  char placed = 0;

  /* make top wall */
  for (i = 0; i < FIELD_SIZE; ++i) {
		field[i] = malloc((FIELD_SIZE + 1) * sizeof(**field));
    field[i][FIELD_SIZE] = '\0';
    field[0][i] = WALL_SYMBOL;
  }

	for (i = 1; i < FIELD_SIZE - 1; ++i) {
		for (j = 1; j < FIELD_SIZE - 1; ++j)
			field[i][j] = EMPTY_SYMBOL;

    /* make left & right walls */
    field[i][0] = WALL_SYMBOL;
    field[i][FIELD_SIZE - 1] = WALL_SYMBOL;
	}

  /* make bottom wall */
  for (i = 0; i < FIELD_SIZE; ++i) field[FIELD_SIZE - 1][i] = WALL_SYMBOL;

  /* make horizontal walls */
  for (i = 6; i < FIELD_SIZE - 2; i += 6) {
    for (j = 1; j < FIELD_SIZE - 1; ++j) {
      if (!placed && rand() % WALL_CHANCE == 0) {
        field[i][j] = EMPTY_SYMBOL;
        placed = 1;
      } 
      else {
        field[i][j] = WALL_SYMBOL;
      }
    }
    if (!placed) 
      field[i][FIELD_SIZE - 2] = EMPTY_SYMBOL;

    placed = 0;
  }

  /* make hero */
  field[player->y + FIELD_SIZE/2][player->x + FIELD_SIZE/2] = HERO_SYMBOL;

  /* make end */
  field[end->y + FIELD_SIZE/2][end->x + FIELD_SIZE/2] = END_SYMBOL;
}

void free_field(char **field)
{
	int i;
	for (i = 0; i < FIELD_SIZE; ++i) {
		free(field[i]);
	}
	
	free(field);
}

void move_player(char **field, struct Position *player, enum PlayerDirection direction, struct Position *end)
{
	int prev_x = player->x;
	int prev_y = player->y;
	
	switch (direction) {
		case UP:
			if (prev_y == -(FIELD_SIZE/2)) 
				return;
			player->y -= 1;
			break;
		case LEFT:
			if (prev_x == -(FIELD_SIZE/2)) 
				return;
			player->x -= 1;
			break;
		case DOWN:
			if (prev_y == FIELD_SIZE/2) 
				return;
      player->y += 1;
			break;
		case RIGHT:
			if (prev_x == FIELD_SIZE/2) 
				return;
			player->x += 1;
			break;
		default:
			return;
	}

  /* wall check */
  if (field[player->y + FIELD_SIZE/2][player->x + FIELD_SIZE/2] == WALL_SYMBOL) {
    player->x = prev_x;
    player->y = prev_y;
    return;
  }

  /* end check */
  if (field[player->y + FIELD_SIZE/2][player->x + FIELD_SIZE/2] == END_SYMBOL) {
    make_nextgame(field, player, end);
    return;
  }
	
	field[prev_y + FIELD_SIZE/2][prev_x + FIELD_SIZE/2] 	    = EMPTY_SYMBOL;
	field[player->y + FIELD_SIZE/2][player->x + FIELD_SIZE/2] = HERO_SYMBOL;
}

int main(int argc, char **argv) 
{
  srand(time(NULL));

	int i, c;
	char **field = malloc(sizeof(*field) * FIELD_SIZE);

	struct Position player;
	struct Position end;

  /* output preparations */
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);
	
  /* player start position */
	player.x = -(FIELD_SIZE/2) + 1;
	player.y = -(FIELD_SIZE/2) + 1;

  /* end position */
	end.x = FIELD_SIZE/2 - 1;
	end.y = FIELD_SIZE/2 - 1;

	fill_field(field, &player, &end);

  for (;;) {
    clear();
    for (i = 0; i < FIELD_SIZE; ++i) {
      mvprintw(i, 0, "%s", field[i]);
    }
    refresh();

    c = getch();
    if (c == 'q') break;

		move_player(field, &player, c, &end);
  }
	
	free_field(field);
  endwin();
  curs_set(1);
	
	return 0;
}

