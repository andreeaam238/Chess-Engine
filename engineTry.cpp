/*
    Team: PAsta medie
    Members: Blaga Ana-Maria-Andreea (324CB)
             Dragomir Constantin-Cristian (324CB)
             Ion Teodora-Andreea (324CB)
*/

#include "others.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

/* Reads in buffer the commands from xboard */
void read_flush_stdin(char buffer[]) {
  cin >> buffer;
  fflush(stdin);
}

/* Checks if the move given by the user is valid */
int is_move(char buffer[]) {

  if ((strlen(buffer) == 4 || strlen(buffer) == 5) && buffer[0] >= 'a' &&
      buffer[0] <= 'h' && buffer[1] >= '1' && buffer[1] <= '8') {
    return 1;
  }
  return 0;
}

/*
                                Updates the table and last pawn move with the
   given positions and sends to the xboard the command to move the pawn
*/
void execute_move(char bot_move[], char last_pawn_move[][2], char table[][9]) {
  char *move = update_table(bot_move, table, last_pawn_move, true);
  cout << "move " << move << '\n';
  free(move);
}

/* Starts a new game */
void new_game_command(char table[][9], char last_black_move[][2],
                      char last_white_move[][2], char *black_or_white,
                      int *force, bool *current_color) {
  init_table(table);

  char last_white_move_aux[16][2] = {
      {'a', '2'}, {'b', '2'}, {'c', '2'}, {'d', '2'}, {'e', '2'}, {'f', '2'},
      {'g', '2'}, {'h', '2'}, {'a', '1'}, {'b', '1'}, {'c', '1'}, {'d', '1'},
      {'e', '1'}, {'f', '1'}, {'g', '1'}, {'h', '1'}};

  char last_black_move_aux[16][2] = {
      {'a', '7'}, {'b', '7'}, {'c', '7'}, {'d', '7'}, {'e', '7'}, {'f', '7'},
      {'g', '7'}, {'h', '7'}, {'a', '8'}, {'b', '8'}, {'c', '8'}, {'d', '8'},
      {'e', '8'}, {'f', '8'}, {'g', '8'}, {'h', '8'}};

  memcpy(last_black_move, last_black_move_aux, 16 * 2 * sizeof(char));
  memcpy(last_white_move, last_white_move_aux, 16 * 2 * sizeof(char));

  // at the beginning there's no previous move
  *black_or_white = 0;
  *force = 0;
  *current_color = 1;
}

/* Tries to move a black piece in the specified order:
    pawn->horse->bishop->rook->queen->king
    Before each move checks if it puts the king in
   danger  */
void black_piece_move(char table[][9], char last_black_move[][2],
                      char last_white_move[][2], char *last_move,
                      int *black_king_has_init_pos,
                      int *short_rook_has_init_pos, int *long_rook_has_init_pos,
                      int *castled) {
  
  int cond[2];
  cond[0] = is_king_attacked(table, last_white_move);
  cond[1] = can_black_king_castle(
      table, black_king_has_init_pos, short_rook_has_init_pos,
      long_rook_has_init_pos, castled, last_white_move, last_black_move);
  if (cond[0] == 0 && cond[1] == 1) {
    return;
  }
  
   Node node, node_move;
    node.table_config = (char**)malloc(9*sizeof(char*));
    for (int i = 0; i < 9; i++) {
        node.table_config[i] = (char*)malloc(9*sizeof(char));
    }
      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          node.table_config[i][j] = table[i][j];
        }
      }

    minimax(node, 5, 5, false, MINUS_INF, PLUS_INF, &node_move, last_black_move,
                      last_white_move, last_move);
    execute_move(node_move.move, last_black_move, table);
}

/* Tries to move a white piece in the specified order:
    pawn->horse->bishop->rook->queen->king
    Before each move checks if it puts the king in
   danger  */
void white_piece_move(char table[][9], char last_white_move[][2],
                      char last_black_move[][2], char *last_move,
                      int *white_king_has_init_pos,
                      int *short_rook_has_init_pos, int *long_rook_has_init_pos,
                      int *castled) {
  
  int cond[2];
  cond[0] = is_king_attacked(table, last_black_move);
  cond[1] = can_white_king_castle(
      table, white_king_has_init_pos, short_rook_has_init_pos,
      long_rook_has_init_pos, castled, last_white_move, last_black_move);
  if (cond[0] == 0 && cond[1] == 1) {
    return;
  }

  Node node, node_move;
  node.table_config = (char**)malloc(9*sizeof(char*));
  for (int i = 0; i < 9; i++) {
    node.table_config[i] = (char*)malloc(9*sizeof(char));
  }
      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          node.table_config[i][j] = table[i][j];
        }
      }

  minimax(node, 5, 5, true, MINUS_INF, PLUS_INF, &node_move, last_black_move,
                      last_white_move, last_move);
  
  execute_move(node_move.move, last_white_move, table);
}

