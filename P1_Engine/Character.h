#pragma once
#include "Actor.h"
#include "Layer.h"
class Character :
	public Actor
{
public:
	Character();
	Character(std::string txFile); //Constructeur des Characters statiques (useless ?)
	Character(std::string txFile, int animLenght, int animSpeed); //Constructeur des Characters animés
	~Character();

	//Mutateurs de currentDirection
	void moveRight(); 
	void moveLeft(); 
	void moveUp();
	void moveDown();
	void stopMove();
	//-----------------------------

	void move(int dX, int dY);

	void update(); //Recharge le Character à chaque frame
	bool getIsMoving();
	void setSpeed(float newSpeed); //Mutateur de speed

protected:
	float m_speed;
	sf::Vector2i m_pos;
	sf::Vector2i m_finalPos;
	sf::Vector2i m_factorPos;
	bool isMoving;
	bool stop;
};

