/* Copyright (C) 2020 Yuri Moskov
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#define CHESSCORE_USE_INTERNAL_FUNCTIONS 1

#include "chesscore.h"
#include <stdlib.h>
#include <string.h>

int max(int x, int y)
{
	return x>y?x:y;
}

void cc_init(game* game_ptr)
{
	static const char START_CELLS[64] = {
	CELL_WHITE_ROOK, CELL_WHITE_KNIGHT, CELL_WHITE_BISHOP, CELL_WHITE_QUEEN, CELL_WHITE_KING, CELL_WHITE_BISHOP, CELL_WHITE_KNIGHT, CELL_WHITE_ROOK,
	CELL_WHITE_PAWN, CELL_WHITE_PAWN, CELL_WHITE_PAWN, CELL_WHITE_PAWN, CELL_WHITE_PAWN, CELL_WHITE_PAWN, CELL_WHITE_PAWN, CELL_WHITE_PAWN,
	CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE,
	CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE,
	CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE,
	CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE, CELL_NONE,
	CELL_BLACK_PAWN, CELL_BLACK_PAWN, CELL_BLACK_PAWN, CELL_BLACK_PAWN, CELL_BLACK_PAWN, CELL_BLACK_PAWN, CELL_BLACK_PAWN, CELL_BLACK_PAWN,
	CELL_BLACK_ROOK, CELL_BLACK_KNIGHT, CELL_BLACK_BISHOP, CELL_BLACK_QUEEN, CELL_BLACK_KING, CELL_BLACK_BISHOP, CELL_BLACK_KNIGHT, CELL_BLACK_ROOK
	};

	game_ptr->state = 0;
	game_ptr->last_cell = -1;
	memcpy(game_ptr->cells, START_CELLS, 64);
}

char cc_get_cell_id(int x, int y)
{
	if (x < 0 || x >= 8 || y < 0 || y >= 8)
	{
		return -1;
	}
	return x + y * 8;
}

char cc_get_cell_id_by_id(int id)
{
	return (id >= 0 && id < 64) ? id : -1;
}

char cc_get_x_cell(char cell)
{
	if (cell < 0 || cell >= 64)
	{
		return -1;
	}
	return cell % 8;
}

char cc_get_y_cell(char cell)
{
	if (cell < 0 || cell >= 64)
	{
		return -1;
	}
	return cell / 8;
}

void cc_get_turns(game* game_ptr, char cell, char output_buffer[28])
{
	memset(output_buffer, -1, 28);

	if (game_ptr->state & 1 << STATE_BIT_STALEMATE || game_ptr->state & 1 << STATE_BIT_WHITE_VICTORY || game_ptr->state & 1 << STATE_BIT_BLACK_VICTORY)
	{
		// Game over
		return;
	}

	// 2. check
}

char cc_get_piece(char piece)
{
	return abs(piece);
}

int cc_is_piece_same_color(char a, char b)
{
	return (a > 0 && b > 0) || (a < 0 && b < 0);
}

int cc_get_potential_turns(game* game_ptr, char cell, char output_buffer[28])
{
	int current_piece = cc_get_piece(game_ptr->cells[cell]);

	memset(output_buffer, -1, 28);

	switch (current_piece) {
		case CELL_WHITE_KING:
		{
			return cc_internal_get_potential_king_turns(game_ptr, cell, output_buffer);
		}
		case CELL_WHITE_QUEEN:
		{
			int index = 0;
			index = cc_internal_fill_potential_hline(game_ptr, index, cell, output_buffer);
			index = cc_internal_fill_potential_vline(game_ptr, index, cell, output_buffer);
			index = cc_internal_fill_potential_dline7(game_ptr, index, cell, output_buffer);
			return cc_internal_fill_potential_dline9(game_ptr, index, cell, output_buffer);
		}
		case CELL_WHITE_ROOK:
		{
			int index = 0;
			index = cc_internal_fill_potential_hline(game_ptr, index, cell, output_buffer);
			return cc_internal_fill_potential_vline(game_ptr, index, cell, output_buffer);
		}
		case CELL_WHITE_BISHOP:
		{
			int index = 0;
			index = cc_internal_fill_potential_dline7(game_ptr, index, cell, output_buffer);
			return cc_internal_fill_potential_dline9(game_ptr, index, cell, output_buffer);
		}
		case CELL_WHITE_KNIGHT:
		{
			return cc_internal_get_potential_knight_turns(game_ptr, cell, output_buffer);
		}
		case CELL_WHITE_PAWN:
		{
			break;
		}
	}
}

int cc_internal_get_potential_king_turns(game* game_ptr, char cell, char output_buffer[28])
{
	int moves[] = {-1, 7, 8, 9, 1, -7, -8, -9};
	int index = 0, i = 0;
	for (; i < 8; i++)
	{
		char new_cell = cc_get_cell_id_by_id(cell + moves[i]);
		if (new_cell != -1)
		{
			char piece = game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, game_ptr->cells[cell]))
			{
				output_buffer[index] = new_cell;
				index++;
			}
		}
	}
	return index;
}

int cc_internal_get_potential_knight_turns(game* game_ptr, char cell, char output_buffer[28])
{
	int moves[] = {6, 15, 17, 10, -6, -15, -17, -10};
	int index = 0, i = 0;
	for (; i < 8; i++)
	{
		char new_cell = cc_get_cell_id_by_id(cell + moves[i]);
		if (new_cell != -1)
		{
			char piece = game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, game_ptr->cells[cell]))
			{
				output_buffer[index] = new_cell;
				index++;
			}
		}
	}
	return index;
}

int cc_internal_get_index(int bitset, int x, int y)
{
	if (bitset & MASK_USE_X)
	{
		return x;
	}
	if (bitset & MASK_USE_Y)
	{
		return y;
	}
	// Something wrong?
	return -1;
}

int cc_internal_fill_line(game* game_ptr, int bitset, char offset, int index, char cell, char output_buffer[28])
{
	char x = cc_get_x_cell(cell);
	char y = cc_get_y_cell(cell);
	int i;

	i = cc_internal_get_index(bitset, x, y) + 1;
	for (; i < 8; i++)
	{
		char new_cell = cell + (i - x) * offset;
		if (new_cell >=0 && new_cell < 64)
		{
			char piece = game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, game_ptr->cells[cell]))
			{
				output_buffer[index] = new_cell;
				index++;
			}
			if (piece != CELL_NONE)
			{
				break;
			}
		}
	}
	i = cc_internal_get_index(bitset, x, y) - 1;
	for (; i >= 0; i--)
	{
		char new_cell = cell + (i - x) * offset;
		if (new_cell >=0 && new_cell < 64)
		{
			char piece = game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, game_ptr->cells[cell]))
			{
				output_buffer[index] = new_cell;
				index++;
			}
			if (piece != CELL_NONE)
			{
				break;
			}
		}
	}
	return index;
}

int cc_internal_fill_potential_hline(game* game_ptr, int index, char cell, char output_buffer[28])
{
	return cc_internal_fill_line(game_ptr, MASK_USE_X, 1, index, cell, output_buffer);
}

int cc_internal_fill_potential_vline(game* game_ptr, int index, char cell, char output_buffer[28])
{
	return cc_internal_fill_line(game_ptr, MASK_USE_Y, 1, index, cell, output_buffer);
}

int cc_internal_fill_potential_dline7(game* game_ptr, int index, char cell, char output_buffer[28])
{
	return cc_internal_fill_line(game_ptr, MASK_USE_X | MASK_USE_Y, 7, index, cell, output_buffer);
}

int cc_internal_fill_potential_dline9(game* game_ptr, int index, char cell, char output_buffer[28])
{
	return cc_internal_fill_line(game_ptr, MASK_USE_X | MASK_USE_Y, 9, index, cell, output_buffer);
}

