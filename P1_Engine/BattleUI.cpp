#include "BattleUI.h"



BattleUI::BattleUI()
{
	g_RdFBarre = std::make_shared<RdF_Battle>();
}


BattleUI::~BattleUI()
{
}

void BattleUI::update()
{
	g_RdFBarre->update();
}

void BattleUI::draw()
{
	g_RdFBarre->draw();
}
