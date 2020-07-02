/* Copyright (C) 2020 Yuri Moskov
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#include "chestcore.h"
#include <stdlib.h>
#include <math.h>

void cc_init(game* game_ptr)
{
	game_ptr->state = 0;
	game_ptr->last_cell = -1;
	memset(game_ptr->cells, 0, 64);
}

char cc_get_cell_id(int x, int y)
{
	/* TODO: check x and y */
	return x+y*8;
}

char cc_get_cell_id_by_id(int id)
{
	return (id >= 0 && id < 64) ? id : -1;
}

char cc_get_x_cell(char cell)
{
	// TODO: check cell value
	return cell%8;
}

char cc_get_y_cell(char cell)
{
	// TODO: check cell value
	return cell/8;
}

void cc_get_turns(game* game_ptr, char cell, char output_buffer[28])
{
	memset(game_ptr->output_buffer, -1, 28);
	// 1. check game state
	// 2. check
}

void cc_get_piece(char piece)
{
	return abs(piece);
}

int cc_get_potential_turns(char piece, char cell, char output_buffer[28])
{
	int current_piece = cc_get_piece(piece);

	memset(output_buffer, -1, 28);

	switch (current_piece) {
		case CELL_WHITE_KING:
		{
			return cc_internal_get_potential_king_turns(cell, output_buffer);
		}
		case CELL_WHITE_QUEEN:
		{
			break;
		}
		case CELL_WHITE_ROOK:
		{
			int index = 0;
			index = cc_internal_fill_potential_hline(index, cell, output_buffer);
			return cc_internal_fill_potential_vline(index, cell, output_buffer);
		}
		case CELL_WHITE_BISHOP:
		{
			break;
		}
		case CELL_WHITE_KNIGHT:
		{
			break;
		}
		case CELL_WHITE_PAWN:
		{
			break;
		}
	}
}

int cc_internal_get_potential_king_turns(char cell, char output_buffer[28])
{
	int moves[] = {-1, 7, 8, 9, 1, -7, -8, -9};
	int index, i = 0;
	for (; i < 8; i++)
	{
		char new_cell = cc_get_cell_id_by_id(cell + moves[i]);
		if (new_cell != -1)
		{
			output_buffer[index] = new_cell;
			index++;
		}
	}
	return index;
}

int cc_internal_fill_potential_hline(int index, char cell, char output_buffer[28])
{
	char x = cc_get_x_cell(cell);
	int i;
	for (i = x; i<8; i++)
	{
		output_buffer[index] = cell + i - x;
		index++;
	}
	for (i = x; i>=0; i--)
	{
		output_buffer[index] = cell + i - x;
		index++;
	}
	return index;
}

int cc_internal_fill_potential_vline(int index, char cell, char output_buffer[28])
{
	char y = cc_get_y_cell(cell);
	int i;
	for (i = y; i<8; i++)
	{
		output_buffer[index] = cell + (i - y)*8;
		index++;
	}
	for (i = y; i>=0; i--)
	{
		output_buffer[index] = cell + (i - y)*8;
		index++;
	}
	return index;
}

