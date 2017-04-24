#include "Personnage.h"



Personnage::Personnage()
{
}

Personnage::Personnage(std::string txFile) :
	Character(txFile)
{
	m_gold = 0;
}

Personnage::Personnage(std::string txFile, int animSpeed) :
	Character(txFile,4,animSpeed)
{
	m_gold = 0;
}


Personnage::~Personnage()
{
}

void Personnage::initPosition(int x, int y)
{
	m_pos = sf::Vector2i(x*(P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX()), y*(P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleY()));
	m_finalPos = m_pos;
}

std::map<std::string, int>* Personnage::getInventory()
{
	return &m_inventory;
}

unsigned int Personnage::getGold()
{
	return m_gold;
}
