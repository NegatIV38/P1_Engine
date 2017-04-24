#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include "P1_Engine.h"

class EventsMgr
{
public:
	EventsMgr(std::string doc);
	~EventsMgr();

	P1_Engine::Event_Vente createVente(std::string str);
	P1_Engine::Event_Objet createObjet(std::string str);

	P1_Engine::Event_Combat createCombat(std::string str);

	P1_Engine::Event_Vente getEventVente(std::string id);
	P1_Engine::Event_Combat getEventCombat(std::string id);

private:
	std::vector<P1_Engine::Event_Vente> m_storesList;
	std::vector<P1_Engine::Event_Combat> m_battlesList;
};

