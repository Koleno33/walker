#include "game.h"

void make_nextgame(Game *game)
{
  /* player start position */
  game->playerpos->x = -(FIELD_SIZE/2) + 1;
  game->playerpos->y = -(FIELD_SIZE/2) + 1;

  fill_field(game);
}

void make_defeat(Game *game)
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

  make_nextgame(game);
}

void fill_field(Game *game)
{
  wchar_t *temp_symb_ptr = NULL;
  int     i, j, next_x_index, next_y_index;

  /* make top wall */
  for (i = 0; i < FIELD_SIZE; ++i) {
    game->field[i][FIELD_SIZE] = L'\0';
    game->field[0][i] = WALL_SYMBOL;
  }

  for (i = 1; i < FIELD_SIZE - 1; ++i) {
    for (j = 1; j < FIELD_SIZE - 1; ++j)
      game->field[i][j] = EMPTY_SYMBOL;

    /* make left & right walls */
    game->field[i][0] = WALL_SYMBOL;
    game->field[i][FIELD_SIZE - 1] = WALL_SYMBOL;
  }

  /* make bottom wall */
  for (i = 0; i < FIELD_SIZE; ++i) game->field[FIELD_SIZE - 1][i] = WALL_SYMBOL;

  /* make other walls */
  for (i = WALLS_STEP; i < FIELD_SIZE - 2; i += WALLS_STEP) {
    for (j = 1; j < FIELD_SIZE - 1; ++j) {
      /* horizontal */
      game->field[i][j] = WALL_SYMBOL;
      /* vertical */
      game->field[j][i] = WALL_SYMBOL;
    }
  }

  /* make hero */
  game->field[game->playerpos->y + FIELD_SIZE/2][game->playerpos->x + FIELD_SIZE/2] = HERO_SYMBOL;

  /* make end */
  game->field[game->endpos->y + FIELD_SIZE/2][game->endpos->x + FIELD_SIZE/2] = END_SYMBOL;

  /* make mines */
  i = 0;
  while (i < MINES_COUNT) {
    /* trying to get horizontal coordinate not with wall or hole */
    next_x_index = 2 + rand() % (FIELD_SIZE - 2);
    if (next_x_index % WALLS_STEP == 0) continue;

    /* trying to get vertical coordinate not with wall or hole */
    next_y_index = 2 + rand() % (FIELD_SIZE - 2);
    if (next_y_index % WALLS_STEP == 0) continue;

    temp_symb_ptr = &game->field[next_y_index][next_x_index];
    if (*temp_symb_ptr != WALL_SYMBOL && *temp_symb_ptr != END_SYMBOL) {
      *temp_symb_ptr = MINE_SYMBOL;
      ++i;
    }
  }

  make_doors(game);
  add_doors(game);
}

void make_doors(Game *game)
{
  int hole_placed = 0;
  int i, j;

  /* amount of holes if go horizontally or vertically*/
  int holesh = 0; 
  int holesv = 0;

  /* is door placed in horizontal direction or no */
  int is_hor = 0;

  for (i = 0; i < WAY_LENGTH; ++i) {
    is_hor = rand() % 2;
    if ((holesh < (WAY_LENGTH / 2) && (is_hor || holesv >= (WAY_LENGTH / 2)))) {
      /* horizontal */
      for (j = (holesv) * WALLS_STEP + 1; j < (holesv + 1) * WALLS_STEP - 1; ++j) {
        if ( game->field[j][(holesh + 1) * WALLS_STEP - 1] == MINE_SYMBOL || 
             game->field[j][(holesh + 1) * WALLS_STEP + 1] == MINE_SYMBOL ) {
          continue;
        }

        if (rand() % WALL_CHANCE == 0) {
          game->field[j][(holesh + 1) * WALLS_STEP] = EMPTY_SYMBOL;
          hole_placed = 1;
          break;
        }
      }

      if (!hole_placed) {
        game->field[j][(holesh + 1) * WALLS_STEP] = EMPTY_SYMBOL;

        if (game->field[j][(holesh + 1) * WALLS_STEP - 1] == MINE_SYMBOL) {
          game->field[j][(holesh + 1) * WALLS_STEP - 1] = EMPTY_SYMBOL;
        }
        if (game->field[j][(holesh + 1) * WALLS_STEP + 1] == MINE_SYMBOL) {
          game->field[j][(holesh + 1) * WALLS_STEP + 1] = EMPTY_SYMBOL;
        }
      }

      ++holesh;
    }
    else {
      /* vertical */
      for (j = (holesh) * WALLS_STEP + 1; j < (holesh + 1) * WALLS_STEP - 1; ++j) {
        if ( game->field[(holesv + 1) * WALLS_STEP - 1][j] == MINE_SYMBOL || 
             game->field[(holesv + 1) * WALLS_STEP + 1][j] == MINE_SYMBOL ) {
          continue;
        }

        if (rand() % WALL_CHANCE == 0) {
          game->field[(holesv + 1) * WALLS_STEP][j] = EMPTY_SYMBOL;
          hole_placed = 1;
          break;
        }
      }

      if (!hole_placed) {
        game->field[(holesv + 1) * WALLS_STEP][j] = EMPTY_SYMBOL;

        if (game->field[(holesv + 1) * WALLS_STEP - 1][j] == MINE_SYMBOL) {
          game->field[(holesv + 1) * WALLS_STEP - 1][j] = EMPTY_SYMBOL;
        }
        if (game->field[(holesv + 1) * WALLS_STEP + 1][j] == MINE_SYMBOL) {
          game->field[(holesv + 1) * WALLS_STEP + 1][j] = EMPTY_SYMBOL;
        }
      }

      ++holesv;
    }

    hole_placed = 0;
  }
}


