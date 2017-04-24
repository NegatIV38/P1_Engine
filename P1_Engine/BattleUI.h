#pragma once
#include "RdF_Battle.h"
#include "Battler.h"
class BattleUI
{
public:
	BattleUI();
	~BattleUI();

	void update();
	void draw();

	
private:
	std::shared_ptr<RdF_Battle> g_RdFBarre;
	std::vector<std::shared_ptr<Battler>> m_advList;
	std::vector<std::shared_ptr<Battler>> m_persList;
};

