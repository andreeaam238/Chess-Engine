#include <fstream>
#include <iostream>
#include <string.h>
#include <list>
#include <iterator>

using namespace std;

#define MINUS_INF -999999
#define PLUS_INF 999999

struct Node {
    char **table_config;
    char move[5];
    list<Node> children;
};

// Internal table functions
void init_table(char table[][9]);
void display_table(char table[][9]);
char *update_table(char coords[], char table[][9], char last_pawn_move[][2],
                   bool bot_or_player);

int is_square_available_black(char coords[], char table[][9]);
int is_square_available_white(char coords[], char table[][9]);
int move_pawn_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_white_move[][2]);
int move_pawn_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_black_move[][2]);

int is_square_available_white_horse(char coords[], char table[][9]);
int is_square_available_black_horse(char coords[], char table[][9]);
int move_horse_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_black_move[][2]);
int move_horse_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_white_move[][2]);

int can_attack_pawn(char table[][9], int col, int row);
int can_attack_bishop(char table[][9], int col, int row);
int can_attack_rook(char table[][9], int col, int row);
int can_attack_horse(char table[][9], int col, int row);

int can_attack_pawn_white(char table[][9], int col, int row);
int can_attack_bishop_white(char table[][9], int col, int row);
int can_attack_rook_white(char table[][9], int col, int row);
int can_attack_horse_white(char table[][9], int col, int row);

int is_king_attacked(char table[][9], char last_white_move[][2]);
int is_king_attacked_white(char table[][9], char last_black_move[][2]);
int is_square_available_white_king(char coords[], char table[][9]);
int is_square_available_black_king(char coords[], char table[][9]);
int move_king_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_black_move[][2]);
int move_king_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_white_move[][2]);
int protect_king_white(char table[][9], char last_black_move[][2],
                       char last_white_move[][2], char *last_move);
int protect_king_black(char table[][9], char last_black_move[][2],
                       char last_white_move[][2], char *last_move);
int can_black_king_castle(char table[][9], int *king_has_init_pos,
                          int *short_rook_has_init_pos,
                          int *long_rook_has_init_pos, int *castled,
                          char last_white_move[][2], char last_black_move[][2]);
int can_white_king_castle(char table[][9], int *king_has_init_pos,
                          int *short_rook_has_init_pos,
                          int *long_rook_has_init_pos, int *castled,
                          char last_white_move[][2], char last_black_move[][2]);

int is_square_available_black_bishop(char coords[], char table[][9]);
int is_square_available_white_bishop(char coords[], char table[][9]);
int move_bishop_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                      char last_black_move[][2]);
int move_bishop_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                      char last_white_move[][2]);

int is_square_available_black_rook(char coords[], char table[][9]);
int is_square_available_white_rook(char coords[], char table[][9]);
int move_rook_black(char table[][9], char last_move[][2], int piece_index,
                    char last_white_move[][2]);
int move_rook_white(char table[][9], char last_move[][2], int piece_index,
                    char last_black_move[][2]);

int is_square_available_black_queen(char coords[], char table[][9]);
int move_queen_black(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_white_move[][2]);
int move_queen_white(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_black_move[][2]);

void execute_move(char bot_move[], char last_pawn_move[][2], char table[][9]);

void generate_tables_black_rook(char table[][9], char last_move[][2], int piece_index,
  char last_white_move[][2], Node *node);
void generate_tables_white_rook(char table[][9], char last_move[][2], int piece_index,
  char last_black_move[][2], Node *node);
void generate_tables_black_bishop(char table[][9], char last_pawn_move[][2], int pawn_index,
  char last_white_move[][2], Node *node);
void generate_tables_white_bishop(char table[][9], char last_pawn_move[][2], int pawn_index,
  char last_black_move[][2], Node *node);
void generate_tables_black_horse(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_white_move[][2], Node *node);
void generate_tables_white_horse(char table[][9], char last_pawn_move[][2], int pawn_index,
                     char last_black_move[][2], Node *node);

void generate_tables_black_king(char table[][9], char last_black_move[][2], int king_index,
                    char last_white_move[][2], 
                    Node *node);
void generate_tables_white_king(char table[][9], char last_pawn_move[][2], int king_index,
                    char last_black_move[][2],
                    Node *node);
void generate_tables_white_pawn(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_black_move[][2], Node *node);
void generate_tables_black_pawn(char table[][9], char last_pawn_move[][2], int pawn_index,
                    char last_move[], char last_white_move[][2], Node *node);
void generate_tables_black_queen(char table[][9], char last_pawn_move[][2], int pawn_index,
  char last_white_move[][2], Node *node);
void generate_tables_white_queen(char table[][9], char last_pawn_move[][2], int pawn_index,
  char last_black_move[][2], Node *node);

int minimax(Node node_address, int depth, int initial_depth, bool black_or_white, int alpha, int beta, Node *right_move,  char last_black_move[][2],
                      char last_white_move[][2], char *last_move);
void generate_positions(Node *node, bool black_or_white,
                      char last_black_move[][2], char last_white_move[][2], char *last_move);
int eval_table(Node node);