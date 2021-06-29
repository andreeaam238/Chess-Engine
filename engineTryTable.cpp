#include "others.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

/* Initializes the table with the defined pieces characters */
void init_table(char table[][9]) {
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
  for (int i = 1; i <= 8; i++) {
    for (int j = 1; j <= 8; j++) {
      table[i][j] = ';';
    }
  }

  // Initialize the white pieces
  table[1][1] = table[1][8] = 'T'; // Rook
  table[1][2] = table[1][7] = 'C'; // Knight
  table[1][3] = table[1][6] = 'N'; // Bishop
  table[1][4] = '*';               // Queen
  table[1][5] = 'R';               // King
  for (int i = 1; i <= 8; i++) {
    table[2][i] = 'P'; // Pawn
  }

  // Initialize the black pieces
  table[8][1] = table[8][8] = '|'; // Rook
  table[8][2] = table[8][7] = 'L'; // Knight
  table[8][3] = table[8][6] = 'B'; // Bishop
  table[8][4] = '#';               // Queen
  table[8][5] = 'K';               // King
  for (int i = 1; i <= 8; i++) {
    table[7][i] = 'S'; // Pawn
  }
}

void display_table(char table[][9]) {
  ofstream fout;
  fout.open("table.txt", ios_base::app);
  for (int i = 8; i > 0; i--) {
    for (int j = 1; j <= 8; j++) {
      fout << table[i][j];
    }
    fout << '\n';
  }
  fout << '\n';
  fout.close();
}

/* Updates the engine table */
char *update_table(char coords[], char table[][9], char last_pawn_move[][2],
                   bool bot_or_player) {

  // Sets the source column and row
  int src_col = coords[0] - 'a' + 1;
  int src_row = coords[1] - '1' + 1;

  // Sets the destination column and row
  int dest_col = coords[2] - 'a' + 1;
  int dest_row = coords[3] - '1' + 1;

  // Extracts the symbol from source position
  char symbol = table[src_row][src_col];

  // Make the source square empty on the table
  table[src_row][src_col] = ';';

  for (int i = 0; i < 16; ++i) {
    if (last_pawn_move[i][0] == coords[0] &&
        last_pawn_move[i][1] == coords[1]) {
      last_pawn_move[i][0] = coords[2];
      last_pawn_move[i][1] = coords[3];
      break;
    }
  }

  int r = rand() % 4;
  char *new_pos = (char *)calloc(5, sizeof(char));
  strncpy(new_pos, coords, 4);

  if (bot_or_player == true) {

    // Transform the pawn in queen if neccesary
    if (dest_row == 1 && symbol == 'S') {
      char new_symbol[][2] = {{'#', 'q'}, {'|', 'r'}, {'L', 'n'}, {'B', 'b'}};
      symbol = new_symbol[r][0];
      new_pos[4] = new_symbol[r][1];
    } else if (dest_row == 8 && symbol == 'P') {
      char new_symbol[][2] = {{'*', 'q'}, {'T', 'r'}, {'C', 'n'}, {'N', 'b'}};
      symbol = new_symbol[r][0];
      new_pos[4] = new_symbol[r][1];
    }
  } else {
    // Transform the pawn in queen if neccesary
    if (dest_row == 1 && symbol == 'S') {
      if (coords[4] == 'q') {
        symbol = '#';
      } else if (coords[4] == 'r') {
        symbol = '|';
      } else if (coords[4] == 'n') {
        symbol = 'L';
      } else if (coords[4] == 'b') {
        symbol = 'B';
      }
    } else if (dest_row == 8 && symbol == 'P') {
      if (coords[4] == 'q') {
        symbol = '*';
      } else if (coords[4] == 'r') {
        symbol = 'T';
      } else if (coords[4] == 'n') {
        symbol = 'C';
      } else if (coords[4] == 'b') {
        symbol = 'N';
      }
    } else if (symbol == 'P' && table[dest_row][dest_col] == ';' &&
               dest_row != src_row && dest_col != src_col) {

      if ((dest_row == src_row + 1 || dest_row == src_row - 1) &&
          (dest_col == src_col + 1 || dest_col == src_col - 1) &&
          table[src_row][dest_col] == 'S') {
        table[src_row][dest_col] = ';';
      }
    } else if (symbol == 'S' && table[dest_row][dest_col] == ';' &&
               dest_row != src_row && dest_col != src_col) {

      if ((dest_row == src_row + 1 || dest_row == src_row - 1) &&
          (dest_col == src_col + 1 || dest_col == src_col - 1) &&
          table[src_row][dest_col] == 'P') {
        table[src_row][dest_col] = ';';
      }
    } else if (symbol == 'R') {
      if (strncmp(coords, "e1g1", 4) == 0) {
        table[src_row][src_col + 1] = table[1][8];
        table[src_row][src_col] = ';';
        table[1][8] = ';';

        last_pawn_move[15][0] = coords[0] + 1;
        last_pawn_move[15][1] = coords[1];
      } else if (strncmp(coords, "e1c1", 4) == 0) {
        table[src_row][src_col - 1] = table[1][1];
        table[src_row][src_col] = ';';
        table[1][1] = ';';

        last_pawn_move[8][0] = coords[0] - 1;
        last_pawn_move[8][1] = coords[1];
      }
    } else if (symbol == 'K') {
      if (strncmp(coords, "e8g8", 4) == 0) {
        table[src_row][src_col + 1] = table[8][8];
        table[src_row][src_col] = ';';
        table[8][8] = ';';

        last_pawn_move[8][0] = coords[0] + 1;
        last_pawn_move[8][1] = coords[1];
      } else if (strncmp(coords, "e8c8", 4) == 0) {
        table[src_row][src_col - 1] = table[8][1];
        table[src_row][src_col] = ';';
        table[8][1] = ';';

        last_pawn_move[8][0] = coords[0] - 1;
        last_pawn_move[8][1] = coords[1];
      }
    }
  }

  // Update the destination position with the symbol
  table[dest_row][dest_col] = symbol;
  return new_pos;
}