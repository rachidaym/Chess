#include "Game.hpp"

// Main loop of the game

void Game::Run() {
	Window* win = Window::getInstance();
	Board board;
	Piece* selectedPiece = nullptr;

	BoardSetup();

	while (win->_window->isOpen()) {

		_isChecked = CheckForCheck(board, _pieces);
		_checkMate = CheckMate(board);

		while (win->_window->pollEvent(win->_event)) {
			if (win->_event.type == sf::Event::Closed) {
				win->_window->close();
			}
			if (!_checkMate) {
				if (win->_event.type == sf::Event::MouseButtonPressed) {
					board.UnHighlight();
					sf::Vector2i pos = sf::Mouse::getPosition(*(win->_window));
					int x = pos.x / (WINDOW_WIDTH / BOARD_SIZE);
					int y = pos.y / (WINDOW_HEIGHT / BOARD_SIZE);
					if (selectedPiece && board._grid[x][y] != Cell::KING) {
						if (isMovePossible(selectedPiece, board, Coords(x, y), false)) {
							for (int i = 0; i < 8; i++) {
								for (int j = 0; j < 8; j++) {
									if (board._grid[i][j] == Cell::ENPASSANT)
										board._grid[i][j] = Cell::EMPTY;
								}
							}
							selectedPiece->Move(Coords(x, y), board);
							if (selectedPiece->_type == Cell::PAWN) {
								CheckForPromotion(selectedPiece);
							}
							_currentSide = (_currentSide == Color::WHITE) ? Color::BLACK : Color::WHITE;
						}
					}
					if (selectedPiece = FindPiece(x, y, _pieces)) {
						if (selectedPiece->_color == _currentSide) {
							HighlightPossibleMoves(selectedPiece, board);
						}
						else {
							selectedPiece = nullptr;
						}
					}
				}
			}
	}

		win->_window->clear();
		if (!_checkMate) {
			UpdateBoard(board);
			board.Draw();
			for (auto x : _pieces) {
				x->Draw();
			}
		}
		win->_window->display();
	}
}

//Fill the board with pieces at starting positions

void Game::BoardSetup() {

	// Pawns
	for (int i = 0; i < 8; i++) {
		_pieces.push_back(new Pawn(Coords(i, 6), Color::WHITE));
		_pieces.push_back(new Pawn(Coords(i, 1), Color::BLACK));
	}

	// Rooks
	_pieces.push_back(new Rook(Coords(0, 0), Color::BLACK));
	_pieces.push_back(new Rook(Coords(7, 0), Color::BLACK));
	_pieces.push_back(new Rook(Coords(0, 7), Color::WHITE));
	_pieces.push_back(new Rook(Coords(7, 7), Color::WHITE));

	// Knights
	_pieces.push_back(new Knight(Coords(1, 0), Color::BLACK));
	_pieces.push_back(new Knight(Coords(6, 0), Color::BLACK));
	_pieces.push_back(new Knight(Coords(1, 7), Color::WHITE));
	_pieces.push_back(new Knight(Coords(6, 7), Color::WHITE));


	// Bishops
	_pieces.push_back(new Bishop(Coords(2, 0), Color::BLACK));
	_pieces.push_back(new Bishop(Coords(5, 0), Color::BLACK));
	_pieces.push_back(new Bishop(Coords(2, 7), Color::WHITE));
	_pieces.push_back(new Bishop(Coords(5, 7), Color::WHITE));

	// Queens
	_pieces.push_back(new Queen(Coords(3, 0), Color::BLACK));
	_pieces.push_back(new Queen(Coords(3, 7), Color::WHITE));

	// Kings
	_pieces.push_back(new King(Coords(4, 0), Color::BLACK));
	_pieces.push_back(new King(Coords(4, 7), Color::WHITE));
}

// Finds a piece in specific coordinates and returns a pointer to it

Piece* Game::FindPiece(int x, int y, std::vector<Piece*>& pieces) {
	for (auto p : pieces) {
		if (p->_pos.x == x && p->_pos.y == y)
			return p;
	}
	return nullptr;
}

// Finds a piece in specific coordinates and returns its index

int Game::FindPieceIndex(int x, int y, std::vector<Piece*>& pieces) {
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces[i]->_pos.x == x && pieces[i]->_pos.y == y)
			return i;
	}
	return -1;
}

// Highlights the selected piece's possible moves in the board

void Game::HighlightPossibleMoves(Piece* piece,Board& board) {
	board.UnHighlight();
	std::vector<Coords> moves = piece->PossibleMoves(board);
	for (auto m : moves) {
		Piece* p = FindPiece(m.x, m.y, _pieces);
		if (p) {
			if (p->_color == piece->_color || p->_type == Cell::KING) {
				board._highlighted[m.x][m.y] = false;
				continue;
			}
		}
		board._highlighted[m.x][m.y] = true;
	}
}

// Updates the board's grid with pieces every frame

void Game::UpdateBoard(Board& board) {
	for (auto p : _pieces) {
		board._grid[p->_pos.x][p->_pos.y] = p->_type;
	}
}

