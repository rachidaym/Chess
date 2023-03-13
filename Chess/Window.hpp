#pragma once

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Chess"

class Window {
private:
	static Window* window_;
	Window(int width, int height, std::string title);

public:
	static Window* getInstance();
	sf::RenderWindow* _window;
	sf::Event _event;
};