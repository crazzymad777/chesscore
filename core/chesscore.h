/* Copyright (C) 2020 Yuri Moskov
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#define STATE_BIT_IS_BLACK_TURN 0
#define STATE_BIT_STALEMATE 1
#define STATE_BIT_WHITE_VICTORY 2
#define STATE_BIT_BLACK_VICTORY 3
#define STATE_BIT_A1_ROOK_MOVED 4
#define STATE_BIT_H1_ROOK_MOVED 5
#define STATE_BIT_A8_ROOK_MOVED 6
#define STATE_BIT_H8_ROOK_MOVED 7

#define CELL_NONE (char)(0)
#define CELL_WHITE_KING (char)(1)
#define CELL_WHITE_QUEEN (char)(2)
#define CELL_WHITE_ROOK (char)(3)
#define CELL_WHITE_BISHOP (char)(4)
#define CELL_WHITE_KNIGHT (char)(5)
#define CELL_WHITE_PAWN (char)(6)
#define CELL_BLACK_KING (char)(-1)
#define CELL_BLACK_QUEEN (char)(-2)
#define CELL_BLACK_ROOK (char)(-3)
#define CELL_BLACK_BISHOP (char)(-4)
#define CELL_BLACK_KNIGHT (char)(-5)
#define CELL_BLACK_PAWN (char)(-6)

#define PIECE_KING 1
#define PIECE_QUEEN 2
#define PIECE_ROOK 3
#define PIECE_BISHOP 4
#define PIECE_KNIGHT 5
#define PIECE_PAWN 6

#define CHESSCORE_WHITE (char)(1)
#define CHESSCORE_BLACK (char)(-1)

typedef struct {
	char state;
	char last_cell;
	char cells[64];
} game;

typedef struct{
	game* game_ptr;
	int index;
    int cells_between_index;
	char cell;
	char for_check;
    char cells_between[7];
	char* output_buffer;
} TurnContext;

#ifdef __cplusplus
extern "C" {
#endif

void cc_init(game* game_ptr);
// char cc_get_cell_id(int x, int y);
char cc_get_cell_id_by_id(int id);
char cc_get_x_cell(char cell);
char cc_get_y_cell(char cell);
void cc_get_turns(game* game_ptr, char cell, char output_buffer[28]);
char cc_get_piece(char piece);
int cc_is_piece_same_color(char a, char b);
int cc_get_potential_turns(game* game_ptr, char cell, char output_buffer[28]);
char cc_get_opposite_color(char piece);
char cc_get_colored_piece(char piece, char color);
int cc_find_piece(game* game_ptr, char piece);

#ifdef CHESSCORE_USE_INTERNAL_FUNCTIONS
// Internals

void cc_internal_init_context(TurnContext* context, game* game_ptr, char cell, char output_buffer[28]);
int cc_internal_get_potential_turns(/*@in@*/ TurnContext* context);
int cc_internal_get_index(int bitset, int x, int y);
int cc_internal_fill_line(TurnContext* context, int bitset, char offset);
int cc_internal_get_potential_pawn_turns(TurnContext* context);
int cc_internal_get_potential_king_turns(TurnContext* context);
int cc_internal_get_potential_knight_turns(TurnContext* context);
int cc_internal_fill_potential_hline(TurnContext* context);
int cc_internal_fill_potential_vline(TurnContext* context);
int cc_internal_fill_potential_dline7(TurnContext* context);
int cc_internal_fill_potential_dline9(TurnContext* context);

#define MASK_USE_X 0x1000
#define MASK_USE_Y 0x10000
#endif

#ifdef __cplusplus
}
#endif


