/* Copyright (C) 2020 Yuri Moskov
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#include <chesscore.h>
#include <iostream>
#include <cstdlib>

#define OUTPUT_BOOLEAN(X) (X?"True":"False")

std::string getPiece(char x)
{
	std::string base_str;
	if (x > 0)
	{
		base_str = "white ";
	}
	else
	{
		base_str = "black ";
	}

	x = abs(x);
	if ( x == CELL_WHITE_KING ) return base_str + " king";
	if ( x == CELL_WHITE_QUEEN ) return base_str + " queen";
	if ( x == CELL_WHITE_ROOK ) return base_str + " rook";
	if ( x == CELL_WHITE_BISHOP ) return base_str + " bishop";
	if ( x == CELL_WHITE_KNIGHT ) return base_str + " knight";
	if ( x == CELL_WHITE_PAWN ) return base_str + " pawn";
	return "none";
}

std::ostream& operator<<(std::ostream& os, game g)
{
	std::ostream::sentry s(os);
	if (s)
	{
		os << "STATE_BIT_IS_WHITE_TURN: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_IS_WHITE_TURN) << std::endl;
		os << "STATE_BIT_STALEMATE: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_STALEMATE) << std::endl;
		os << "STATE_BIT_WHITE_VICTORY: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_WHITE_VICTORY) << std::endl;
		os << "STATE_BIT_A1_ROOK_MOVED: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_A1_ROOK_MOVED) << std::endl;
		os << "STATE_BIT_H1_ROOK_MOVED: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_H1_ROOK_MOVED) << std::endl;
		os << "STATE_BIT_A8_ROOK_MOVED: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_A8_ROOK_MOVED) << std::endl;
		os << "STATE_BIT_H8_ROOK_MOVED: " << OUTPUT_BOOLEAN(g.state & STATE_BIT_H8_ROOK_MOVED) << std::endl;
		os << "Last cell: " << g.last_cell << std::endl;
		os << "Cells: ";
		for (int y = 7; y >= 0; y--)
		{
			for (int x = 0; x < 8; x++)
			{
				os << getPiece(g.cells[x+y*8]) << " (" << x << ", " << y << "); ";
			}
			os << std::endl;
		}
	}
	return os;
}

int main(int argc, char* argv[])
{
	game chess_game;
	std::string cmd;

	std::cout << "Enter command: ";
	std::cin >> cmd;
	while (cmd != "quit")
	{
		if (cmd == "dump")
		{
			std::cout << chess_game;
		}
		if (cmd == "init")
		{
			cc_init(&chess_game);
		}
		if (cmd == "get")
		{
			char buffer[28];
			int x, y;
			std::cin >> x >> y;

			cc_get_potential_turns(&chess_game, x+y*8, buffer);

			for (int i = 0; i < 28; i++)
			{
				std::cout << int(buffer[i]) << ":";
			}
			std::cout << std::endl;
		}

		std::cout << "Enter command: ";
		std::cin >> cmd;
	}
	return 0;
}

