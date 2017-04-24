#pragma once
#include "P1_Engine.h"

class Splashscreen
{
public:
	Splashscreen();
	~Splashscreen();

	void showTitle();
	void showSub();
	void unshow();
	void draw();
	void update();

private:
	std::shared_ptr<sf::RectangleShape> g_title;
	std::shared_ptr<sf::Text> m_title;
	std::shared_ptr<sf::Text> m_subtitle;
	std::shared_ptr<sf::Clock> m_clock;
	int m_time;
	int m_secTime;
	bool gradSub;
	bool noShow;
};

