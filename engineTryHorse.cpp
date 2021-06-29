#include "others.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <list>
#include <iterator>

using namespace std;

/* Function that ckecks if there is an available square
 for the given white horse to be moved */
int is_square_available_white_horse(char coords[], char table[][9]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char white_pieces[] = "PTCNR*";

  if (table[src_row][src_col] != 'C') {
    return 0;
  }

  if (strchr(white_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  if (coords[2] < 'a' || coords[2] > 'h' || coords[3] < '1' || coords[3] > '8')
    return 0;

  if (table[dest_row][dest_col] == ';') {
    return 1;

  } else if (table[dest_row][dest_col] != ';') {
    if (!strchr(white_pieces, table[dest_row][dest_col]))
      return 1;
  }

  return 0;
}

/* Function that tries to move the given white horse
    It checks every move to be safe for the king */
int move_horse_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_black_move[][2]) {
  char bot_move[4];

  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  int x_moves[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int y_moves[] = {1, 2, 2, 1, -1, -2, -2, -1};

  for (int i = 0; i < 8; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];

    if (is_square_available_white_horse(bot_move, table)) {
      char new_table[9][9];
      for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }
      int src_row = bot_move[1] - '1' + 1;
      int src_col = bot_move[0] - 'a' + 1;
      int dest_row = bot_move[3] - '1' + 1;
      int dest_col = bot_move[2] - 'a' + 1;
      new_table[src_row][src_col] = ';';
      new_table[dest_row][dest_col] = 'C';
      if (!is_king_attacked_white(new_table, last_black_move)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }

  return 0;
}

/* Function that ckecks if there is an available square
 for the given black horse to be moved */
int is_square_available_black_horse(char coords[], char table[][9]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char black_pieces[] = "S|LB#K";

  if (table[src_row][src_col] != 'L') {
    return 0;
  }

  if (strchr(black_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  if (coords[2] < 'a' || coords[2] > 'h' || coords[3] < '1' || coords[3] > '8')
    return 0;

  if (table[dest_row][dest_col] == ';') {
    return 1;

  } else if (table[dest_row][dest_col] != ';') {

    // the piece is not the same color
    if (!strchr(black_pieces, table[dest_row][dest_col]))
      return 1;
  }

  return 0;
}

/* Function that tries to move the given black horse
        It checks every move to be safe for the king */
int move_horse_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_white_move[][2]) {
  char bot_move[4];

  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  int x_moves[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int y_moves[] = {1, 2, 2, 1, -1, -2, -2, -1};

  for (int i = 0; i < 8; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];

    if (is_square_available_black_horse(bot_move, table)) {
      char new_table[9][9];
      for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }
      int src_row = bot_move[1] - '1' + 1;
      int src_col = bot_move[0] - 'a' + 1;
      int dest_row = bot_move[3] - '1' + 1;
      int dest_col = bot_move[2] - 'a' + 1;
      new_table[src_row][src_col] = ';';
      new_table[dest_row][dest_col] = 'L';
      if (!is_king_attacked(new_table, last_white_move)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }
  return 0;
}

/*
  Function that generates all possible table configurations by moving a black knight
*/
void generate_tables_black_horse(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_white_move[][2], Node *node) {
  char bot_move[4];

  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  int x_moves[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int y_moves[] = {1, 2, 2, 1, -1, -2, -2, -1};

  for (int i = 0; i < 8; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];

    if (is_square_available_black_horse(bot_move, table)) {
      char new_table[9][9];
      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }
      int src_row = bot_move[1] - '1' + 1;
      int src_col = bot_move[0] - 'a' + 1;
      int dest_row = bot_move[3] - '1' + 1;
      int dest_col = bot_move[2] - 'a' + 1;
      new_table[src_row][src_col] = ';';
      new_table[dest_row][dest_col] = 'L';
      if (!is_king_attacked(new_table, last_white_move)) {
        char **new_table1 = (char**)malloc(9*sizeof(char*));
        for (int i = 0; i < 9; i++) {
          new_table1[i] = (char*)malloc(9*sizeof(char));
        }
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
        Node child;
        child.table_config = new_table1;
        child.move[0] = bot_move[0]; child.move[1] = bot_move[1];
        child.move[2] = bot_move[2]; child.move[3] = bot_move[3];
        node->children.push_back(child);
      }
    }
  }
}


/*
  Function that generates all possible table configurations by moving a white knight
*/
void generate_tables_white_horse(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_black_move[][2], Node *node) {
  char bot_move[4];

  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  int x_moves[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int y_moves[] = {1, 2, 2, 1, -1, -2, -2, -1};

  for (int i = 0; i < 8; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];

    if (is_square_available_white_horse(bot_move, table)) {
      char new_table[9][9];
      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }
      int src_row = bot_move[1] - '1' + 1;
      int src_col = bot_move[0] - 'a' + 1;
      int dest_row = bot_move[3] - '1' + 1;
      int dest_col = bot_move[2] - 'a' + 1;
      new_table[src_row][src_col] = ';';
      new_table[dest_row][dest_col] = 'C';
      if (!is_king_attacked_white(new_table, last_black_move)) {
        char **new_table1 = (char**)malloc(9*sizeof(char*));
        for (int i = 0; i < 9; i++) {
          new_table1[i] = (char*)malloc(9*sizeof(char));
        }
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
        Node child;
        child.table_config = new_table1;
        child.move[0] = bot_move[0]; child.move[1] = bot_move[1];
        child.move[2] = bot_move[2]; child.move[3] = bot_move[3];
        node->children.push_back(child);
      }
    }
  }
}