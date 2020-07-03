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

char cc_get_opposite_color(char piece)
{
	if (piece > 0) return CHESSCORE_BLACK;
	if (piece < 0) return CHESSCORE_WHITE;
	return 0;
}

int cc_is_cell_under_attack(game* game_ptr, char color, char cell)
{
	int i;
	for (i = 0; i < 64; i++)
	{
		if (cc_is_piece_same_color(color, game_ptr->cells[i]))
		{
			char output_buffer[26];
			cc_get_potential_turns(game_ptr, i, output_buffer);

			int x;
			for (x = 0; x < 28; x++)
			{
				if (output_buffer[x] == cell)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int cc_get_potential_turns(game* game_ptr, char cell, char output_buffer[28])
{
	TurnContext context;
	context.game_ptr = game_ptr;
	context.index = 0;
	context.cell = cell;
	context.output_buffer = output_buffer;

	return cc_internal_get_potential_turns(&context);
}

int cc_internal_get_potential_turns(TurnContext* context)
{
	int current_piece = cc_get_piece(context->game_ptr->cells[context->cell]);

	memset(context->output_buffer, -1, 28);

	switch (current_piece) {
		case PIECE_KING:
		{
			context->index = cc_internal_get_potential_king_turns(context);
			break;
		}
		case PIECE_QUEEN:
		{
			context->index = cc_internal_fill_potential_hline(context);
			context->index = cc_internal_fill_potential_vline(context);
			context->index = cc_internal_fill_potential_dline7(context);
			context->index = cc_internal_fill_potential_dline9(context);
			break;
		}
		case PIECE_ROOK:
		{
			context->index = cc_internal_fill_potential_hline(context);
			context->index = cc_internal_fill_potential_vline(context);
			break;
		}
		case PIECE_BISHOP:
		{
			context->index = cc_internal_fill_potential_dline7(context);
			context->index = cc_internal_fill_potential_dline9(context);
			break;
		}
		case PIECE_KNIGHT:
		{
			context->index = cc_internal_get_potential_knight_turns(context);
			break;
		}
		case PIECE_PAWN:
		{
			context->index = cc_internal_get_potential_pawn_turns(context);
			break;
		}
	}
	return context->index;
}

int cc_internal_get_potential_pawn_turns(TurnContext* context)
{
	int direction = (context->game_ptr->cells[context->cell] > 0) ? 8 : -8;
	char new_cell = cc_get_cell_id_by_id(context->cell + direction);

	// 
	if (context->game_ptr->cells[new_cell] == CELL_NONE)
	{
		context->output_buffer[context->index] = new_cell;
		context->index++;

		new_cell = cc_get_cell_id_by_id(context->cell + direction * 2);
		if (new_cell != -1)
		{
			if ( (direction == 8  && cc_get_y_cell(context->cell) == 1) ||
				 (direction == -8 && cc_get_y_cell(context->cell) == 7) )
			{
				if (context->game_ptr->cells[new_cell] == CELL_NONE)
				{
					context->output_buffer[context->index] = new_cell;
					context->index++;
				}
			}
		}
	}
	
	new_cell = cc_get_cell_id_by_id(context->cell + direction + 1);
	if (new_cell != -1)
	{
		char piece = context->game_ptr->cells[new_cell];
		if (piece != CELL_NONE && !cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
		{
			context->output_buffer[context->index] = new_cell;
			context->index++;
		}

		// En passant
		if ( (direction == 8  && cc_get_y_cell(context->cell) == 4) ||
			 (direction == -8 && cc_get_y_cell(context->cell) == 3) )
		{
			if (context->game_ptr->last_cell == new_cell - direction)
			{
				piece = context->game_ptr->cells[context->game_ptr->last_cell];
				if (cc_get_piece(piece) == PIECE_PAWN)
				{
					if (!cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
					{
						context->output_buffer[context->index] = new_cell;
						context->index++;
					}
				}
			}
		}
	}

	new_cell = cc_get_cell_id_by_id(context->cell + direction - 1);
	if (new_cell != -1)
	{
		char piece = context->game_ptr->cells[new_cell];
		if (piece != CELL_NONE && !cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
		{
			context->output_buffer[context->index] = new_cell;
			context->index++;
		}

		// En passant
		if ( (direction == 8  && cc_get_y_cell(context->cell) == 4) ||
			 (direction == -8 && cc_get_y_cell(context->cell) == 3) )
		{
			if (context->game_ptr->last_cell == new_cell - direction)
			{
				piece = context->game_ptr->cells[context->game_ptr->last_cell];
				if (cc_get_piece(piece) == PIECE_PAWN)
				{
					if (!cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
					{
						context->output_buffer[context->index] = new_cell;
						context->index++;
					}
				}
			}
		}
	}

	return context->index;
}

int cc_internal_get_potential_king_turns(TurnContext* context)
{
	int moves[] = {-1, 7, 8, 9, 1, -7, -8, -9};
	int i = 0;
	for (; i < 8; i++)
	{
		char new_cell = cc_get_cell_id_by_id(context->cell + moves[i]);
		if (new_cell != -1)
		{
			char piece = context->game_ptr->cells[new_cell];
			char k_piece = context->game_ptr->cells[context->cell];
			if (!cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
			{
				if (!cc_is_cell_under_attack(context->game_ptr, cc_get_opposite_color(k_piece), new_cell))
				{
					context->output_buffer[context->index] = new_cell;
					context->index++;
				}
			}
		}
	}
	return context->index;
}

int cc_internal_get_potential_knight_turns(TurnContext* context)
{
	int moves[] = {6, 15, 17, 10, -6, -15, -17, -10};
	int i = 0;
	for (; i < 8; i++)
	{
		char new_cell = cc_get_cell_id_by_id(context->cell + moves[i]);
		if (new_cell != -1)
		{
			char piece = context->game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
			{
				context->output_buffer[context->index] = new_cell;
				context->index++;
			}
		}
	}
	return context->index;
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

int cc_internal_fill_line(TurnContext* context, int bitset, char offset)
{
	char x = cc_get_x_cell(context->cell);
	char y = cc_get_y_cell(context->cell);
	int i;

	i = cc_internal_get_index(bitset, x, y) + 1;
	for (; i < 8; i++)
	{
		char new_cell = context->cell + (i - x) * offset;
		if (new_cell >=0 && new_cell < 64)
		{
			char piece = context->game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
			{
				context->output_buffer[context->index] = new_cell;
				context->index++;
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
		char new_cell = context->cell + (i - x) * offset;
		if (new_cell >=0 && new_cell < 64)
		{
			char piece = context->game_ptr->cells[new_cell];
			if (!cc_is_piece_same_color(piece, context->game_ptr->cells[context->cell]))
			{
				context->output_buffer[context->index] = new_cell;
				context->index++;
			}
			if (piece != CELL_NONE)
			{
				break;
			}
		}
	}
	return context->index;
}

int cc_internal_fill_potential_hline(TurnContext* context)
{
	return cc_internal_fill_line(context, MASK_USE_X, 1);
}

int cc_internal_fill_potential_vline(TurnContext* context)
{
	return cc_internal_fill_line(context, MASK_USE_Y, 1);
}

int cc_internal_fill_potential_dline7(TurnContext* context)
{
	return cc_internal_fill_line(context, MASK_USE_X | MASK_USE_Y, 7);
}

int cc_internal_fill_potential_dline9(TurnContext* context)
{
	return cc_internal_fill_line(context, MASK_USE_X | MASK_USE_Y, 9);
}

