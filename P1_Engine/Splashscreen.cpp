#include "Splashscreen.h"



Splashscreen::Splashscreen()
{
	m_title = std::make_shared<sf::Text>();
	m_subtitle = std::make_shared<sf::Text>();
	m_clock = std::make_shared<sf::Clock>();
	g_title = std::make_shared<sf::RectangleShape>();
	m_time = 3500;

	m_title->setFont(*P1_Engine::getFont());
	m_subtitle->setFont(*P1_Engine::getFont());

	m_title->setCharacterSize(96);
	m_subtitle->setCharacterSize(42);

	m_title->setColor(sf::Color::Black);
	m_subtitle->setColor(sf::Color::Black);

	m_title->setString("P1 Engine");
	m_subtitle->setString("SAMUEL ATHANASE - 2017");

	g_title->setFillColor(sf::Color::Black);
	g_title->setOutlineColor(sf::Color::Black);
	g_title->setOutlineThickness(5);

	m_clock->restart();
	gradSub = false;
	noShow = false;
}


Splashscreen::~Splashscreen()
{
}

void Splashscreen::showTitle()
{
	if (m_clock->getElapsedTime().asMilliseconds() < m_time) {
		int g = (m_clock->getElapsedTime().asMilliseconds() * 255) / m_time;
		m_title->setColor(sf::Color(g,g,g));
		g_title->setOutlineColor(sf::Color(g, g, g));
	}
	else {
		m_clock->restart();
		gradSub = true;
	}

}

void Splashscreen::showSub()
{
	if (m_clock->getElapsedTime().asMilliseconds() < m_time) {
		int g = (m_clock->getElapsedTime().asMilliseconds() * 255) / m_time;
		m_subtitle->setColor(sf::Color(g,g,g));
		m_secTime = m_clock->getElapsedTime().asMilliseconds() + 1000;
	}
	else {
		if (m_clock->getElapsedTime().asMilliseconds() > m_secTime) {
			m_clock->restart();
			noShow = true;
		}
	}
}

void Splashscreen::unshow()
{
	if (m_clock->getElapsedTime().asMilliseconds() < m_time) {
		int g = (m_clock->getElapsedTime().asMilliseconds() * 255) / m_time;
		m_subtitle->setColor(sf::Color(255-g, 255-g, 255-g));
		m_title->setColor(sf::Color(255 - g, 255 - g, 255 - g));
		g_title->setOutlineColor(sf::Color(255 - g, 255 - g, 255 - g));
		m_secTime = m_clock->getElapsedTime().asMilliseconds() + 250;
	}
	else {
		if (m_clock->getElapsedTime().asMilliseconds() > m_secTime) {
			P1_Engine::setCurrentState(MainStates::MAIN_MENU);
		}
	}
}

void Splashscreen::draw()
{
	P1_Engine::getWindow()->draw(*g_title.get());
	P1_Engine::getWindow()->draw(*m_title.get());
	P1_Engine::getWindow()->draw(*m_subtitle.get());
}

void Splashscreen::update()
{
	sf::FloatRect titleRect = m_title->getLocalBounds();
	sf::FloatRect subRect = m_subtitle->getLocalBounds();

	m_title->setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
	m_subtitle->setOrigin(subRect.left + subRect.width / 2.0f, subRect.top + subRect.height / 2.0f);

	m_title->setPosition(sf::Vector2f(P1_Engine::getNativeResolutionX() / 2, P1_Engine::getNativeResolutionY() / 2));
	m_subtitle->setPosition(sf::Vector2f(P1_Engine::getNativeResolutionX()/2, P1_Engine::getNativeResolutionY() - 60));

	g_title->setSize(sf::Vector2f(titleRect.width * 1.25f, titleRect.height *1.5f));
	g_title->setOrigin(g_title->getSize().x /2 , g_title->getSize().y / 2);
	g_title->setPosition(m_title->getPosition() - sf::Vector2f(0,10));

	if (!gradSub) {
		showTitle();
	}
	else {
		if (!noShow) {
			showSub();
		}
		else {
			unshow();
		}
	}
}
