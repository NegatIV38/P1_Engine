#pragma once
#include "Actor.h"
class Battler :
	public Actor
{
public:
	Battler();
	Battler(std::string txFile);
	Battler(std::string txFile, int animLenght, int animSpeed);
	~Battler();
};

