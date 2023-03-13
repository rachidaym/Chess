#pragma once

#include "Window.hpp"
#include "Board.hpp"
#include "Pieces.hpp"


class Game {
private:
	std::vector <Piece*> _pieces;
	bool _isChecked;
	bool _checkMate = false;
	Color _currentSide = Color::WHITE;
public:
	void Run();
	void BoardSetup();
	Piece* FindPiece(int, int, std::vector<Piece*>&);
	int FindPieceIndex(int, int, std::vector<Piece*>&);
	void HighlightPossibleMoves(Piece*, Board&);
	void UpdateBoard(Board&);
	bool isMovePossible(Piece*, Board&, Coords, bool);
	bool CheckForCheck(Board&, std::vector<Piece*>&);
	bool LeavesCheck(Piece*, Board, Coords, bool, int, int);
	bool CheckMate(Board&);
	void CheckForPromotion(Piece*);
	void Promote(Piece*);
	void PromoteTo(Piece*, Cell);
};