// Checks if a move is possible

bool Game::isMovePossible(Piece* piece, Board& board, Coords move, bool test) {
	std::vector<Coords> moves = piece->PossibleMoves(board);

	int dir = 0;
	bool possible = false;
	bool enpassant = false;
	int castle = 0;

	Piece* rook = nullptr;

	for (auto m : moves) {
		if (m.x == move.x && m.y == move.y) {
			if (board._grid[move.x][move.y] != Cell::EMPTY && board._grid[move.x][move.y] != Cell::KING) {
				if (board._grid[move.x][move.y] == Cell::ENPASSANT) {
					if (move.y = 2) {
						dir = 1;
					}
					else {
						dir = -1;
					}

					enpassant = true;
					possible = true;
					break;
				}

				if (FindPiece(move.x, move.y, _pieces)->_color != piece->_color) {
					possible = true;
					break;
				}
				else {
					break;
				}
			}
			if (piece->_type == Cell::KING && abs(move.x - piece->_pos.x) > 1) {
				if (move.x - piece->_pos.x > 0) {
					rook = FindPiece(7, piece->_pos.y, _pieces);
					if (rook) {
						if (rook->_hasMoved == false) {
							possible = true;
							castle = 1;
							break;
						}
					}
				}
				else {
					rook = FindPiece(0, piece->_pos.y, _pieces);
					if (rook) {
						if (rook->_hasMoved == false) {
							possible = true;
							castle = -1;
							break;
						}
					}
				}
			}

			if (board._grid[move.x][move.y] == Cell::KING) {
				possible = false;
			}
			else {
				possible = true;
			}
			break;
		}
	}

	if (possible) {
		if (LeavesCheck(piece, board, move, enpassant, dir, castle)) {
			if (board._grid[move.x][move.y] != Cell::EMPTY && board._grid[move.x][move.y] != Cell::KING && !test) {
				if (enpassant) {
					_pieces.erase(_pieces.cbegin() + FindPieceIndex(move.x, move.y + dir, _pieces));
					board._grid[move.x][move.y + dir] = Cell::EMPTY;
				}
				else {
					_pieces.erase(_pieces.cbegin() + FindPieceIndex(move.x, move.y, _pieces));
				}
			}
			if (castle == 1 && rook) {
				rook->Move(Coords(5, piece->_pos.y), board);
				piece->Move(Coords(6, piece->_pos.y), board);
				_currentSide = (_currentSide == Color::WHITE) ? Color::BLACK : Color::WHITE;
				return false;
			}
			else if (castle == -1 && rook) {
				rook->Move(Coords(3, piece->_pos.y), board);
				piece->Move(Coords(2, piece->_pos.y), board);
				_currentSide = (_currentSide == Color::WHITE) ? Color::BLACK : Color::WHITE;
				return false;
			}
			return true;
		}
	}

	return false;
	
}

// Checks if the king is checked

