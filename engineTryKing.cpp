#include "others.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

/* Function that checks if the pawn given at table[row][col]
 can attack the black king */
int can_attack_pawn(char table[][9], int col, int row) {
  if (!(row + 1 < 1 || row + 1 > 8 || col - 1 < 1 || col - 1 > 8) &&
      table[row + 1][col - 1] == 'K')
    return 1;
  if (!(row + 1 < 1 || row + 1 > 8 || col + 1 < 1 || col + 1 > 8) &&
      table[row + 1][col + 1] == 'K')
    return 1;
  return 0;
}

/* Function that checks if the pawn given at table[row][col]
 can attack the white king */
int can_attack_pawn_white(char table[][9], int col, int row) {
  if (!(row - 1 < 1 || row - 1 > 8 || col - 1 < 1 || col - 1 > 8) &&
      table[row - 1][col - 1] == 'R')
    return 1;
  if (!(row - 1 < 1 || row - 1 > 8 || col + 1 < 1 || col + 1 > 8) &&
      table[row - 1][col + 1] == 'R')
    return 1;
  return 0;
}

/* Function that checks if the given bishop at table[row][col]
 can attack the black king */
int can_attack_bishop(char table[][9], int col, int row) {
  int x_moves[] = {-1, -1, 1, 1};
  int y_moves[] = {1, -1, 1, -1};
  int ok = 0;

  int init_col = col;
  int init_row = row;

  for (int i = 0; i < 4; ++i) {
    col = init_col;
    row = init_row;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'K') {
      ok = 1;
      break;
    }
    while (table[row][col] == ';') {
      col += y_moves[i];
      row += x_moves[i];
      if (row < 1 || row > 8 || col < 1 || col > 8)
        break;
      if (table[row][col] == 'K') {
        ok = 1;
        break;
      }
    }
  }
  return ok;
}

/* Function that checks if the given bishop at table[row][col]
 can attack the white king */
int can_attack_bishop_white(char table[][9], int col, int row) {
  int x_moves[] = {-1, -1, 1, 1};
  int y_moves[] = {1, -1, 1, -1};
  int ok = 0;

  int init_col = col;
  int init_row = row;

  for (int i = 0; i < 4; ++i) {
    col = init_col;
    row = init_row;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'R') {
      ok = 1;
      break;
    }
    while (table[row][col] == ';') {
      col += y_moves[i];
      row += x_moves[i];
      if (row < 1 || row > 8 || col < 1 || col > 8)
        break;
      if (table[row][col] == 'R') {
        ok = 1;
        break;
      }
    }
  }
  return ok;
}

/* Function that checks if the given rook at table[row][col]
 can attack the black king */
int can_attack_rook(char table[][9], int col, int row) {
  int x_moves[] = {0, 1, 0, -1};
  int y_moves[] = {1, 0, -1, 0};

  int ok = 0;

  int init_col = col;
  int init_row = row;

  for (int i = 0; i < 4; ++i) {
    col = init_col;
    row = init_row;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'K') {
      ok = 1;
      break;
    }
    while (table[row][col] == ';') {
      col += y_moves[i];
      row += x_moves[i];
      if (row < 1 || row > 8 || col < 1 || col > 8)
        break;
      if (table[row][col] == 'K') {
        ok = 1;
        break;
      }
    }
  }
  return ok;
}

/* Function that checks if the given rook at table[row][col]
 can attack the white king */
int can_attack_rook_white(char table[][9], int col, int row) {
  int x_moves[] = {0, 1, 0, -1};
  int y_moves[] = {1, 0, -1, 0};

  int ok = 0;

  int init_col = col;
  int init_row = row;

  for (int i = 0; i < 4; ++i) {
    col = init_col;
    row = init_row;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'R') {
      ok = 1;
      break;
    }
    while (table[row][col] == ';') {
      col += y_moves[i];
      row += x_moves[i];
      if (row < 1 || row > 8 || col < 1 || col > 8)
        break;
      if (table[row][col] == 'R') {
        ok = 1;
        break;
      }
    }
  }
  return ok;
}

