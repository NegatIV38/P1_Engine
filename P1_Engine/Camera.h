#pragma once
#include <vector>
#include <memory>
#include "SFML\Graphics.hpp"
#include <cmath>
#include "Actor.h"

class Actor;

class Camera
{
public:
	Camera();
	~Camera();

	void drawView(sf::RenderWindow& win);
	void update();
	std::shared_ptr<sf::View> getView();
	void move(int dX, int dY);
	sf::Vector2f adjustPos(sf::Vector2i vect);
	sf::Vector2f adjustPos(sf::Vector2f vect);
	bool getIsMoving();
	void centerOnActor(Actor& target);
	

private:
	std::shared_ptr<sf::View> m_view;
	int m_speed;
	sf::Vector2i m_pos;
	sf::Vector2i m_finalPos;
	sf::Vector2i m_factorPos;
	bool isMoving;

};


