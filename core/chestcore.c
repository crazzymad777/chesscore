/* Copyright (C) 2020 Yuri Moskov
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#define CHESTCORE_USE_INTERNAL_FUNCTIONS 1

#include "chestcore.h"
#include <stdlib.h>
#include <string.h>

int max(int x, int y)
{
	return x>y?x:y;
}

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
	memset(output_buffer, -1, 28);
	// 1. check game state
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
	int index, i = 0;
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
	int index, i = 0;
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


int cc_internal_fill_potential_hline(game* game_ptr, int index, char cell, char output_buffer[28])
{
	char x = cc_get_x_cell(cell);
	int i;
	for (i = x; i<8; i++)
	{
		char new_cell = cell + i - x;
		output_buffer[index] = new_cell;
		index++;
	}
	for (i = x; i>=0; i--)
	{
		char new_cell = cell + i - x;
		output_buffer[index] = new_cell;
		index++;
	}
	return index;
}

int cc_internal_fill_potential_vline(game* game_ptr, int index, char cell, char output_buffer[28])
{
	char y = cc_get_y_cell(cell);
	int i;
	for (i = y; i<8; i++)
	{
		char new_cell = cell + (i - y)*8;
		output_buffer[index] = new_cell;
		index++;
	}
	for (i = y; i>=0; i--)
	{
		char new_cell = cell + (i - y)*8;
		output_buffer[index] = new_cell;
		index++;
	}
	return index;
}


int cc_internal_fill_potential_dline7(game* game_ptr, int index, char cell, char output_buffer[28])
{
	char x = cc_get_x_cell(cell);
	char y = cc_get_y_cell(cell);
	int i;
	for (i = 7-max(x, 7-y); i>=0; i--)
	{
		char new_cell = cell + (i - x)*7;
		output_buffer[index] = new_cell;
		index++;
	}
	for (i = 7-max(7-x, y); i<8; i++)
	{
		char new_cell = cell + (i - x)*7;
		output_buffer[index] = new_cell;
		index++;
	}
	return index;
}


int cc_internal_fill_potential_dline9(game* game_ptr, int index, char cell, char output_buffer[28])
{
	char x = cc_get_x_cell(cell);
	char y = cc_get_y_cell(cell);
	int i;
	for (i = 7-max(x, y); i>=0; i--)
	{
		char new_cell = cell + (i - x)*9;
		output_buffer[index] = new_cell;
		index++;
	}
	for (i = 7-max(7-x, 7-y); i<8; i++)
	{
		char new_cell = cell + (i - x)*9;
		output_buffer[index] = new_cell;
		index++;
	}
	return index;
}

