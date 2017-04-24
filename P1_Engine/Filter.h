#pragma once
#include "SFML\Graphics.hpp"
#include <memory>
#include "P1_Engine.h"

class Filter
{
public:
	Filter();
	~Filter();

	void setColor(sf::Color col);
	void setColor(int r, int g, int b, int a = 0);
	void setAlpha(int alpha);
	void draw();
	void setActive(bool arg);
	bool getActive();

private:
	std::shared_ptr<sf::RectangleShape> g_filter;
	bool isActive;
};