/* Function to make a move according to the color and mode(forced or not) */
void move_to_do(char buffer[1000], char table[][9], char last_black_move[][2],
                char last_white_move[][2], char *last_move, bool *current_color,
                int force, char black_or_white, int *black_king_has_init_pos,
                int *short_rook_has_init_pos_black,
                int *long_rook_has_init_pos_black, int *black_castled,
                int *white_king_has_init_pos,
                int *short_rook_has_init_pos_white,
                int *long_rook_has_init_pos_white, int *white_castled) {

  if (*current_color == 0) {
    update_table(buffer, table, last_black_move, false);
    if (force == 0) {
      if (is_king_attacked_white(table, last_black_move)) {
        if (move_king_white(table, last_white_move, 12, last_black_move) == 0) {
          if (protect_king_white(table, last_black_move, last_white_move,
                                 last_move) == 0)
            cout << "resign\n";
        } else {
          *white_king_has_init_pos = 0;
        }

        strcpy(last_move, buffer);
        return;
      } else if (is_king_attacked(table, last_black_move) == 0 &&
                 can_white_king_castle(table, white_king_has_init_pos,
                                       short_rook_has_init_pos_white,
                                       long_rook_has_init_pos_white,
                                       white_castled, last_white_move,
                                       last_black_move)) {

        return;
      }
    } else {
      if ((strncmp(buffer, "e8g8", 4) == 0 ||
           strncmp(buffer, "e8c8", 4) == 0) &&
          *black_castled == 0) {
        *black_castled = 1;
      }
      if (strncmp(buffer, "a8", 2) == 0 && *long_rook_has_init_pos_black == 1) {
        *long_rook_has_init_pos_black = 0;
      }
      if (strncmp(buffer, "h8", 2) == 0 && *long_rook_has_init_pos_black == 1) {
        *short_rook_has_init_pos_black = 0;
      }
      if (strncmp(buffer, "e8", 2) == 0 && *black_king_has_init_pos == 1) {
        *black_king_has_init_pos = 0;
      }
    }
  } else {
    update_table(buffer, table, last_white_move, false);
    if (force == 0) {
      if (is_king_attacked(table, last_white_move)) {
        if (move_king_black(table, last_black_move, 12, last_white_move) == 0) {
          if (protect_king_black(table, last_black_move, last_white_move,
                                 last_move) == 0)
            cout << "resign\n";
        } else {
          *black_king_has_init_pos = 0;
        }
        strcpy(last_move, buffer);
        return;
      } else if (is_king_attacked(table, last_white_move) == 0 &&
                 can_black_king_castle(table, black_king_has_init_pos,
                                       short_rook_has_init_pos_black,
                                       long_rook_has_init_pos_black,
                                       black_castled, last_white_move,
                                       last_black_move)) {

        return;
      }
    } else {
      if ((strncmp(buffer, "e1g1", 4) == 0 ||
           strncmp(buffer, "e1c1", 4) == 0) &&
          *white_castled == 0) {
        *white_castled = 1;
      }
      if (strncmp(buffer, "a1", 2) == 0 && *long_rook_has_init_pos_white == 1) {
        *long_rook_has_init_pos_white = 0;
      }
      if (strncmp(buffer, "h1", 2) == 0 && *long_rook_has_init_pos_white == 1) {
        *short_rook_has_init_pos_white = 0;
      }
      if (strncmp(buffer, "e1", 2) == 0 && *white_king_has_init_pos == 1) {
        *white_king_has_init_pos = 0;
      }
    }
  }
  strcpy(last_move, buffer);
  *current_color = !(*current_color);

  // if force is disabled the engine must think of a move
  // either from white's perspective or black's
  if (force == 0) {
    if (black_or_white == 1) {
      white_piece_move(table, last_white_move, last_black_move, last_move,
                       white_king_has_init_pos, short_rook_has_init_pos_white,
                       long_rook_has_init_pos_white, white_castled);
    } else if (black_or_white == 0) {
      black_piece_move(table, last_black_move, last_white_move, last_move,
                       black_king_has_init_pos, short_rook_has_init_pos_black,
                       long_rook_has_init_pos_black, black_castled);
    }
    *current_color = !(*current_color);
  }
}

