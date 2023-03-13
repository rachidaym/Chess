#include "Board.hpp"



Board::Board() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			_grid[i][j] = Cell::EMPTY;
			_highlighted[i][j] = false;
		}
	}
}

void Board::Draw() {
	Window* win = Window::getInstance();
	for (int i = 0;i < BOARD_SIZE;i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) & 1) {
				_color = DARK_COLOR;
			}
			else {
				_color = LIGHT_COLOR;
			}

			_rectangle.setSize(sf::Vector2f(WINDOW_WIDTH/BOARD_SIZE, WINDOW_HEIGHT/BOARD_SIZE));
			_rectangle.setFillColor(_color);
			_rectangle.setPosition(sf::Vector2f(i * WINDOW_WIDTH / BOARD_SIZE, j * WINDOW_WIDTH / BOARD_SIZE));
			win->_window->draw(_rectangle);

			if (_highlighted[i][j]) {
				_rectangle.setFillColor(HIGHLIGHTED_COLOR);
				win->_window->draw(_rectangle);
			}
		}
	}
}

void Board::UnHighlight() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_highlighted[i][j] = false;
		}
	}
}

void Board::UnHighlightCell(int x, int y) {
	_highlighted[x][y] = false;
}