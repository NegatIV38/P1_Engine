#include "Battler.h"



Battler::Battler()
{
}

Battler::Battler(std::string txFile) :
	Actor(txFile)
{
}

Battler::Battler(std::string txFile, int animLenght, int animSpeed) :
	Actor(txFile,animLenght,animSpeed)
{
}


Battler::~Battler()
{
}