void add_doors(Game *game)
{
  int i, j;
  int hole_shift, room_start;

  /* for horizontal walls */
  for (i = WALLS_STEP; i < FIELD_SIZE - WALLS_STEP; i += WALLS_STEP) {
    room_start = 0;
    for (j = 1; j < FIELD_SIZE; ++j) {
      if (j % WALLS_STEP == 0) {
        hole_shift = 1 + rand() % (WALLS_STEP - 1);
        if (rand() % ADD_WALL_CHANCE == 0) {
          if (room_start + hole_shift >= FIELD_SIZE - 1) {
            break;
          }
          game->field[i][room_start + hole_shift] = EMPTY_SYMBOL;
        }
        room_start = j;
      }

      if (game->field[i][j] == EMPTY_SYMBOL) {
        /* hole found */
        j = room_start + WALLS_STEP; 
        room_start = j;
      }
    }
  }

  /* for vertical walls */
  for (i = WALLS_STEP; i < FIELD_SIZE - WALLS_STEP; i += WALLS_STEP) {
    room_start = 0;
    for (j = 1; j < FIELD_SIZE; ++j) {
      if (j % WALLS_STEP == 0) {
        hole_shift = 1 + rand() % (WALLS_STEP - 1);
        if (rand() % ADD_WALL_CHANCE == 0) {
          if (room_start + hole_shift >= FIELD_SIZE - 1) {
            break;
          }
          game->field[room_start + hole_shift][i] = EMPTY_SYMBOL;
        }
        room_start = j;
      }

      if (game->field[j][i] == EMPTY_SYMBOL) {
        /* hole found */
        j = room_start + WALLS_STEP; 
        room_start = j;
      }
    }
  }
}

void free_field(Game *game)
{
  int i;
  for (i = 0; i < FIELD_SIZE; ++i) {
    free(game->field[i]);
  }
  
  free(game->field);
}

void move_player(Game *game, enum PlayerDirection direction)
{
  int prev_x = game->playerpos->x;
  int prev_y = game->playerpos->y;
  
  switch (direction) {
    case UP:
      if (prev_y == -(FIELD_SIZE/2)) 
        return;
      game->playerpos->y -= 1;
      break;
    case LEFT:
      if (prev_x == -(FIELD_SIZE/2)) 
        return;
      game->playerpos->x -= 1;
      break;
    case DOWN:
      if (prev_y == FIELD_SIZE/2) 
        return;
      game->playerpos->y += 1;
      break;
    case RIGHT:
      if (prev_x == FIELD_SIZE/2) 
        return;
      game->playerpos->x += 1;
      break;
    default:
      return;
  }

  /* wall check */
  if (game->field[game->playerpos->y + FIELD_SIZE/2][game->playerpos->x + FIELD_SIZE/2] == WALL_SYMBOL) {
    game->playerpos->x = prev_x;
    game->playerpos->y = prev_y;
    return;
  }

  /* end check */
  if (game->field[game->playerpos->y + FIELD_SIZE/2][game->playerpos->x + FIELD_SIZE/2] == END_SYMBOL) {
    make_nextgame(game);
    return;
  }

  /* mine check */
  if (game->field[game->playerpos->y + FIELD_SIZE/2][game->playerpos->x + FIELD_SIZE/2] == MINE_SYMBOL) {
    make_defeat(game);
    return;
  }
  
  game->field[prev_y + FIELD_SIZE/2][prev_x + FIELD_SIZE/2]                         = EMPTY_SYMBOL;
  game->field[game->playerpos->y + FIELD_SIZE/2][game->playerpos->x + FIELD_SIZE/2] = HERO_SYMBOL;
}