/* Function that checks if the given horse at table[row][col]
 can attack the black king */
int can_attack_horse(char table[][9], int col, int row) {
  int x_moves[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int y_moves[] = {1, 2, 2, 1, -1, -2, -2, -1};

  int init_row = row;
  int init_col = col;
  for (int i = 0; i < 8; ++i) {
    row = init_row;
    col = init_col;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'K')
      return 1;
  }
  return 0;
}

/* Function that checks if the given horse at table[row][col]
 can attack the white king */
int can_attack_horse_white(char table[][9], int col, int row) {
  int x_moves[] = {2, 1, -1, -2, -2, -1, 1, 2};
  int y_moves[] = {1, 2, 2, 1, -1, -2, -2, -1};

  int init_row = row;
  int init_col = col;
  for (int i = 0; i < 8; ++i) {
    row = init_row;
    col = init_col;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'R')
      return 1;
  }
  return 0;
}

/* Function that checks if the given white king at table[row][col]
 can attack the black king */
int can_attack_king(char table[][9], int col, int row) {
  int x_moves[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y_moves[] = {1, 0, -1, 1, -1, 1, 0, -1};

  int init_row = row;
  int init_col = col;
  for (int i = 0; i < 8; ++i) {
    row = init_row;
    col = init_col;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'K')
      return 1;
  }
  return 0;
}

/* Function that checks if the given black king at table[row][col]
 can attack the white king */
int can_attack_king_white(char table[][9], int col, int row) {
  int x_moves[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y_moves[] = {1, 0, -1, 1, -1, 1, 0, -1};

  int init_row = row;
  int init_col = col;
  for (int i = 0; i < 8; ++i) {
    row = init_row;
    col = init_col;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'R')
      return 1;
  }
  return 0;
}

/* Function that checks if the given queen at table[row][col]
 can attack the black king */
int can_attack_queen(char table[][9], int col, int row) {
  int x_moves[] = {-1, -1, 1, 1, 0, 1, 0, -1};
  int y_moves[] = {1, -1, 1, -1, -1, 0, 1, 0};

  int init_col = col;
  int init_row = row;

  for (int i = 0; i < 8; ++i) {
    col = init_col;
    row = init_row;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'K') {
      return 1;
    }
    while (table[row][col] == ';') {
      col += y_moves[i];
      row += x_moves[i];
      if (row < 1 || row > 8 || col < 1 || col > 8)
        break;
      if (table[row][col] == 'K') {
        return 1;
      }
    }
  }
  return 0;
}

/* Function that checks if the given queen at table[row][col]
 can attack the white king */
int can_attack_queen_white(char table[][9], int col, int row) {
  int x_moves[] = {-1, -1, 1, 1, 0, 1, 0, -1};
  int y_moves[] = {1, -1, 1, -1, -1, 0, 1, 0};

  int ok = 0;

  int init_col = col;
  int init_row = row;

  for (int i = 0; i < 8; ++i) {
    col = init_col;
    row = init_row;
    col += y_moves[i];
    row += x_moves[i];
    if (!(row < 1 || row > 8 || col < 1 || col > 8) && table[row][col] == 'R') {
      ok = 1;
      break;
    }
    while (table[row][col] == ';') {
      col += y_moves[i];
      row += x_moves[i];
      if (row < 1 || row > 8 || col < 1 || col > 8)
        break;
      if (table[row][col] == 'R') {
        ok = 1;
        break;
      }
    }
  }
  return ok;
}

/* Function to check if any of the pieces on the table
 can attack the black king */
