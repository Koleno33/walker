#include "game.h"

int main(void) 
{
  int i, c;
  Game game;

  wchar_t **field = malloc(sizeof(*field) * FIELD_SIZE);

  Position player;
  Position end;

  setlocale(LC_ALL, "");
  srand(time(NULL));

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

  game.field = field;
  game.playerpos = &player;
  game.endpos = &end;

  for (i = 0; i < FIELD_SIZE; ++i) 
   field[i] = malloc((FIELD_SIZE + 1) * sizeof(**field));
  fill_field(&game);

  for (;;) {
    clear();
    for (i = 0; i < FIELD_SIZE; ++i) {
      mvprintw(i, 0, "%ls", field[i]);
    }
    refresh();

    c = getch();
    if (c == 'q') break;

    move_player(&game, c);
  }
  
  free_field(&game);
  endwin();
  curs_set(1);
  
  return 0;
}

