#include "others.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

/* Function to check if there is a square available
 for the given black queen to be moved */
int is_square_available_black_queen(char coords[], char table[][9]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char black_pieces[] = "S|LB#K";

  // Checks if the current piece to be moved is the black queen.
  if (table[src_row][src_col] != '#') {
    return 0;
  }

  // Checks if the destination has a piece with the same color
  if (strchr(black_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  // Checks if the destination is still on the table
  if (coords[2] < 'a' || coords[2] > 'h' || coords[3] < '1' || coords[3] > '8')
    return 0;

  // Checks if the destination square is empty
  if (table[dest_row][dest_col] == ';') {
    return 1;

    // Checks if the rook can capture an enemy piece
  } else if (table[dest_row][dest_col] != ';') {

    // and the piece is not the same color
    if (!strchr(black_pieces, table[dest_row][dest_col]))
      return 2;
  }

  return 0;
}

/* Function to check if there is a square available
 for the given white queen to be moved */
int is_square_available_white_queen(char coords[], char table[][9]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char white_pieces[] = "PTCNR*";

  // Checks if the current piece to be moved is the white queen.
  if (table[src_row][src_col] != '*') {
    return 0;
  }

  // Checks if the destination has a piece with the same color
  if (strchr(white_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  // Checks if the destination is still on the table
  if (coords[2] < 'a' || coords[2] > 'h' || coords[3] < '1' || coords[3] > '8')
    return 0;

  // Checks if the destination square is empty
  if (table[dest_row][dest_col] == ';') {
    return 1;

    // Checks if the rook can capture an enemy piece
  } else if (table[dest_row][dest_col] != ';') {
    if (!strchr(white_pieces, table[dest_row][dest_col]))
      return 2;
  }

  return 0;
}

/* Function that tries to move the given black queen
   It checks every move to be safe for the king */
int move_queen_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_white_move[][2]) {
  char bot_move[4];

  // The queen's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The queen's moves are a combination of the bishop's moves and the rook's
  // moves.

  // The coordinates to which the queen can be moved multiple positions
  int bishop_x_moves[] = {-1, -1, 1, 1};
  int bishop_y_moves[] = {1, -1, 1, -1};



  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + bishop_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + bishop_y_moves[i];
    int ok = 0;

    while (is_square_available_black_queen(bot_move, table)) {
      ok = 1;
      if (is_square_available_black_queen(bot_move, table) == 2) {
        bot_move[3] += bishop_x_moves[i];
        bot_move[2] += bishop_y_moves[i];
        break;
      }
      bot_move[3] += bishop_x_moves[i];
      bot_move[2] += bishop_y_moves[i];
    }

    if (ok == 1) {
      bot_move[3] -= bishop_x_moves[i];
      bot_move[2] -= bishop_y_moves[i];
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
      new_table[dest_row][dest_col] = '#';

      while (bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h' && is_king_attacked(new_table, last_white_move)) {
        bot_move[3] -= bishop_x_moves[i];
        bot_move[2] -= bishop_y_moves[i];
        for (int i = 0; i <= 8; i++) {
          for (int j = 0; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }

        if (bot_move[3] <'1' || bot_move[3] > '8' || bot_move[2] < 'a' || bot_move[2] > 'h') {
          break;
        }

        src_row = bot_move[1] - '1' + 1;
        src_col = bot_move[0] - 'a' + 1;
        dest_row = bot_move[3] - '1' + 1;
        dest_col = bot_move[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = '#';
        
        FILE *out = fopen("out", "at");
        fprintf(out, "Queen B %s\n", bot_move);
        fclose(out);
      }
      if(is_square_available_black_queen(bot_move, table)){
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }

  // The coordinates to which the queen can be moved multiple positions
  int rook_x_moves[] = {0, 1, 0, -1};
  int rook_y_moves[] = {-1, 0, 1, 0};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + rook_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + rook_y_moves[i];
    int ok = 0;

    while (is_square_available_black_queen(bot_move, table)) {
      ok = 1;
      if (is_square_available_black_queen(bot_move, table) == 2) {
        bot_move[3] += rook_x_moves[i];
        bot_move[2] += rook_y_moves[i];
        break;
      }
      bot_move[3] += rook_x_moves[i];
      bot_move[2] += rook_y_moves[i];
    }

    if (ok == 1) {
      bot_move[3] -= rook_x_moves[i];
      bot_move[2] -= rook_y_moves[i];
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
      new_table[dest_row][dest_col] = '#';

      while (bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h' && is_king_attacked(new_table, last_white_move)) {
        if (bot_move[3])
        bot_move[3] -= rook_x_moves[i];
        bot_move[2] -= rook_y_moves[i];
        for (int i = 0; i <= 8; i++) {
          for (int j = 0; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }

        if (bot_move[3] <'1' || bot_move[3] > '8' || bot_move[2] < 'a' || bot_move[2] > 'h') {
          break;
        }

        src_row = bot_move[1] - '1' + 1;
        src_col = bot_move[0] - 'a' + 1;
        dest_row = bot_move[3] - '1' + 1;
        dest_col = bot_move[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = '#';

        FILE *out = fopen("out", "at");
        fprintf(out, "Queen R %s\n", bot_move);
        fclose(out);
      }
      if(is_square_available_black_queen(bot_move, table)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }

  return 0;
}

/* Function that tries to move the given white queen
   It checks every move to be safe for the king */
int move_queen_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_black_move[][2]) {
  char bot_move[4];

  // The queen's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The queen's moves are a combination of the bishop's moves and the rook's
  // moves.

  // The coordinates to which the queen can be moved multiple positions
  int bishop_x_moves[] = {-1, -1, 1, 1};
  int bishop_y_moves[] = {1, -1, 1, -1};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + bishop_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + bishop_y_moves[i];
    int ok = 0;

    while (is_square_available_white_queen(bot_move, table)) {
      ok = 1;
      if (is_square_available_white_queen(bot_move, table) == 2) {
        bot_move[3] += bishop_x_moves[i];
        bot_move[2] += bishop_y_moves[i];
        break;
      }
      bot_move[3] += bishop_x_moves[i];
      bot_move[2] += bishop_y_moves[i];
    }

    if (ok == 1) {
      bot_move[3] -= bishop_x_moves[i];
      bot_move[2] -= bishop_y_moves[i];
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
      new_table[dest_row][dest_col] = '*';

      while (is_king_attacked_white(new_table, last_black_move)) {
        bot_move[3] -= bishop_x_moves[i];
        bot_move[2] -= bishop_y_moves[i];
        for (int i = 0; i <= 8; i++) {
          for (int j = 0; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }

        if (bot_move[3] <'1' || bot_move[3] > '8' || bot_move[2] < 'a' || bot_move[2] > 'h') {
          break;
        }

        src_row = bot_move[1] - '1' + 1;
        src_col = bot_move[0] - 'a' + 1;
        dest_row = bot_move[3] - '1' + 1;
        dest_col = bot_move[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = '*';
      }
      if (is_square_available_white_queen(bot_move, table)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }

  // The coordinates to which the queen can be moved multiple positions
  int rook_x_moves[] = {0, 1, 0, -1};
  int rook_y_moves[] = {-1, 0, 1, 0};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + rook_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + rook_y_moves[i];
    int ok = 0;

    while (is_square_available_white_queen(bot_move, table)) {
      ok = 1;
      if (is_square_available_white_queen(bot_move, table) == 2) {
        bot_move[3] += rook_x_moves[i];
        bot_move[2] += rook_y_moves[i];
        break;
      }
      bot_move[3] += rook_x_moves[i];
      bot_move[2] += rook_y_moves[i];
    }

    if (ok == 1) {
      bot_move[3] -= rook_x_moves[i];
      bot_move[2] -= rook_y_moves[i];
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
      new_table[dest_row][dest_col] = '*';

      while (is_king_attacked_white(new_table, last_black_move)) {
        bot_move[3] -= rook_x_moves[i];
        bot_move[2] -= rook_y_moves[i];
        for (int i = 0; i <= 8; i++) {
          for (int j = 0; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }

        if (bot_move[3] <'1' || bot_move[3] > '8' || bot_move[2] < 'a' || bot_move[2] > 'h') {
          break;
        }

        src_row = bot_move[1] - '1' + 1;
        src_col = bot_move[0] - 'a' + 1;
        dest_row = bot_move[3] - '1' + 1;
        dest_col = bot_move[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = '*';
      }
      if (is_square_available_white_queen(bot_move, table)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }

  return 0;
}


/*
  Function that generates all possible table configurations by moving the black queen
*/
void generate_tables_black_queen(char table[][9], char last_pawn_move[][2], int pawn_index,
  char last_white_move[][2], Node *node) {

  char bot_move[4];

  // The queen's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The queen's moves are a combination of the bishop's moves and the rook's
  // moves.

  // The coordinates to which the queen can be moved multiple positions
  int bishop_x_moves[] = {-1, -1, 1, 1};
  int bishop_y_moves[] = {1, -1, 1, -1};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + bishop_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + bishop_y_moves[i];
    // int ok = 0;

    while (is_square_available_black_queen(bot_move, table)) {
     
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
      new_table[dest_row][dest_col] = '#';

      if (is_square_available_black_queen(bot_move, table) == 2) {
        if(bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h' && 
          !is_king_attacked(new_table, last_white_move)) {
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
        break;
      }
      if(bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h' && !is_king_attacked(new_table, last_white_move)) {
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
      bot_move[3] += bishop_x_moves[i];
      bot_move[2] += bishop_y_moves[i];
    }
  }

  // The coordinates to which the queen can be moved multiple positions
  int rook_x_moves[] = {0, 1, 0, -1};
  int rook_y_moves[] = {-1, 0, 1, 0};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + rook_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + rook_y_moves[i];
    // int ok = 0;

    while (is_square_available_black_queen(bot_move, table)) {
      // ok = 1;
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
      new_table[dest_row][dest_col] = '#';

      if (is_square_available_black_queen(bot_move, table) == 2) {
        if(!(is_king_attacked(new_table, last_white_move)) && bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h') {
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
        break;
      }
      if(!(is_king_attacked(new_table, last_white_move)) && bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h') {
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
      bot_move[3] += rook_x_moves[i];
      bot_move[2] += rook_y_moves[i];
    }
  }
}


/*
  Function that generates all possible table configurations by moving the white queen
*/
void generate_tables_white_queen(char table[][9], char last_pawn_move[][2], int pawn_index,
  char last_black_move[][2], Node *node) {

  char bot_move[4];

  // The queen's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The queen's moves are a combination of the bishop's moves and the rook's
  // moves.

  // The coordinates to which the queen can be moved multiple positions
  int bishop_x_moves[] = {-1, -1, 1, 1};
  int bishop_y_moves[] = {1, -1, 1, -1};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + bishop_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + bishop_y_moves[i];
   //  int ok = 0;

    while (is_square_available_white_queen(bot_move, table)) {
     
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
      new_table[dest_row][dest_col] = '#';

      if (is_square_available_white_queen(bot_move, table) == 2) {
        if(bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h' && 
          !is_king_attacked(new_table, last_black_move)) {
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
        break;
      }
      if(bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h' && !is_king_attacked(new_table, last_black_move)) {
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
      bot_move[3] += bishop_x_moves[i];
      bot_move[2] += bishop_y_moves[i];
    }
  }

  // The coordinates to which the queen can be moved multiple positions
  int rook_x_moves[] = {0, 1, 0, -1};
  int rook_y_moves[] = {-1, 0, 1, 0};

  // Check for all the possible moves and execute one that keeps the King in a
  // safe spot
  for (int i = 0; i < 4; ++i) {
    bot_move[3] = last_pawn_move[pawn_index][1] + rook_x_moves[i];
    bot_move[2] = last_pawn_move[pawn_index][0] + rook_y_moves[i];
    // int ok = 0;

    while (is_square_available_white_queen(bot_move, table)) {
      // ok = 1;
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
      new_table[dest_row][dest_col] = '#';

      if (is_square_available_white_queen(bot_move, table) == 2) {
        if(!(is_king_attacked(new_table, last_black_move)) && bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h') {
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
        break;
      }
      if(!(is_king_attacked(new_table, last_black_move)) && bot_move[3] >= '1' && bot_move[3] <= '8' && bot_move[2] >= 'a' && bot_move[2] <= 'h') {
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
      bot_move[3] += rook_x_moves[i];
      bot_move[2] += rook_y_moves[i];
    }
  }
}