int is_king_attacked(char table[][9], char last_white_move[][2]) {

  for (int i = 0; i < 16; i++) {
    int col = last_white_move[i][0] - 'a' + 1;
    int row = last_white_move[i][1] - '1' + 1;
    if (table[row][col] == 'P' && can_attack_pawn(table, col, row)) {
      // the king can be captured with a next move of the pawn
      return 1;
    } else if (table[row][col] == 'C' && can_attack_horse(table, col, row)) {
      return 1;
    } else if (table[row][col] == 'N' && can_attack_bishop(table, col, row)) {
      return 1;
    } else if (table[row][col] == 'T' && can_attack_rook(table, col, row)) {
      return 1;
    } else if (table[row][col] == 'R' && can_attack_king(table, col, row)) {
      return 1;
    } else if (table[row][col] == '*' && can_attack_queen(table, col, row)) {
      return 1;
    }
  }
  return 0;
}

/* Function to check if any of the pieces on the table
 can attack the white king */
int is_king_attacked_white(char table[][9], char last_black_move[][2]) {

  for (int i = 0; i < 16; i++) {
    int col = last_black_move[i][0] - 'a' + 1;
    int row = last_black_move[i][1] - '1' + 1;
    if (table[row][col] == 'S' && can_attack_pawn_white(table, col, row)) {
      // the king can be captured with a next move of the pawn
      return 1;
    } else if (table[row][col] == 'L' &&
               can_attack_horse_white(table, col, row)) {
      return 1;
    } else if (table[row][col] == 'B' &&
               can_attack_bishop_white(table, col, row)) {
      return 1;
    } else if (table[row][col] == '|' &&
               can_attack_rook_white(table, col, row)) {
      return 1;
    } else if (table[row][col] == 'K' &&
               can_attack_king_white(table, col, row)) {
      return 1;
    } else if (table[row][col] == '#' &&
               can_attack_queen_white(table, col, row)) {
      return 1;
    }
  }
  return 0;
}

/* Function to check if there is a square available
 for the white king to be moved */
