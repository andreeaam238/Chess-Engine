#include <iostream>
#include <unordered_map>
#include <string.h>
#include "others.h"

using namespace std;

/* Function that evaluates the current table configuration and returns a value */
int eval_table(Node node) {
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

    unordered_map<char, int> piece_and_val;

    piece_and_val['|'] = -50;
    piece_and_val['L'] = -30;
    piece_and_val['B'] = -30;
    piece_and_val['#'] = -90;
    piece_and_val['K'] = -900;
    piece_and_val['S'] = -10;

    piece_and_val['T'] = 50;
    piece_and_val['C'] = 30;
    piece_and_val['N'] = 30;
    piece_and_val['*'] = 90;
    piece_and_val['R'] = 900;
    piece_and_val['P'] = 10;

    int sum = 0;

    for (int i = 1; i < 9; i++) {
        for (int j = 1; j < 9; j++) {
           sum += piece_and_val[node.table_config[i][j]];
        }
    }

    return sum;
}

/* 
    From a given configuration of the table, depending on who has to move, 
    we generate all possible future configurations
*/
void generate_positions(Node *node, bool black_or_white,
                      char last_black_move[][2], char last_white_move[][2], char *last_move) {
    char **table = node->table_config;

    char new_table[9][9];

      for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
          new_table[i][j] = table[i][j];
        }
      }

    if (black_or_white == false) {
        for (int i = 0; i < 9; i++) {
            int dest_col = last_black_move[i][0] - 'a' + 1;
            int dest_row = last_black_move[i][1] - '1' + 1;
            
            switch(table[dest_row][dest_col]){
                case 'S':
                    generate_tables_black_pawn(new_table, last_black_move, i,
                        last_move, last_white_move, node);
                    break;
                case 'B':
                    generate_tables_black_bishop(new_table, last_black_move, i,
                        last_white_move, node);
                    break;
                case 'L':
                    generate_tables_black_horse(new_table, last_black_move, i,
                        last_white_move, node);
                    break;
                case '|':
                    generate_tables_black_rook(new_table, last_black_move, i,
                        last_white_move, node);
                    break;
                case '#':
                    generate_tables_black_queen(new_table, last_black_move, i,
                        last_white_move, node);
                    break;
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; i++) {
            int dest_col = last_white_move[i][0] - 'a' + 1;
            int dest_row = last_white_move[i][1] - '1' + 1;
            switch(table[dest_row][dest_col]){
                case 'P':
                    generate_tables_white_pawn(new_table, last_white_move, i,
                        last_move, last_black_move, node);
                    break;
                case 'B':
                    generate_tables_white_bishop(new_table, last_white_move, i,
                        last_black_move, node);
                    break;
                case 'L':
                    generate_tables_white_horse(new_table, last_white_move, i,
                        last_black_move, node);
                    break;
                case '|':
                    generate_tables_white_rook(new_table, last_white_move, i,
                        last_black_move, node);
                    break;
                case '#':
                    generate_tables_white_queen(new_table, last_white_move, i,
                        last_black_move, node);
                    break;
            }
        }
    }

    if (black_or_white == false) {
                
        generate_tables_black_rook(new_table, last_black_move, 8,
                last_white_move, node);
        generate_tables_black_rook(new_table, last_black_move, 15,
                last_white_move, node);
                     
        generate_tables_black_horse(new_table, last_black_move, 9,
                last_white_move, node);
        generate_tables_black_horse(new_table, last_black_move, 14,
                last_white_move, node);
                       
        generate_tables_black_bishop(new_table, last_black_move, 10,
                last_white_move, node);
        generate_tables_black_bishop(new_table, last_black_move, 13,
                last_white_move, node);
                        
        generate_tables_black_queen(new_table, last_black_move, 11,
                last_white_move, node);
                        
        generate_tables_black_king(new_table, last_black_move, 12,
                last_white_move, node);
                     
    }
    else {
                
        generate_tables_white_rook(new_table, last_white_move, 8,
                last_black_move, node);
        generate_tables_white_rook(new_table, last_white_move, 15,
                last_black_move, node);
                      
        generate_tables_white_horse(new_table, last_white_move, 9, last_black_move, node);
        generate_tables_white_horse(new_table, last_white_move, 14, last_black_move, node);
                        
        generate_tables_white_bishop(new_table, last_white_move, 10,
                last_black_move, node);
        generate_tables_white_bishop(new_table, last_white_move, 13,
                last_black_move, node);
                    
        generate_tables_white_queen(new_table, last_white_move, 11,
                last_black_move, node);
                        
        generate_tables_white_king(new_table, last_white_move, 12,
                last_black_move, node);
    }
}

/*
    Implementation of the minimax algorithm with alpha-beta prunning optimization
*/
int minimax(Node node_address, int depth, int initial_depth, bool black_or_white, int alpha, int beta, Node *right_move,  char last_black_move[][2],
                      char last_white_move[][2], char *last_move) {


    if (depth == 0) {
        return eval_table(node_address);
    } else {
        generate_positions(&node_address, black_or_white,
                      last_black_move, last_white_move, last_move);
        // Am generat toate configuratiile tablei dupa ce a mutat o anumita culoare
        if (black_or_white == false) {
            int best = MINUS_INF;

            list <Node> :: iterator it;
            for(it = node_address.children.begin(); it != node_address.children.end(); ++it) {
                int val = minimax(*it, depth - 1, initial_depth, true, alpha, beta, right_move, last_black_move,
                    last_white_move, last_move);
                
                int aux = max(best, val);
                if (aux != best && ((initial_depth - depth) % 2) == 0 ) {
                    *right_move = (*it);
                    best = aux;
                }
                alpha = max(alpha, best);
            
                if (beta <= alpha) {
                    break;
                }
            }
            return best;
        } else {
            int best = PLUS_INF;

            list <Node> :: iterator it;
            for(it = node_address.children.begin(); it != node_address.children.end(); ++it) {
                int val = minimax(*it, depth - 1, initial_depth, false, alpha, beta, right_move, last_black_move,
                    last_white_move, last_move);
                int aux = min(best, val);
                if (aux != best && ((initial_depth - depth) % 2) == 0) {
                    *right_move = (*it);
                    best = aux;
                }
                beta = min(beta, best);
                
                if (beta <= alpha) {
                    break;
                }
            }
            return best;
        }
    }
}