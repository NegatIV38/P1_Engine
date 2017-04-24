#include "Filter.h"



Filter::Filter()
{
	g_filter = std::make_shared<sf::RectangleShape>();
	g_filter->setSize(sf::Vector2f(P1_Engine::getNativeResolutionX(), P1_Engine::getNativeResolutionY()));
	g_filter->setFillColor(sf::Color::Transparent);
	isActive = false;
}


Filter::~Filter()
{
}

void Filter::setColor(sf::Color col)
{
	g_filter->setFillColor(col);
}

void Filter::setColor(int r, int g, int b, int a)
{
	setColor(sf::Color(r, g, b, a));
}

void Filter::setAlpha(int alpha)
{
	sf::Color col = g_filter->getFillColor();
	g_filter->setFillColor(sf::Color(col.r, col.g, col.b, alpha));
}

void Filter::draw()
{
	if (isActive) {
		g_filter->setPosition(g_filter->getPosition() + sf::Vector2f(P1_Engine::getLastDeltaCamMove_X(), P1_Engine::getLastDeltaCamMove_Y()));
		P1_Engine::getWindow()->draw(*g_filter.get());
	}
}

void Filter::setActive(bool arg)
{
	isActive = arg;
}

bool Filter::getActive()
{
	return isActive;
}
