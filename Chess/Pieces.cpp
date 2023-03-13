#include "Pieces.hpp"

bool isPiece(int x, int y, const Board& board) {
	if (board._grid[x][y] != Cell::EMPTY && board._grid[x][y] != Cell::ENPASSANT) {
		return true;
	}
	return false;
}

bool insideBoard(int x, int y, const Board& board) {
	if (x >= 0 && x < 8 && y >= 0 && y < 8) {
		return true;
	}
	return false;
}

Coords::Coords(int a, int b) {
	x = a;
	y = b;
}

Piece::Piece(Coords coord, Color color, Cell type, Coords& pos) {
	pos = coord;
	_color = color;
	_type = type;
	getTexture(type);
}

void Piece::getTexture(Cell type) {
	std::string location = "assets/bdt60.png";
	if (_color == Color::WHITE)
		location[8] = 'l';
	switch (type) {
	case Cell::PAWN:
		location[7] = 'p';
		break;
	case Cell::KING:
		location[7] = 'k';
		break;
	case Cell::QUEEN:
		location[7] = 'q';
		break;
	case Cell::KNIGHT:
		location[7] = 'n';
		break;
	case Cell::BISHOP:
		location[7] = 'b';
		break;
	case Cell::ROOK:
		location[7] = 'r';
		break;
	}

	_texture.loadFromFile(location, sf::IntRect(0, 0, 60, 60));
	_texture.setSmooth(true);
	_sprite.setTexture(_texture);
	_sprite.setOrigin(sf::Vector2f(30, 30));
}

void Piece::Draw() {
	Window* win = Window::getInstance();

	int x = (WINDOW_WIDTH / BOARD_SIZE) * (_pos.x + 0.5);
	int y = (WINDOW_HEIGHT / BOARD_SIZE) * (_pos.y + 0.5);

	_sprite.setPosition(x, y);
	win->_window->draw(_sprite);
}

void Piece::Draw(int x, int y) {
	Window* win = Window::getInstance();

	_sprite.setPosition(x, y);
	win->_window->draw(_sprite);
}

std::vector<Coords> Pawn::PossibleMoves(const Board& board) {
	std::vector<Coords> moves;
	int direction = (_color == Color::WHITE) ? -1 : 1;

	if (board._grid[_pos.x][_pos.y + direction] == Cell::EMPTY && _pos.y + direction >= 0 && _pos.y + direction < 8) {
		moves.push_back(Coords(_pos.x, _pos.y + direction));
	}
	if (!_hasMoved) {
		if (board._grid[_pos.x][_pos.y + (direction*2)] == Cell::EMPTY && board._grid[_pos.x][_pos.y + (direction*2)] == Cell::EMPTY) {
			moves.push_back(Coords(_pos.x, _pos.y + (direction*2)));
		}
	}
	if (board._grid[_pos.x + 1][_pos.y + direction] != Cell::EMPTY && insideBoard(_pos.x + 1, _pos.y + direction, board)) {
		moves.push_back(Coords(_pos.x + 1, _pos.y + direction));
	}
	if (board._grid[_pos.x - 1][_pos.y + direction] != Cell::EMPTY && insideBoard(_pos.x - 1, _pos.y + direction, board)) {
		moves.push_back(Coords(_pos.x - 1, _pos.y + direction));
	}

	return moves;
}