/* Forces the engine to make a move with the current color */
void go_command(char table[][9], char last_black_move[][2],
                char last_white_move[][2], char *last_move, bool *current_color,
                int *force, char *black_or_white, int *black_king_has_init_pos,
                int *short_rook_has_init_pos_black,
                int *long_rook_has_init_pos_black, int *black_castled,
                int *white_king_has_init_pos,
                int *short_rook_has_init_pos_white,
                int *long_rook_has_init_pos_white, int *white_castled) {
  *force = 0;
  if (*current_color == 1) {
    white_piece_move(table, last_white_move, last_black_move, last_move,
                     white_king_has_init_pos, short_rook_has_init_pos_white,
                     long_rook_has_init_pos_white, white_castled);
    *black_or_white = 1;
  } else {
    black_piece_move(table, last_black_move, last_white_move, last_move,
                     black_king_has_init_pos, short_rook_has_init_pos_black,
                     long_rook_has_init_pos_black, black_castled);
    *black_or_white = 0;
  }
  *current_color = !(*current_color);
}

int main(void) {
  srand(time(NULL));

  char buffer[1000]; // reading commands
  char table[9][9];  // internal table
  char black_or_white = 0;

  int force = 0; // engine is passive or active
  bool current_color = 1;

  int black_castled = 0;
  int black_king_has_init_pos = 1;
  int short_rook_has_init_pos_black = 1;
  int long_rook_has_init_pos_black = 1;

  int white_castled = 0;
  int white_king_has_init_pos = 1;
  int short_rook_has_init_pos_white = 1;
  int long_rook_has_init_pos_white = 1;

  char last_white_move[16][2] = {
      {'a', '2'}, {'b', '2'}, {'c', '2'}, {'d', '2'}, {'e', '2'}, {'f', '2'},
      {'g', '2'}, {'h', '2'}, {'a', '1'}, {'b', '1'}, {'c', '1'}, {'d', '1'},
      {'e', '1'}, {'f', '1'}, {'g', '1'}, {'h', '1'}};

  char last_black_move[16][2] = {
      {'a', '7'}, {'b', '7'}, {'c', '7'}, {'d', '7'}, {'e', '7'}, {'f', '7'},
      {'g', '7'}, {'h', '7'}, {'a', '8'}, {'b', '8'}, {'c', '8'}, {'d', '8'},
      {'e', '8'}, {'f', '8'}, {'g', '8'}, {'h', '8'}};

  char *last_move = (char *)calloc(5, sizeof(char));

  // initializing and synchronizing engine and xboard interface
  init_table(table);
  read_flush_stdin(buffer);

  if (strcmp(buffer, "xboard") != 0)
    return 1;

  cout << "feature sigint=0\n";
  read_flush_stdin(buffer);
  cout << "feature san=0\n";
  read_flush_stdin(buffer);
  cout << "feature name=PASTA_MEDIE\n";

  while (1) {
    // receiving input from xboard
    read_flush_stdin(buffer);

    // checks if the given command is a move
    if (is_move(buffer)) {
      move_to_do(buffer, table, last_black_move, last_white_move, last_move,
                 &current_color, force, black_or_white,
                 &black_king_has_init_pos, &short_rook_has_init_pos_black,
                 &long_rook_has_init_pos_black, &black_castled,
                 &white_king_has_init_pos, &short_rook_has_init_pos_white,
                 &long_rook_has_init_pos_white, &white_castled);
      // playing a new game, reinitializing engine's table
    } else if (strcmp(buffer, "new") == 0) {
      black_castled = 0;
      black_king_has_init_pos = 1;
      short_rook_has_init_pos_black = 1;
      long_rook_has_init_pos_black = 1;

      white_castled = 0;
      white_king_has_init_pos = 1;
      short_rook_has_init_pos_white = 1;
      long_rook_has_init_pos_white = 1;
      new_game_command(table, last_black_move, last_white_move, &black_or_white,
                       &force, &current_color);
      // halting the engine at xboard's request
    } else if (strcmp(buffer, "quit") == 0) {
      break;
    } else if (strcmp(buffer, "white") == 0) {
      black_or_white = 1;
      // the engine must think of a move for white and send it to xboard
    } else if (strcmp(buffer, "go") == 0) {
      go_command(table, last_black_move, last_white_move, last_move,
                 &current_color, &force, &black_or_white,
                 &black_king_has_init_pos, &short_rook_has_init_pos_black,
                 &long_rook_has_init_pos_black, &black_castled,
                 &white_king_has_init_pos, &short_rook_has_init_pos_white,
                 &long_rook_has_init_pos_white, &white_castled);

      // the engine becomes passive
    } else if (strcmp(buffer, "force") == 0) {
      force = 1;
    }
  }
  return 0;
}