int is_square_available_white_king(char coords[], char table[][9],
                                   char last_black_move[][2]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char white_pieces[] = "PTCNR*";

  if (table[src_row][src_col] != 'R') {
    return 0;
  }

  if (strchr(white_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  if (coords[2] < 'a' || coords[2] > 'h' || coords[3] < '1' || coords[3] > '8')
    return 0;

  char new_table[9][9];
  for (int i = 0; i <= 8; i++) {
    for (int j = 0; j <= 8; j++) {
      new_table[i][j] = table[i][j];
    }
  }
  new_table[src_row][src_col] = ';';
  new_table[dest_row][dest_col] = 'R';
  if (!is_king_attacked_white(new_table, last_black_move)) {
    return 1;
  }
  return 0;
}

/* Function to move the white king on the table */
int move_king_white(char table[][9], char last_pawn_move[][2], int king_index,
                    char last_black_move[][2]) {
  char bot_move[4];

  bot_move[0] = last_pawn_move[king_index][0];
  bot_move[1] = last_pawn_move[king_index][1];

  int x_moves[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y_moves[] = {1, 0, -1, 1, -1, 1, 0, -1};

  for (int i = 0; i < 8; i++) {
    bot_move[3] = last_pawn_move[king_index][1] + x_moves[i];
    bot_move[2] = last_pawn_move[king_index][0] + y_moves[i];
    if (is_square_available_white_king(bot_move, table, last_black_move)) {
      execute_move(bot_move, last_pawn_move, table);
      last_pawn_move[king_index][0] = bot_move[2];
      last_pawn_move[king_index][1] = bot_move[3];
      return 1;
    }
  }
  return 0;
}

/* Function to check if there is a square available
 for the black king to be moved */
int is_square_available_black_king(char coords[], char table[][9],
                                   char last_white_move[][2]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char black_pieces[] = "S|LB#K";

  if (table[src_row][src_col] != 'K') {
    return 0;
  }

  if (strchr(black_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  if (coords[2] < 'a' || coords[2] > 'h' || coords[3] < '1' || coords[3] > '8')
    return 0;

  char new_table[9][9];
  for (int i = 0; i <= 8; i++) {
    for (int j = 0; j <= 8; j++) {
      new_table[i][j] = table[i][j];
    }
  }
  new_table[src_row][src_col] = ';';
  new_table[dest_row][dest_col] = 'K';

  if (!is_king_attacked(new_table, last_white_move)) {
    return 1;
  }
  return 0;
}

/* Function to move the black king on the table */
int move_king_black(char table[][9], char last_black_move[][2], int king_index,
                    char last_white_move[][2]) {
  char bot_move[4];

  bot_move[0] = last_black_move[king_index][0];
  bot_move[1] = last_black_move[king_index][1];

  int x_moves[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y_moves[] = {1, 0, -1, 1, -1, 1, 0, -1};

  for (int i = 0; i < 8; i++) {
    bot_move[3] = last_black_move[king_index][1] + x_moves[i];
    bot_move[2] = last_black_move[king_index][0] + y_moves[i];
    if (is_square_available_black_king(bot_move, table, last_white_move)) {
      execute_move(bot_move, last_black_move, table);
      last_black_move[king_index][0] = bot_move[2];
      last_black_move[king_index][1] = bot_move[3];
      return 1;
    }
  }
  return 0;
}

/* Function that tries to move a black piece to protect
 the black king in case of attack */
int protect_king_black(char table[][9], char last_black_move[][2],
                       char last_white_move[][2], char *last_move) {
  int check = 1;
  for (int i = 0; i < 8; ++i) {
    if (move_pawn_black(table, last_black_move, i, last_move,
                        last_white_move) != 0) {
      check = 0;
      break;
    } else {
      int dest_col = last_black_move[i][0] - 'a' + 1;
      int dest_row = last_black_move[i][1] - '1' + 1;

      if (table[dest_row][dest_col] == 'B') {
        if (move_bishop_black(table, last_black_move, i, last_white_move) !=
            0) {
          check = 0;
          break;
        }
      } else if (table[dest_row][dest_col] == 'L') {
        if (move_horse_black(table, last_black_move, i, last_white_move) != 0) {
          check = 0;
          break;
        }
      } else if (table[dest_row][dest_col] == '|') {
        if (move_rook_black(table, last_black_move, i, last_white_move) != 0) {
          check = 0;
          break;
        }
      } else if (table[dest_row][dest_col] == '#') {
        if (move_queen_black(table, last_black_move, i, last_white_move) != 0) {
          check = 0;
          break;
        }
      }
    }
  }
  if (check == 1) {
    if (move_horse_black(table, last_black_move, 9, last_white_move) == 0 &&
        move_horse_black(table, last_black_move, 14, last_white_move) == 0) {
      if (move_bishop_black(table, last_black_move, 10, last_white_move) == 0 &&
          move_bishop_black(table, last_black_move, 13, last_white_move) == 0) {
        if (move_rook_black(table, last_black_move, 8, last_white_move) == 0 &&
            move_rook_black(table, last_black_move, 15, last_white_move) == 0) {
          if (move_queen_black(table, last_black_move, 11, last_white_move) ==
              0) {
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

/* Function that tries to move a white piece to protect
 the white king in case of attack */
int protect_king_white(char table[][9], char last_black_move[][2],
                       char last_white_move[][2], char *last_move) {
  int check = 1;
  for (int i = 0; i < 8; ++i) {
    if (move_pawn_white(table, last_white_move, i, last_move,
                        last_black_move) != 0) {
      check = 0;
      break;
    } else {
      int dest_col = last_white_move[i][0] - 'a' + 1;
      int dest_row = last_white_move[i][1] - '1' + 1;

      if (table[dest_row][dest_col] == 'N') {
        if (move_bishop_white(table, last_white_move, i, last_black_move) !=
            0) {
          check = 0;
          break;
        }
      } else if (table[dest_row][dest_col] == 'C') {
        if (move_horse_white(table, last_white_move, i, last_black_move) != 0) {
          check = 0;
          break;
        }
      } else if (table[dest_row][dest_col] == 'T') {
        if (move_rook_white(table, last_white_move, i, last_black_move) != 0) {
          check = 0;
          break;
        }
      } else if (table[dest_row][dest_col] == '*') {
        if (move_queen_white(table, last_white_move, i, last_black_move) != 0) {
          check = 0;
          break;
        }
      }
    }
  }
  if (check == 1) {
    if (move_horse_white(table, last_white_move, 9, last_black_move) == 0 &&
        move_horse_white(table, last_white_move, 14, last_black_move) == 0) {
      if (move_bishop_white(table, last_white_move, 10, last_black_move) == 0 &&
          move_bishop_white(table, last_white_move, 13, last_black_move) == 0) {
        if (move_rook_white(table, last_white_move, 8, last_black_move) == 0 &&
            move_rook_white(table, last_white_move, 15, last_black_move) == 0) {
          if (move_queen_white(table, last_white_move, 11, last_black_move) ==
              0) {
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

/*
  Function to determine if black king can castle
*/
int can_black_king_castle(char table[][9], int *king_has_init_pos,
                          int *short_rook_has_init_pos,
                          int *long_rook_has_init_pos, int *castled,
                          char last_white_move[][2],
                          char last_black_move[][2]) {
  // We already know the king is safe in the current position
  /*
    |LB#KBL| - blacks
    SSSSSSSS
    ;;;;;;;;
    ;;;;;;;;
    ;;;;;;;;
    ;;;;;;;;
    PPPPPPPP
    TCN*RNCT - whites
  */
  if (*castled == 1) {
    return 0;
  }
  if (*king_has_init_pos == 0) {
    return 0;
  }
  char new_table[9][9];
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      new_table[i][j] = table[i][j];
    }
  }
  if (*short_rook_has_init_pos == 1) {
    // Trying short castle
    int short_castle_rook_index = 15;
    int black_king_index = 12;
    if (table['8' - '1' + 1]['f' - 'a' + 1] == ';' &&
        table['8' - '1' + 1]['g' - 'a' + 1] == ';') {
      // It must be a clear path between king a rook
      int king_col = 'e' - 'a' + 1;
      int king_row = '8' - '1' + 1;
      int rook_col = 'h' - 'a' + 1;
      int rook_row = '8' - '1' + 1;

      int dest_king_col = 'g' - 'a' + 1;
      int dest_king_row = '8' - '1' + 1;
      int dest_rook_col = 'f' - 'a' + 1;
      int dest_rook_row = '8' - '1' + 1;

      new_table[king_row][king_col] = ';';
      new_table[rook_row][rook_col] = ';';
      new_table[dest_king_row][dest_king_col] = 'K';
      new_table[dest_rook_row][dest_rook_col] = '|';

      char table2[9][9];
      for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
          table2[i][j] = table[i][j];
        }
      }
      table2['8' - '1' + 1]['f' - 'a' + 1] = 'K';
      table2[king_row][king_col] = ';';
      if (!is_king_attacked(new_table, last_white_move) &&
          !is_king_attacked(table2, last_white_move)) {
        cout << "move e8g8" << '\n';
        for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
            table[i][j] = new_table[i][j];
          }
        }
        last_black_move[black_king_index][0] = dest_king_col;
        last_black_move[black_king_index][1] = dest_king_row;
        last_black_move[short_castle_rook_index][0] = dest_rook_col;
        last_black_move[short_castle_rook_index][1] = dest_rook_row;
        *castled = 1;
        return 1;
      }
    }
  }
  if (*long_rook_has_init_pos == 1) {
    // Trying long castle
    int long_castle_rook_index = 8;
    int black_king_index = 12;
    if (table['8' - '1' + 1]['b' - 'a' + 1] == ';' &&
        table['8' - '1' + 1]['c' - 'a' + 1] == ';' &&
        table['8' - '1' + 1]['d' - 'a' + 1] == ';') {
      // It must be a clear path between king a rook
      int king_col = 'e' - 'a' + 1;
      int king_row = '8' - '1' + 1;
      int rook_col = 'a' - 'a' + 1;
      int rook_row = '8' - '1' + 1;

      int dest_king_col = 'c' - 'a' + 1;
      int dest_king_row = '8' - '1' + 1;
      int dest_rook_col = 'd' - 'a' + 1;
      int dest_rook_row = '8' - '1' + 1;
      new_table[king_row][king_col] = ';';
      new_table[rook_row][rook_col] = ';';
      new_table[dest_king_row][dest_king_col] = 'K';
      new_table[dest_rook_row][dest_rook_col] = '|';

      char table2[9][9];
      for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
          table2[i][j] = table[i][j];
        }
      }
      table2['8' - '1' + 1]['c' - 'a' + 1] = 'K';
      table2[king_row][king_col] = ';';

      if (!is_king_attacked(new_table, last_white_move) &&
          !is_king_attacked(table2, last_white_move)) {
        cout << "move e8c8" << '\n';
        for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
            table[i][j] = new_table[i][j];
          }
        }
        last_black_move[black_king_index][0] = dest_king_col;
        last_black_move[black_king_index][1] = dest_king_row;
        last_black_move[long_castle_rook_index][0] = dest_rook_col;
        last_black_move[long_castle_rook_index][1] = dest_rook_row;
        *castled = 1;
        return 1;
      }
    }
  }
  return 0;
}

/*
  Function to determine if white king can castle
*/
int can_white_king_castle(char table[][9], int *king_has_init_pos,
                          int *short_rook_has_init_pos,
                          int *long_rook_has_init_pos, int *castled,
                          char last_white_move[][2],
                          char last_black_move[][2]) {
  // We already know the king is safe in the current position
  /*
    |LB#KBL| - blacks
    SSSSSSSS
    ;;;;;;;;
    ;;;;;;;;
    ;;;;;;;;
    ;;;;;;;;
    PPPPPPPP
    TCN*RNCT - whites
  */
  if (*castled == 1) {
    return 0;
  }
  if (*king_has_init_pos == 0) {
    return 0;
  }
  char new_table[9][9];
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      new_table[i][j] = table[i][j];
    }
  }
  if (*short_rook_has_init_pos == 1) {
    // Trying short castle
    int short_castle_rook_index = 15;
    int white_king_index = 12;
    if (table['1' - '1' + 1]['f' - 'a' + 1] == ';' &&
        table['1' - '1' + 1]['g' - 'a' + 1] == ';') {
      // It must be a clear path between king a rook
      int king_col = 'e' - 'a' + 1;
      int king_row = '1' - '1' + 1;
      int rook_col = 'h' - 'a' + 1;
      int rook_row = '1' - '1' + 1;

      int dest_king_col = 'g' - 'a' + 1;
      int dest_king_row = '1' - '1' + 1;
      int dest_rook_col = 'f' - 'a' + 1;
      int dest_rook_row = '1' - '1' + 1;

      new_table[king_row][king_col] = ';';
      new_table[rook_row][rook_col] = ';';
      new_table[dest_king_row][dest_king_col] = 'R';
      new_table[dest_rook_row][dest_rook_col] = 'T';

      char table2[9][9];
      for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
          table2[i][j] = table[i][j];
        }
      }
      table2['1' - '1' + 1]['f' - 'a' + 1] = 'R';
      table2[king_row][king_col] = ';';

      if (!is_king_attacked(new_table, last_black_move) &&
          !is_king_attacked(table2, last_black_move)) {
        cout << "move e1g1" << '\n';
        for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
            table[i][j] = new_table[i][j];
          }
        }
        last_white_move[white_king_index][0] = dest_king_col;
        last_white_move[white_king_index][1] = dest_king_row;
        last_white_move[short_castle_rook_index][0] = dest_rook_col;
        last_white_move[short_castle_rook_index][1] = dest_rook_row;
        *castled = 1;
        return 1;
      }
    }
  }
  if (*long_rook_has_init_pos == 1) {
    // Trying long castle
    int long_castle_rook_index = 8;
    int white_king_index = 12;
    if (table['1' - '1' + 1]['b' - 'a' + 1] == ';' &&
        table['1' - '1' + 1]['c' - 'a' + 1] == ';' &&
        table['1' - '1' + 1]['d' - 'a' + 1] == ';') {
      // It must be a clear path between king a rook
      int king_col = 'e' - 'a' + 1;
      int king_row = '1' - '1' + 1;
      int rook_col = 'a' - 'a' + 1;
      int rook_row = '1' - '1' + 1;

      int dest_king_col = 'c' - 'a' + 1;
      int dest_king_row = '1' - '1' + 1;
      int dest_rook_col = 'd' - 'a' + 1;
      int dest_rook_row = '1' - '1' + 1;
      new_table[king_row][king_col] = ';';
      new_table[rook_row][rook_col] = ';';
      new_table[dest_king_row][dest_king_col] = 'R';
      new_table[dest_rook_row][dest_rook_col] = 'T';

      char table2[9][9];
      for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
          table2[i][j] = table[i][j];
        }
      }
      table2['1' - '1' + 1]['c' - 'a' + 1] = 'R';
      table2[king_row][king_col] = ';';

      if (!is_king_attacked(new_table, last_black_move) &&
          !is_king_attacked(table2, last_black_move)) {
        cout << "move e1c1" << '\n';
        for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
            table[i][j] = new_table[i][j];
          }
        }
        last_white_move[white_king_index][0] = dest_king_col;
        last_white_move[white_king_index][1] = dest_king_row;
        last_white_move[long_castle_rook_index][0] = dest_rook_col;
        last_white_move[long_castle_rook_index][1] = dest_rook_row;
        *castled = 1;
        return 1;
      }
    }
  }
  return 0;
}


