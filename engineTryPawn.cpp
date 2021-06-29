#include "others.h"
#include <fstream>
#include <iostream>
#include <string.h>

#include <list>
#include <iterator>

using namespace std;

/* Function that tries to move the given black pawn
   It checks every move to be safe for the king */
int is_square_available_black(char coords[], char table[][9]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char black_pieces[] = "S|LB#K";

  // Checks if the current piece to be moved is the black pawn
  if (table[src_row][src_col] != 'S') {
    return 0;
  }

  // Checks if the destination has a pawn with the same color
  if (strchr(black_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  // Checks if the destination is still on the table
  if (dest_row < 1 || dest_row > 8 || dest_col < 1 || dest_col > 8)
    return 0;

  // Checks if the pawn can go forward
  if (table[dest_row][dest_col] == ';' &&
      ((dest_col == src_col && dest_row == src_row - 1) ||
       (src_row == 7 && dest_row == src_row - 2))) {
    return 1;

    // Checks if the pawn can capture to the left (of the white pieces)
  } else if (table[dest_row][dest_col] != ';' && dest_col == src_col - 1 &&
             dest_row == src_row - 1) {

    // and the piece is not the same color
    if (!strchr(black_pieces, table[dest_row][dest_col]))
      return 1;

    // Checks if the pawn can capture to the right
  } else if (table[dest_row][dest_col] != ';' && dest_col == src_col + 1 &&
             dest_row == src_row - 1) {
    if (!strchr(black_pieces, table[dest_row][dest_col]))
      return 1;
  }

  return 0;
}

/* Function that tries to move the given white pawn
   It checks every move to be safe for the king */
int is_square_available_white(char coords[], char table[][9]) {
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  char white_pieces[] = "PTCNR*";

  // Checks if the current piece to be moved is the white pawn
  if (table[src_row][src_col] != 'P') {
    return 0;
  }

  // Checks if the destination has a pawn with the same color
  if (strchr(white_pieces, table[dest_row][dest_col]) != NULL) {
    return 0;
  }

  // Checks if the destination is still on the table
  if (dest_row < 1 || dest_row > 8 || dest_col < 1 || dest_col > 8)
    return 0;

  // Checks if the pawn can go forward
  if (table[dest_row][dest_col] == ';' &&
      ((dest_col == src_col && dest_row == src_row + 1) ||
       (src_row == 2 && dest_row == src_row + 2))) {
    return 1;

    // Checks if the pawn can capture to the left (of the white pieces)
  } else if (table[dest_row][dest_col] != ';' && dest_col == src_col - 1 &&
             dest_row == src_row + 1) {
    if (!strchr(white_pieces, table[dest_row][dest_col]))
      return 1;

    // Checks if the pawn can capture to the right
  } else if (table[dest_row][dest_col] != ';' && dest_col == src_col + 1 &&
             dest_row == src_row + 1) {
    if (!strchr(white_pieces, table[dest_row][dest_col]))
      return 1;
  }

  return 0;
}

/* Function that tries to move the given black pawn
   It checks every move to be safe for the king */
int move_pawn_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_white_move[][2]) {
  char bot_move[4];

  // At the beginning the pawn has no previous move
  // We check if we can move 2 squares forward, 1 square forward, capture
  // diagonally or make an en passant move

  // If it's at the first move try to move it two squares forward if possible.
  if (last_pawn_move[pawn_index][0] == 'a' + pawn_index &&
      last_pawn_move[pawn_index][1] == '7') {
    char pos[4];

    pos[0] = 'a' + pawn_index;
    pos[2] = pos[0];

    pos[1] = '7';
    pos[3] = '6';

    if (is_square_available_black(pos, table)) {
      pos[3] = '5';
      if (is_square_available_black(pos, table)) {
        char new_table[9][9];
        for (int i = 0; i <= 8; i++) {
          for (int j = 0; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }
        int src_row = pos[1] - '1' + 1;
        int src_col = pos[0] - 'a' + 1;
        int dest_row = pos[3] - '1' + 1;
        int dest_col = pos[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = 'S';
        if (!is_king_attacked(new_table, last_white_move)) {
          execute_move(pos, last_pawn_move, table);
          return 1;
        }
      }
    }
  }

  // The pawn's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The enemy's last moved piece coordinates
  int dest_col = last_move[2] - 'a' + 1;
  int dest_row = last_move[3] - '1' + 1;

  // The pawn's current coordinates
  int cur_col = last_pawn_move[pawn_index][0] - 'a' + 1;
  int cur_row = last_pawn_move[pawn_index][1] - '1' + 1;

  // Check if an 'en passant' move can be made
  if (abs(last_move[3] - last_move[1]) == 2 &&
      table[dest_row][dest_col] == 'P' &&
      abs(last_pawn_move[pawn_index][0] - last_move[2]) == 1 &&
      table[cur_row][cur_col] == 'S' && bot_move[1] == last_move[3]) {
    bot_move[2] = last_move[2];
    bot_move[3] = last_move[3] - 1;

    table[dest_row][dest_col] = ';';

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
    new_table[dest_row][dest_col] = 'S';
    if (!is_king_attacked(new_table, last_white_move)) {
      execute_move(bot_move, last_pawn_move, table);
      return 1;
    }
  }

  bot_move[3] = last_pawn_move[pawn_index][1] - 1;

  // Check if the pawn can be moved one square forward or capture diagonally.

  int y_moves[] = {-1, 1, 0};

  for (int i = 0; i < 3; ++i) {
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];
    if (is_square_available_black(bot_move, table)) {
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
      new_table[dest_row][dest_col] = 'S';
      if (!is_king_attacked(new_table, last_white_move)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }

  return 0;
}

/* Function that tries to move the given white pawn
   It checks every move to be safe for the king */
int move_pawn_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_black_move[][2]) {
  char bot_move[4];

  // At the beginning the pawn has no previous move
  // We check if we can move 2 squares forward, 1 square forward, capture
  // diagonally or make an en passant move

  // If it's at the first move try to move it two squares forward if possible.
  if (last_pawn_move[pawn_index][0] == 'a' + pawn_index &&
      last_pawn_move[pawn_index][1] == '2') {
    char pos[4];

    pos[0] = 'a' + pawn_index;
    pos[2] = pos[0];

    pos[1] = '2';
    pos[3] = '3';

    if (is_square_available_white(pos, table)) {
      pos[3] = '4';
      if (is_square_available_white(pos, table)) {
        char new_table[9][9];
        for (int i = 0; i <= 8; i++) {
          for (int j = 0; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }
        int src_row = pos[1] - '1' + 1;
        int src_col = pos[0] - 'a' + 1;
        int dest_row = pos[3] - '1' + 1;
        int dest_col = pos[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = 'P';
        if (!is_king_attacked_white(new_table, last_black_move)) {
          execute_move(pos, last_pawn_move, table);
          return 1;
        }
      }
    }
  }

  // The pawn's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The enemy's last moved piece coordinates
  int dest_col = last_move[2] - 'a' + 1;
  int dest_row = last_move[3] - '1' + 1;

  // The pawn's current coordinates
  int cur_col = last_pawn_move[pawn_index][0] - 'a' + 1;
  int cur_row = last_pawn_move[pawn_index][1] - '1' + 1;

  // Check if an 'en passant' move can be made
  if (abs(last_move[3] - last_move[1]) == 2 &&
      table[dest_row][dest_col] == 'S' &&
      abs(last_pawn_move[pawn_index][0] - last_move[2]) == 1 &&
      table[cur_row][cur_col] == 'P' && bot_move[1] == last_move[3]) {
    bot_move[2] = last_move[2];
    bot_move[3] = last_move[3] + 1;
    table[dest_row][dest_col] = ';';

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
    new_table[dest_row][dest_col] = 'P';
    if (!is_king_attacked_white(new_table, last_black_move)) {
      execute_move(bot_move, last_pawn_move, table);
      return 1;
    }
  }

  bot_move[3] = last_pawn_move[pawn_index][1] + 1;

  // Check if the pawn can be moved one square forward or capture diagonally.

  int y_moves[] = {-1, 1, 0};

  for (int i = 0; i < 3; ++i) {
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];
    if (is_square_available_white(bot_move, table)) {
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
      new_table[dest_row][dest_col] = 'P';
      if (!is_king_attacked_white(new_table, last_black_move)) {
        execute_move(bot_move, last_pawn_move, table);
        return 1;
      }
    }
  }
  return 0;
}


/*
  Function that generates all possible table configurations by moving a black pawn
*/
void generate_tables_black_pawn(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_white_move[][2], Node *node) {

  char bot_move[4];

  // At the beginning the pawn has no previous move
  // We check if we can move 2 squares forward, 1 square forward, capture
  // diagonally or make an en passant move

  // If it's at the first move try to move it two squares forward if possible.
  if (last_pawn_move[pawn_index][0] == 'a' + pawn_index &&
      last_pawn_move[pawn_index][1] == '7') {
    
    char pos[4];

    pos[0] = 'a' + pawn_index;
    pos[2] = pos[0];

    pos[1] = '7';
    pos[3] = '6';

    if (is_square_available_black(pos, table)) {
      
      pos[3] = '5';
      if (is_square_available_black(pos, table)) {
        
        char new_table[9][9];
        for (int i = 1; i <= 8; i++) {
          for (int j = 1; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }
        
        int src_row = pos[1] - '1' + 1;
        int src_col = pos[0] - 'a' + 1;
        int dest_row = pos[3] - '1' + 1;
        int dest_col = pos[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = 'S';
        if (!is_king_attacked(new_table, last_white_move)) {
          
          char **new_table1 = (char **)malloc(9 * sizeof(char *));
          for (int i = 0; i < 9; i++) {
            new_table1[i] = (char *)malloc(9 * sizeof(char));
          }
          
          for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
          Node child;
          child.table_config = new_table1;
          child.move[0] = pos[0];
          child.move[1] = pos[1];
          child.move[2] = pos[2];
          child.move[3] = pos[3];
          child.move[4] = '\0';
          node->children.push_back(child);
        }
      }
    }
  }
  

  // The pawn's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The enemy's last moved piece coordinates
  int dest_col = last_move[2] - 'a' + 1;
  int dest_row = last_move[3] - '1' + 1;

  // The pawn's current coordinates
  int cur_col = last_pawn_move[pawn_index][0] - 'a' + 1;
  int cur_row = last_pawn_move[pawn_index][1] - '1' + 1;

  // Check if an 'en passant' move can be made
  if (abs(last_move[3] - last_move[1]) == 2 &&
      table[dest_row][dest_col] == 'P' &&
      abs(last_pawn_move[pawn_index][0] - last_move[2]) == 1 &&
      table[cur_row][cur_col] == 'S' && bot_move[1] == last_move[3]) {
   
    bot_move[2] = last_move[2];
    bot_move[3] = last_move[3] - 1;

    table[dest_row][dest_col] = ';';

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
    new_table[dest_row][dest_col] = 'S';
    if (!is_king_attacked(new_table, last_white_move)) {
      char **new_table1 = (char **)malloc(9 * sizeof(char *));
      for (int i = 0; i < 9; i++) {
        new_table1[i] = (char *)malloc(9 * sizeof(char));
      }
      for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
      Node child;
      child.table_config = new_table1;
      child.move[0] = bot_move[0];
      child.move[1] = bot_move[1];
      child.move[2] = bot_move[2];
      child.move[3] = bot_move[3];
      child.move[4] = '\0';
      node->children.push_back(child);
    }
  }

  bot_move[3] = last_pawn_move[pawn_index][1] - 1;

  // Check if the pawn can be moved one square forward or capture diagonally.

  int y_moves[] = {-1, 1, 0};

  for (int i = 0; i < 3; ++i) {
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];
    if (is_square_available_black(bot_move, table)) {
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
      new_table[dest_row][dest_col] = 'S';
      if (!is_king_attacked(new_table, last_white_move)) {
        char **new_table1 = (char **)malloc(9 * sizeof(char *));
        for (int i = 0; i < 9; i++) {
          new_table1[i] = (char *)malloc(9 * sizeof(char));
        }
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
        Node child;
        child.table_config = new_table1;
        child.move[0] = bot_move[0];
        child.move[1] = bot_move[1];
        child.move[2] = bot_move[2];
        child.move[3] = bot_move[3];
        child.move[4] = '\0';
        node->children.push_back(child);
      }
    }
  }
}


/*
  Function that generates all possible table configurations by moving a white knight
*/

void generate_tables_white_pawn(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_black_move[][2], Node *node) {
  char bot_move[4];

  // At the beginning the pawn has no previous move
  // We check if we can move 2 squares forward, 1 square forward, capture
  // diagonally or make an en passant move

  // If it's at the first move try to move it two squares forward if possible.
  if (last_pawn_move[pawn_index][0] == 'a' + pawn_index &&
      last_pawn_move[pawn_index][1] == '2') {
    char pos[4];

    pos[0] = 'a' + pawn_index;
    pos[2] = pos[0];

    pos[1] = '2';
    pos[3] = '3';

    if (is_square_available_white(pos, table)) {
      pos[3] = '4';
      if (is_square_available_white(pos, table)) {
        char new_table[9][9];
        for (int i = 1; i <= 8; i++) {
          for (int j = 1; j <= 8; j++) {
            new_table[i][j] = table[i][j];
          }
        }
        int src_row = pos[1] - '1' + 1;
        int src_col = pos[0] - 'a' + 1;
        int dest_row = pos[3] - '1' + 1;
        int dest_col = pos[2] - 'a' + 1;
        new_table[src_row][src_col] = ';';
        new_table[dest_row][dest_col] = 'P';
        if (!is_king_attacked_white(new_table, last_black_move)) {
          char **new_table1 = (char **)malloc(9 * sizeof(char *));
          for (int i = 0; i < 9; i++) {
            new_table1[i] = (char *)malloc(9 * sizeof(char));
          }
          for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
          Node child;
          child.table_config = new_table1;
          child.move[0] = pos[0];
          child.move[1] = pos[1];
          child.move[2] = pos[2];
          child.move[3] = pos[3];
          child.move[4] = '\0';
          node->children.push_back(child);
        }
      }
    }
  }

  // The pawn's current position
  bot_move[0] = last_pawn_move[pawn_index][0];
  bot_move[1] = last_pawn_move[pawn_index][1];

  // The enemy's last moved piece coordinates
  int dest_col = last_move[2] - 'a' + 1;
  int dest_row = last_move[3] - '1' + 1;

  // The pawn's current coordinates
  int cur_col = last_pawn_move[pawn_index][0] - 'a' + 1;
  int cur_row = last_pawn_move[pawn_index][1] - '1' + 1;

  // Check if an 'en passant' move can be made
  if (abs(last_move[3] - last_move[1]) == 2 &&
      table[dest_row][dest_col] == 'S' &&
      abs(last_pawn_move[pawn_index][0] - last_move[2]) == 1 &&
      table[cur_row][cur_col] == 'P' && bot_move[1] == last_move[3]) {
    bot_move[2] = last_move[2];
    bot_move[3] = last_move[3] + 1;
    table[dest_row][dest_col] = ';';

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
    new_table[dest_row][dest_col] = 'P';
    if (!is_king_attacked_white(new_table, last_black_move)) {
        char **new_table1 = (char **)malloc(9 * sizeof(char *));
        for (int i = 0; i < 9; i++) {
          new_table1[i] = (char *)malloc(9 * sizeof(char));
        }
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
        Node child;
        child.table_config = new_table1;
        child.move[0] = bot_move[0];
        child.move[1] = bot_move[1];
        child.move[2] = bot_move[2];
        child.move[3] = bot_move[3];
        child.move[4] = '\0';
        node->children.push_back(child);
    }
  }

  bot_move[3] = last_pawn_move[pawn_index][1] + 1;

  // Check if the pawn can be moved one square forward or capture diagonally.

  int y_moves[] = {-1, 1, 0};

  for (int i = 0; i < 3; ++i) {
    bot_move[2] = last_pawn_move[pawn_index][0] + y_moves[i];
    if (is_square_available_white(bot_move, table)) {
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
      new_table[dest_row][dest_col] = 'P';
      if (!is_king_attacked_white(new_table, last_black_move)) {
        char **new_table1 = (char **)malloc(9 * sizeof(char *));
        for (int i = 0; i < 9; i++) {
          new_table1[i] = (char *)malloc(9 * sizeof(char));
        }
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
              new_table1[i][j] = new_table[i][j];
            }
          }
        Node child;
        child.table_config = new_table1;
        child.move[0] = bot_move[0];
        child.move[1] = bot_move[1];
        child.move[2] = bot_move[2];
        child.move[3] = bot_move[3];
        child.move[4] = '\0';
        node->children.push_back(child);
      }
    }
  }
}