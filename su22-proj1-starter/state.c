#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);


/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t *back = (game_state_t *)malloc(sizeof(game_state_t));
	back->num_rows = 18;
	back->board = (char **)malloc(18 * sizeof(char *));
	back->board[0] = (char *)malloc(21 * sizeof(char));
	back->board[17] = (char *)malloc(21 * sizeof(char));
	const char a[21] = "####################\0";
	const char b[21] = "# d>D    *         #\0";
	const char c[21] = "#                  #\0";
	strcpy(back->board[0], a);
	strcpy(back->board[17], a);
	for (int i = 1; i < 17; i++)
	{
		back->board[i] = (char *)malloc(21 * sizeof(char));
		if (i == 2)
			strcpy(back->board[i], b);
		else
			strcpy(back->board[i], c);
	}
	back->num_snakes = 1;
	back->snakes = (snake_t *)malloc(sizeof(snake_t));
	back->snakes->tail_x = 2;
	back->snakes->tail_y = 2;
	back->snakes->head_x = 4;
	back->snakes->head_y = 2;
	back->snakes->live = 1;
	return back;
}


/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  for(int i = 0; i < state->num_rows; i++)
    free(state->board[i]);
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for(int i = 0; i < state->num_rows;i++)
    if(fp && state)
      fprintf(fp,"%s\n",state->board[i]);
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int x, unsigned int y) {
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch) {
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  const char test[5] = "wasd\0";
  for(int i = 0;i < 4; i++)
    if(c == test[i])
      return true;
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  const char test[6] = "WASDx\0";
  for(int i = 0;i < 5; i++)
    if(c == test[i])
      return true;
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>vWASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  const char test[14] = "wasd^<>vWASDx\0";
  for(int i = 0;i < 13; i++)
    if(c == test[i])
      return true;
  return false;
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  const char test[5] = "^<v>\0";
  if (c == test[0])
    return 'w';
  else if(c == test[1])
    return 'a';
  else if(c == test[2])
    return 's';
  else if(c == test[3])
    return 'd';
  return c;
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  const char test[5] = "WASD\0";
  if (c == test[0])
    return '^';
  else if(c == test[1])
    return '<';
  else if(c == test[2])
    return 'v';
  else if(c == test[3])
    return '>';
  return c;
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c) {
  // TODO: Implement this function.
  if(c == '>' || c == 'd' || c == 'D')
    return cur_x + 1;
  if(c == '<' || c == 'a' || c == 'A')
    return cur_x - 1;
  return cur_x;
}

/*
  Returns cur_y + 1 if c is '^' or 'w' or 'W'.
  Returns cur_y - 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c) {
  // TODO: Implement this function.
  if(c == '^' || c == 'w' || c == 'W')
    return cur_y - 1;
  if(c == 'v' || c == 's' || c == 'S')
    return cur_y + 1;
  return cur_y;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *t = &state->snakes[snum];
  char next = get_board_at(state, t->head_x, t->head_y);
  unsigned int next_x = get_next_x(t->head_x, next);
  unsigned int next_y = get_next_y(t->head_y, next);
  return get_board_at(state, next_x, next_y);
}


/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the x and y coordinates of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *t = &state->snakes[snum];
  char next = get_board_at(state, t->head_x, t->head_y);
  unsigned int next_x = get_next_x(t->head_x, next);
  unsigned int next_y = get_next_y(t->head_y, next);
  set_board_at(state, next_x, next_y, next);
  next = head_to_body(next);
  set_board_at(state, t->head_x, t->head_y, next);
  t->head_x = next_x;
  t->head_y = next_y;
  return;
}


/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)

  ...in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *t = &state->snakes[snum];
  char tail_next = get_board_at(state, t->tail_x, t->tail_y);
  unsigned int next_x = get_next_x(t->tail_x, tail_next);
  unsigned int next_y = get_next_y(t->tail_y, tail_next);
  set_board_at(state, t->tail_x, t->tail_y, ' ');
  t->tail_x = next_x;
  t->tail_y = next_y;

  char next = get_board_at(state, next_x, next_y);
  next = body_to_tail(next);
  set_board_at(state, next_x, next_y, next);
  return;
}


/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  for(unsigned int i = 0; i < state->num_snakes; i++)
  {
    snake_t *now = &state->snakes[i];
    if(!now->live)
      continue;
    char next = next_square(state, i);
    if(next == '#' ||(is_snake(next) && !is_head(next)) )
    {
      set_board_at(state, now->head_x, now->head_y, 'x');
      now->live = 0;
    }
    else if(next == '*')
    {
      update_head(state, i);
      add_food(state);
    }
    else
    {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}


/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  FILE *file;
  file = fopen(filename, "r");
  if(file == NULL)
    return NULL;
  unsigned int counter = 0;
  game_state_t* new = (game_state_t*) malloc(sizeof(game_state_t));
  char buff[1000000];
  new->board =(char**)malloc(sizeof(char*));
  while(fgets(buff, 1000000, file))
  {
    counter ++; 
    unsigned long len = strlen(buff)-1;
    new->board =(char**)realloc(new->board,counter*sizeof(char*));
    new->board[counter-1] = (char*)calloc(len+1,sizeof(char));
    for(int i = 0;i < len;i++)
      new->board[counter-1][i] = buff[i];
    new->board[counter-1][len] = 0;
  }
  new->num_rows = counter;
  fclose(file);
  return new;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *t = &state->snakes[snum];
  unsigned int curx = t->tail_x;
  unsigned int cury = t->tail_y;
  char c = get_board_at(state,curx,cury);
  while(!is_head(c))
  {
    curx = get_next_x(curx,c);
    cury = get_next_y(cury,c);
    c = get_board_at(state, curx, cury);
  }
  t->head_x = curx;
  t->head_y = cury;
  return;
}


/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  unsigned int counter = 0;
  for(unsigned int i = 1; i < state->num_rows;i++)
  {

    for(unsigned int j = 1;state->board[i][j]!='\0' ; j++)
    {
      if(is_tail(state->board[i][j]))
      {
        if(counter == 0)
          state->snakes = (snake_t*)malloc(sizeof(snake_t));
        else
          state->snakes = (snake_t*)realloc(state->snakes,(counter+1)*sizeof(snake_t));
        state->snakes[counter].live = 1;
        state->snakes[counter].tail_x = j;
        state->snakes[counter].tail_y = i;
        find_head(state, counter);
        counter++;
        state->num_snakes++;
      }
    }
  }
  return state;
}