/*
  Function that generates all possible table configurations by moving the black king
*/
void generate_tables_black_king(char table[][9], char last_black_move[][2], int king_index,
                    char last_white_move[][2], 
                    Node *node) {
  
  
  char bot_move[4];

  bot_move[0] = last_black_move[king_index][0];
  bot_move[1] = last_black_move[king_index][1];

  int x_moves[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y_moves[] = {1, 0, -1, 1, -1, 1, 0, -1};

  for (int i = 0; i < 8; i++) {
    bot_move[3] = last_black_move[king_index][1] + x_moves[i];
    bot_move[2] = last_black_move[king_index][0] + y_moves[i];
    if (is_square_available_black_king(bot_move, table, last_white_move)) {

      int src_col = bot_move[0] - 'a' + 1;
      int src_row = bot_move[1] - '1' + 1;

      int dest_col = bot_move[2] - 'a' + 1;
      int dest_row = bot_move[3] - '1' + 1;

      char new_table[9][9];
      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }
      new_table[src_row][src_col] = ';';
      new_table[dest_row][dest_col] = 'K';

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


/*
  Function that generates all possible table configurations by moving a white knight
*/
void generate_tables_white_king(char table[][9], char last_pawn_move[][2], int king_index,
                    char last_black_move[][2],
                    Node *node) {
  char bot_move[4];

  bot_move[0] = last_pawn_move[king_index][0];
  bot_move[1] = last_pawn_move[king_index][1];

  int x_moves[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y_moves[] = {1, 0, -1, 1, -1, 1, 0, -1};

  for (int i = 0; i < 8; i++) {
    bot_move[3] = last_pawn_move[king_index][1] + x_moves[i];
    bot_move[2] = last_pawn_move[king_index][0] + y_moves[i];
    if (is_square_available_white_king(bot_move, table, last_black_move)) {
      char new_table[9][9];

      int src_col = bot_move[0] - 'a' + 1;
      int src_row = bot_move[1] - '1' + 1;

      int dest_col = bot_move[2] - 'a' + 1;
      int dest_row = bot_move[3] - '1' + 1;

      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }
      new_table[src_row][src_col] = ';';
      new_table[dest_row][dest_col] = 'R';

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