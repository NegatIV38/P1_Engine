#pragma once
#include "Character.h"
#include <list>

class Personnage :
	public Character
{
public:
	Personnage();
	Personnage(std::string txFile);
	Personnage(std::string txFile, int animSpeed);
	~Personnage();

	void initPosition(int x, int y);

	std::map<std::string, int>* getInventory();
	unsigned int getGold();

private:
	std::map<std::string,int> m_inventory;
	unsigned int m_gold;
};