bool Game::CheckForCheck(Board& board, std::vector<Piece*>& pieces) {
	Piece* piece;

	for (auto p : pieces) {
		if (p->_color != _currentSide) {
			std::vector<Coords> moves = p->PossibleMoves(board);
			for (auto m : moves) {
				if (board._grid[m.x][m.y] != Cell::EMPTY && board._grid[m.x][m.y] != Cell::ENPASSANT) {
					piece = FindPiece(m.x, m.y, pieces);
					if (piece->_color == _currentSide && piece->_type == Cell::KING) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

// Checks if a move leaves check or leads to check

bool Game::LeavesCheck(Piece* piece, Board board, Coords move, bool enpassant, int enpassantdir, int castle) {
	std::vector<Piece*> pieces;
	Board newBoard = board;

	bool leavesCheck;

	Piece* rook;
	Piece* newPiece;

	// Copy vector

	for (auto p : _pieces) {
		switch (p->_type) {
		case Cell::PAWN:
			newPiece = new Pawn(p->_pos, p->_color);
			pieces.push_back(newPiece);
			break;
		case Cell::KING:
			newPiece = new King(p->_pos, p->_color);
			pieces.push_back(newPiece);
			break;
		case Cell::QUEEN:
			newPiece = new Queen(p->_pos, p->_color);
			pieces.push_back(newPiece);
			break;
		case Cell::ROOK:
			newPiece = new Rook(p->_pos, p->_color);
			pieces.push_back(newPiece);
			break;
		case Cell::BISHOP:
			newPiece = new Bishop(p->_pos, p->_color);
			pieces.push_back(newPiece);
			break;
		case Cell::KNIGHT:
			newPiece = new Knight(p->_pos, p->_color);
			pieces.push_back(newPiece);
			break;
		}
	}

	if (newBoard._grid[move.x][move.y] != Cell::EMPTY && newBoard._grid[move.x][move.y] != Cell::KING) {
		if (enpassant) {
			pieces.erase(pieces.cbegin() + FindPieceIndex(move.x, move.y + enpassantdir, pieces));
			newBoard._grid[move.x][move.y + enpassantdir] = Cell::EMPTY;
		}
		else {
			pieces.erase(pieces.cbegin() + FindPieceIndex(move.x, move.y, pieces));
		}
	}
	newPiece = FindPiece(piece->_pos.x, piece->_pos.y, pieces);

	if (castle == 1) {
		rook = FindPiece(7, newPiece->_pos.y, pieces);
		rook->Move(Coords(5, newPiece->_pos.y), newBoard);
		newPiece->Move(Coords(6, newPiece->_pos.y), newBoard);
	}
	else if (castle == -1) {
		rook = FindPiece(0, newPiece->_pos.y, pieces);
		rook->Move(Coords(3, newPiece->_pos.y), newBoard);
		newPiece->Move(Coords(2, newPiece->_pos.y), newBoard);
	}
	else {
		newPiece->Move(move, newBoard);
	}
	leavesCheck = !CheckForCheck(newBoard, pieces);
	for (auto p : pieces) {
		delete p;
	}
	return leavesCheck;
}

// Checks if there is a checkmate

bool Game::CheckMate(Board& board) {
	std::vector<Coords> moves;

	if (_isChecked) {
		for (auto p : _pieces) {
			if (p->_color == _currentSide) {
				moves = p->PossibleMoves(board);
				for (auto m : moves) {
					if (isMovePossible(p, board, m, true)) {
						return false;
					}
				}
			}
		}
		return true;
	}
	return false;
}

// Checks if a pawn can promote

void Game::CheckForPromotion(Piece* piece) {
	if(piece->_color == Color::WHITE){
		if (piece->_pos.y == 0) {
			Promote(piece);
		}
	}
	else {
		if (piece->_pos.y == 7) {
			Promote(piece);
		}
	}
}

// Draws the UI for pawn promotion

void Game::Promote(Piece* piece) {
	Window* win = Window::getInstance();

	Piece* queen = new Queen(Coords(0, 0), piece->_color);
	Piece* rook = new Rook(Coords(0, 0), piece->_color);
	Piece* knight = new Knight(Coords(0, 0), piece->_color);
	Piece* bishop = new Bishop(Coords(0, 0), piece->_color);

	sf::RectangleShape rect(sf::Vector2f(WINDOW_WIDTH/4, 2 * WINDOW_HEIGHT / BOARD_SIZE));
	rect.setFillColor(sf::Color::Blue);
	float verticalpos = (WINDOW_HEIGHT / 2) - (2 * WINDOW_HEIGHT / BOARD_SIZE) / 2;

	int pieceOffsetX = WINDOW_WIDTH/8;

	for (int i = 0; i < 4; i++) {
		rect.setPosition(sf::Vector2f(i*WINDOW_WIDTH/4, verticalpos));
		win->_window->draw(rect);
		switch (i) {
		case 0:
			queen->Draw(i * WINDOW_WIDTH / 4 + pieceOffsetX, WINDOW_HEIGHT / 2);
			break;
		case 1:
			knight->Draw(i * WINDOW_WIDTH / 4 + pieceOffsetX, WINDOW_HEIGHT / 2);
			break;
		case 2:
			rook->Draw(i * WINDOW_WIDTH / 4 + pieceOffsetX, WINDOW_HEIGHT / 2);
			break;
		case 3:
			bishop->Draw(i * WINDOW_WIDTH / 4 + pieceOffsetX, WINDOW_HEIGHT / 2);
			break;
		}
	}

	win->_window->display();

	while (true) {
		while (win->_window->pollEvent(win->_event)) {
			if (win->_event.type == sf::Event::Closed) {
				return;
			}
			if (win->_event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2i pos = sf::Mouse::getPosition(*(win->_window));
				if (pos.y >= verticalpos && pos.y <= (verticalpos + 2 * WINDOW_HEIGHT / BOARD_SIZE)) {
					int x = pos.x / (WINDOW_WIDTH / 4);
					switch (x) {
					case 0:
						PromoteTo(piece, Cell::QUEEN);
						break;
					case 1:
						PromoteTo(piece, Cell::KNIGHT);
						break;
					case 2:
						PromoteTo(piece, Cell::ROOK);
						break;
					case 3:
						PromoteTo(piece, Cell::BISHOP);
						break;
					}
					return;
				}
			}
		}
	}
}

// Promotes the pawn to the selected piece

void Game::PromoteTo(Piece* piece, Cell type) {
	int x = piece->_pos.x;
	int y = piece->_pos.y;
	Color color = piece->_color;

	_pieces.erase(_pieces.cbegin() + FindPieceIndex(piece->_pos.x, piece->_pos.y, _pieces));
	delete piece;

	switch (type) {
	case Cell::QUEEN:
		_pieces.push_back(new Queen(Coords(x, y), color));
		break;
	case Cell::KNIGHT:
		_pieces.push_back(new Knight(Coords(x, y), color));
		break;
	case Cell::ROOK:
		_pieces.push_back(new Rook(Coords(x, y), color));
		break;
	case Cell::BISHOP:
		_pieces.push_back(new Bishop(Coords(x, y), color));
		break;
	}
}