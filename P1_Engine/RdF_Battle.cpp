#include "RdF_Battle.h"



RdF_Battle::RdF_Battle()
{
	g_back = std::make_shared<sf::RectangleShape>();
	g_adv = std::make_shared<sf::RectangleShape>();
	g_pers = std::make_shared<sf::RectangleShape>();

	setCenterValue(0.5);

	initBackground();
	initAdv();
	initPers();
}


RdF_Battle::~RdF_Battle()
{
}

void RdF_Battle::update()
{

	g_adv->setSize(sf::Vector2f(g_back->getSize().x * m_centerValue, g_back->getSize().y));
	g_pers->setSize(sf::Vector2f(g_back->getSize().x - g_adv->getSize().x, g_back->getSize().y));

	g_back->setPosition(sf::Vector2f(P1_Engine::getNativeResolutionX() / 2, g_back->getSize().y + 10));
	g_adv->setPosition(g_back->getPosition() - sf::Vector2f(g_back->getSize().x / 2, g_back->getSize().y/2));
	g_pers->setPosition(g_adv->getPosition() + sf::Vector2f(g_adv->getSize().x, 0));
}

void RdF_Battle::draw()
{
	P1_Engine::getWindow()->draw(*g_back.get());
	P1_Engine::getWindow()->draw(*g_adv.get());
	P1_Engine::getWindow()->draw(*g_pers.get());
}

void RdF_Battle::setCenterValue(double arg)
{
	m_centerValue = arg;
}

void RdF_Battle::initBackground()
{
	g_back->setFillColor(sf::Color::Black);
	g_back->setOutlineColor(sf::Color::White);
	g_back->setOutlineThickness(5);
	g_back->setSize(sf::Vector2f(P1_Engine::getNativeResolutionX() - 100, 30));
	g_back->setOrigin(g_back->getSize().x / 2, g_back->getSize().y / 2);

}

void RdF_Battle::initAdv()
{
	g_adv->setFillColor(sf::Color::Red);
	g_adv->setOrigin(0, 0);
}

void RdF_Battle::initPers()
{
	g_pers->setFillColor(sf::Color::Blue);
	g_pers->setOrigin(0, 0);
}
