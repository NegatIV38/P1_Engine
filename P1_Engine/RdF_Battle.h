#pragma once
#include "P1_Engine.h"
class RdF_Battle
{
public:
	RdF_Battle();
	~RdF_Battle();

	void update();
	void draw();

	void setCenterValue(double arg);

	void initBackground();
	void initAdv();
	void initPers();

private:
	std::shared_ptr<sf::RectangleShape> g_back;
	std::shared_ptr<sf::RectangleShape> g_adv;
	std::shared_ptr<sf::RectangleShape> g_pers;

	double m_centerValue;

};

