#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include "P1_Engine.h"

class Objects_Mgr
{
public:
	Objects_Mgr(std::string doc);
	~Objects_Mgr();

	P1_Engine::Objet_ID createObject(std::string str);

	P1_Engine::Objet_ID getObject(std::string id);

private:
	std::vector<P1_Engine::Objet_ID> m_objectList;

};

