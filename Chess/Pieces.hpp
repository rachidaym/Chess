#pragma once

#include "Board.hpp"

enum class Color{WHITE, BLACK};

struct Coords{
	int x;
	int y;
	Coords(int, int);
	Coords(){}
};

bool isPiece(int, int, const Board&);
bool insideBoard(int, int, const Board&);


class Piece {
protected:
	bool _isAlive = 1;
	sf::Sprite _sprite;
	sf::Texture _texture;

public:
	bool _hasMoved = false;

	Color _color;
	Coords _pos;
	Cell _type;

	explicit Piece(Coords, Color, Cell, Coords&);
	virtual std::vector<Coords> PossibleMoves(const Board&);
	void Draw();
	void Draw(int x, int y);
	void getTexture(Cell type);
	void Move(Coords, Board&);
};

class Pawn : public Piece {
public:
	Cell _type = Cell::PAWN;

	Pawn(Coords coord, Color color) : Piece(coord, color, Cell::PAWN, this->_pos) {}
	void Promote();
	std::vector<Coords> PossibleMoves(const Board&);
};

class King : public Piece {
public:
	Cell _type = Cell::KING;

	King(Coords coord, Color color) : Piece(coord, color, Cell::KING, this->_pos) {}
	std::vector<Coords> PossibleMoves(const Board&);
	void Castle(int direction);
};

class Queen : public Piece {
public:
	Cell _type = Cell::QUEEN;

	Queen(Coords coord, Color color) : Piece(coord, color, Cell::QUEEN, this->_pos) {}
	std::vector<Coords> PossibleMoves(const Board&);
};

class Rook : public Piece {
public:
	Cell _type = Cell::ROOK;

	Rook(Coords coord, Color color) : Piece(coord, color, Cell::ROOK, this->_pos) {}
	std::vector<Coords> PossibleMoves(const Board&);
};

class Knight : public Piece {
public:
	Cell _type = Cell::KNIGHT;

	Knight(Coords coord, Color color) : Piece(coord, color, Cell::KNIGHT, this->_pos) {}
	std::vector<Coords> PossibleMoves(const Board&);
};

class Bishop : public Piece {
public:
	Cell _type = Cell::BISHOP;

	Bishop(Coords coord, Color color) : Piece(coord, color, Cell::BISHOP, this->_pos) {}
	std::vector<Coords> PossibleMoves(const Board&);
};