#pragma once

#include "Window.hpp"

#define BOARD_SIZE 8
#define LIGHT_COLOR sf::Color::Color(238, 225, 215, 255)
#define DARK_COLOR sf::Color::Color(188, 133, 92, 255)
#define HIGHLIGHTED_COLOR sf::Color::Color(255, 255, 0, 128)

enum class Cell{EMPTY, PAWN, KING, QUEEN, KNIGHT, BISHOP, ROOK, ENPASSANT};

class Board {
private:
	sf::RectangleShape _rectangle;
	sf::Color _color;
public:
	bool _highlighted[8][8];
	Cell _grid[BOARD_SIZE][BOARD_SIZE];

	Board();
	void Draw();
	void UnHighlight();
	void UnHighlightCell(int, int);
};