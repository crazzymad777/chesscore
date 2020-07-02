/* Copyright (C) 2020 Yuri Moskov
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#define STATE_BIT_IS_WHITE_TURN 0
#define STATE_BIT_STALEMATE 1
#define STATE_BIT_WHITE_VICTORY 2
#define STATE_BIT_BLACK_VICTORY 3
#define STATE_BIT_A1_ROOK_MOVED 4
#define STATE_BIT_H1_ROOK_MOVED 5
#define STATE_BIT_A8_ROOK_MOVED 6
#define STATE_BIT_H8_ROOK_MOVED 7

#define CELL_NONE 0
#define CELL_WHITE_KING 1
#define CELL_WHITE_QUEEN 2
#define CELL_WHITE_ROOK 3
#define CELL_WHITE_BISHOP 4
#define CELL_WHITE_KNIGHT 5
#define CELL_WHITE_PAWN 6
#define CELL_BLACK_KING -1
#define CELL_BLACK_QUEEN -2
#define CELL_BLACK_ROOK -3
#define CELL_BLACK_BISHOP -4
#define CELL_BLACK_KNIGHT -5
#define CELL_BLACK_PAWN -6

struct game {
	char state;
	char last_cell;
	char cells[64];
};

void cc_init(game* game_ptr);
// char cc_get_cell_id(int x, int y);
char cc_get_cell_id_by_id(int id);
void cc_get_turns(game* game_ptr, char cell, char output_buffer[28]);
void cc_get_piece(char piece);
void cc_get_potential_turns(char piece, char cell, char output_buffer[28]);
void cc_internal_get_potential_king_turns(char cell, char output_buffer[28]);