std::vector<Coords> King::PossibleMoves(const Board& board) {
	std::vector<Coords> moves;

	// Normal Movement
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i != 0 || j != 0) {
				if (insideBoard(_pos.x + i, _pos.y + j, board)) {
					moves.push_back(Coords(_pos.x + i, _pos.y + j));
				}
			}
		}
	}
	
	if (!_hasMoved) {
		if (board._grid[5][_pos.y] == Cell::EMPTY && board._grid[6][_pos.y] == Cell::EMPTY && board._grid[7][_pos.y] == Cell::ROOK) {
			moves.push_back(Coords(6, _pos.y));
		}
		if (board._grid[3][_pos.y] == Cell::EMPTY && board._grid[2][_pos.y] == Cell::EMPTY && board._grid[1][_pos.y] == Cell::EMPTY && board._grid[0][_pos.y] == Cell::ROOK) {
			moves.push_back(Coords(2, _pos.y));
		}
	}

	return moves;
}
std::vector<Coords> Queen::PossibleMoves(const Board& board) {
	std::vector<Coords> moves;

	// Orthogonal Movement
	for (int i = -1; i >= -_pos.x; i--) {
			if (insideBoard(_pos.x + i, _pos.y, board)) {
				moves.push_back(Coords(_pos.x + i, _pos.y));	
				if (isPiece(_pos.x + i, _pos.y, board)) break;
			}
			else {
				break;
			}
	}

	for (int i = 1; i < (8-_pos.x); i++) {
			if (insideBoard(_pos.x + i, _pos.y, board)) {
				moves.push_back(Coords(_pos.x + i, _pos.y));
				if (isPiece(_pos.x + i, _pos.y, board)) break;
			}
			else {
				break;
			}
	}
	
	for (int i = -1; i >= -_pos.y; i--) {
			if (insideBoard(_pos.x, _pos.y + i, board)) {
				moves.push_back(Coords(_pos.x, _pos.y + i));
				if (isPiece(_pos.x, _pos.y + i, board)) break;
			}
			else {
				break;
			}
		
	}

	for (int i = 1; i < (8 - _pos.y); i++) {
			if (insideBoard(_pos.x, _pos.y + i, board)) {
				moves.push_back(Coords(_pos.x, _pos.y + i));
				if (isPiece(_pos.x, _pos.y + i, board)) break;
			}
			else {
				break;
			}
	}

	// Diagonal Movement
	for (int i = -1; i >= -((_pos.x < _pos.y) ? _pos.x : _pos.y); i--) {
		if (insideBoard(_pos.x + i, _pos.y + i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y + i));
			if (isPiece(_pos.x + i, _pos.y + i, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = -1; i >= -((_pos.x < 8-_pos.y) ? _pos.x : 8-_pos.y); i--) {
		if (insideBoard(_pos.x + i, _pos.y - i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y - i));
			if (isPiece(_pos.x + i, _pos.y - i, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = +1; i <= ((8-_pos.x < 8-_pos.y) ? 8-_pos.x : 8-_pos.y); i++) {
		if (insideBoard(_pos.x + i, _pos.y + i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y + i));
			if (isPiece(_pos.x + i, _pos.y + i, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = +1; i <= ((8 - _pos.x < _pos.y) ? 8-_pos.x : _pos.y); i++) {
		if (insideBoard(_pos.x + i, _pos.y - i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y - i));
			if (isPiece(_pos.x + i, _pos.y - i, board)) break;
		}
		else {
			break;
		}
	}

	return moves;
}

std::vector<Coords> Rook::PossibleMoves(const Board& board) {
	std::vector<Coords> moves;

	for (int i = -1; i >= -_pos.x; i--) {
		if (insideBoard(_pos.x + i, _pos.y, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y));
			if (isPiece(_pos.x + i, _pos.y, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = 1; i < (8 - _pos.x); i++) {
		if (insideBoard(_pos.x + i, _pos.y, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y));
			if (isPiece(_pos.x + i, _pos.y, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = -1; i >= -_pos.y; i--) {
		if (insideBoard(_pos.x, _pos.y + i, board)) {
			moves.push_back(Coords(_pos.x, _pos.y + i));
			if (isPiece(_pos.x, _pos.y + i, board)) break;
		}
		else {
			break;
		}

	}

	for (int i = 1; i < (8 - _pos.y); i++) {
		if (insideBoard(_pos.x, _pos.y + i, board)) {
			moves.push_back(Coords(_pos.x, _pos.y + i));
			if (isPiece(_pos.x, _pos.y + i, board)) break;
		}
		else {
			break;
		}
	}

	return moves;
}

std::vector<Coords> Knight::PossibleMoves(const Board& board) {
	std::vector<Coords> moves;

	for (int i = -2; i < 3; i++) {
		for (int j = -2; j < 3; j++) {
			if (abs(i) + abs(j) == 3) {
				if (insideBoard(_pos.x + i, _pos.y + j, board)) {
					moves.push_back(Coords(_pos.x + i, _pos.y + j));
				}
			}
		}
	}

	return moves;
}

std::vector<Coords> Bishop::PossibleMoves(const Board& board) {
	std::vector<Coords> moves;

	for (int i = -1; i >= -((_pos.x < _pos.y) ? _pos.x : _pos.y); i--) {
		if (insideBoard(_pos.x + i, _pos.y + i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y + i));
			if (isPiece(_pos.x + i, _pos.y + i, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = -1; i >= -((_pos.x < 8 - _pos.y) ? _pos.x : 8 - _pos.y); i--) {
		if (insideBoard(_pos.x + i, _pos.y - i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y - i));
			if (isPiece(_pos.x + i, _pos.y - i, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = +1; i <= ((8 - _pos.x < 8 - _pos.y) ? 8 - _pos.x : 8 - _pos.y); i++) {
		if (insideBoard(_pos.x + i, _pos.y + i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y + i));
			if (isPiece(_pos.x + i, _pos.y + i, board)) break;
		}
		else {
			break;
		}
	}

	for (int i = +1; i <= ((8 - _pos.x < _pos.y) ? 8 - _pos.x : _pos.y); i++) {
		if (insideBoard(_pos.x + i, _pos.y - i, board)) {
			moves.push_back(Coords(_pos.x + i, _pos.y - i));
			if (isPiece(_pos.x + i, _pos.y - i, board)) break;
		}
		else {
			break;
		}
	}

	return moves;
}

std::vector<Coords> Piece::PossibleMoves(const Board&) {
	std::vector<Coords> moves;
	return moves;
}

void Piece::Move(Coords coords, Board& board) {
	if (_type == Cell::PAWN) {
		if (!_hasMoved) {
			if (abs(coords.y - _pos.y) == 2) {
				board._grid[_pos.x][(_pos.y + coords.y) / 2] = Cell::ENPASSANT;
			}
		}
	}
	_hasMoved = true;
	board._grid[_pos.x][_pos.y] = Cell::EMPTY;
	_pos.x = coords.x;
	_pos.y = coords.y;
	board._grid[_pos.x][_pos.y] = _type;
}