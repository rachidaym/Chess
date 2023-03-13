#include "Window.hpp"

Window* Window::window_ = nullptr;

Window::Window(int width, int height, std::string title) {
	_window = new sf::RenderWindow(sf::VideoMode(width, height), title);
}

Window* Window::getInstance() {
	if (window_ == nullptr) {
		window_ = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	}
	return window_;
}