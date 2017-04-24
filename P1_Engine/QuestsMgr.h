#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include "P1_Engine.h"

class QuestsMgr
{
public:
	QuestsMgr(std::string doc);
	~QuestsMgr();

	P1_Engine::Quete createQuete(std::string str);
	P1_Engine::Quest_option createOption(std::string str);

	std::vector<P1_Engine::Quete>* getQuestList();
	P1_Engine::Quete getQuete(std::string id);

private:
	std::vector<P1_Engine::Quete> m_questsList;
};

