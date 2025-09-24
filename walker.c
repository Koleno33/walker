#include "walker.h"

void make_nextgame(wchar_t **field, struct Position *player, struct Position *end)
{
  /* player start position */
	player->x = -(FIELD_SIZE/2) + 1;
	player->y = -(FIELD_SIZE/2) + 1;

	fill_field(field, player, end);
}

void make_defeat(wchar_t **field, struct Position *player, struct Position *end)
{
  const char *defeat_msg = "Death has overtaken you...";
  const char *hint_msg   = "Press any key to try again.";
  int max_y, max_x, start_y, start_x;

  getmaxyx(stdscr, max_y, max_x);
  if (strlen(hint_msg) > max_x) {
    start_y = 0;
    start_x = 0;
  }
  else {
    start_y = max_y / 2;
    start_x = (max_x - strlen(hint_msg)) / 2;
  }

  clear();

  mvprintw(start_y,     start_x, "%s", defeat_msg);
  mvprintw(start_y + 1, start_x, "%s", hint_msg);

  refresh();
  getch();

  make_nextgame(field, player, end);
}

void fill_field(wchar_t **field, struct Position *player, struct Position *end)
{
  wchar_t *temp_symb_ptr = NULL;
	int     i, j, next_x_index, next_y_index;
  char    placed = 0;

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

  /* make other walls */
  for (i = WALLS_STEP; i < FIELD_SIZE - 2; i += WALLS_STEP) {
    /* horizontal */
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

  /* make mines */
  i = 0;
  while (i < MINES_COUNT) {
    /* trying to get horizontal coordinate not with wall or hole */
    next_x_index = 2 + rand() % (FIELD_SIZE - 2);
    if (next_x_index % WALLS_STEP == 0) continue;

    /* trying to get vertical coordinate not with wall or hole */
    next_y_index = 2 + rand() % (FIELD_SIZE - 2);
    if (next_y_index % WALLS_STEP == 0) continue;

    temp_symb_ptr = &field[next_y_index][next_x_index];
    if (*temp_symb_ptr != WALL_SYMBOL) {
      *temp_symb_ptr = MINE_SYMBOL;
      ++i;
    }
  }

}

void free_field(wchar_t **field)
{
	int i;
	for (i = 0; i < FIELD_SIZE; ++i) {
		free(field[i]);
	}
	
	free(field);
}

void move_player(wchar_t **field, struct Position *player, enum PlayerDirection direction, struct Position *end)
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

  /* mine check */
  if (field[player->y + FIELD_SIZE/2][player->x + FIELD_SIZE/2] == MINE_SYMBOL) {
    make_defeat(field, player, end);
    return;
  }
	
	field[prev_y + FIELD_SIZE/2][prev_x + FIELD_SIZE/2] 	    = EMPTY_SYMBOL;
	field[player->y + FIELD_SIZE/2][player->x + FIELD_SIZE/2] = HERO_SYMBOL;
}

int main(void) 
{
  setlocale(LC_ALL, "");
  srand(time(NULL));

	int i, c;
	wchar_t **field = malloc(sizeof(*field) * FIELD_SIZE);

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
      mvprintw(i, 0, "%ls", field[i]);
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

