#include <stdio.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

int main(int argc, char* argv[]) {
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      in_filename = argv[i + 1];
      i++;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */
  // Read board from file, or create default board
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    state = load_board(in_filename);
    if(!state)
      return -1;
    // TODO: If the file doesn't exist, return -1
    initialize_snakes(state);
    // TODO: Then call initialize_snakes on the state you made
  } else {
    // TODO: Create default state
    state = create_default_state();
  }
  // TODO: Update state. Use the deterministic_food function
  update_state(state,deterministic_food);
  // (already implemented in state_utils.h) to add food.
  // Write updated board to file or stdout
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
    FILE* fp;
    fp = fopen(out_filename, "w");
    if(!fp)
      return -1;
    print_board(state, fp);
    fclose(fp);
  } else {
    // TODO: Print the board to stdout
    for(int i = 0;i < state->num_rows; i++)
      printf("%s\n",state->board[i]);
  }
  // TODO: Free the state
  free_state(state);
  return 0;
}
