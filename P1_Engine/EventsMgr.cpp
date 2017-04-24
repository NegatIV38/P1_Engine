#include "EventsMgr.h"



EventsMgr::EventsMgr(std::string doc)
{
	std::ifstream stream(doc, std::ios::in);
	std::string file;
	std::string line;
	if (stream) {
		while (std::getline(stream, line))
		{
			file.append(line);
		}
		stream.close();
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier des events" << std::endl;
	}
	std::string event_entry;
	std::string eventType;
	file.replace(0, file.find("<Entry "), "");
	file.replace(file.find("</Events>"), file.size() - 1, "");
	while (file != "") {
		event_entry = P1_Engine::getStringBetween(file, "<Entry", "</Entry>");
		eventType = P1_Engine::getStringBetween(event_entry, "eventType=\"", "\">\t");
		if (eventType == "Vente") {
			m_storesList.push_back(createVente(event_entry));
		}
		else if (eventType == "Combat") {
			m_battlesList.push_back(createCombat(event_entry));
		}
		file.replace(0, file.find("</Entry>") + std::string("</Entry>").size(), "");
		file.replace(0, file.find("<Entry"), "");
	}
}


EventsMgr::~EventsMgr()
{
}

P1_Engine::Event_Vente EventsMgr::createVente(std::string str)
{
	P1_Engine::Event_Vente ev;
	ev.eventID = P1_Engine::getStringBetween(str, "eventID=\"", "\" eventType");
	ev.pnjName = P1_Engine::getStringBetween(str, "<Name>", "</Name>");
	std::string objects = P1_Engine::getStringBetween(str, "<Objets>", "</Objets>");
	while (objects != "")
	{
		std::string ob = P1_Engine::getStringBetween(objects, "<Objet ", "</Objet>");
		ev.objets.push_back(createObjet(ob));
		objects.replace(0, objects.find("</Objet>"), "");
		objects.replace(0, objects.find("<Objet "), "");
	}
	return ev;
}

P1_Engine::Event_Objet EventsMgr::createObjet(std::string str)
{
	P1_Engine::Event_Objet eo;
	eo.objectID = P1_Engine::getStringBetween(str, "objectID=\"", "\">\t");
	eo.objectName = P1_Engine::getStringBetween(str, "<Nom>", "</Nom>");
	eo.prix = atoi(P1_Engine::getStringBetween(str, "<Prix>", "</Prix>").c_str());
	eo.description = P1_Engine::getStringBetween(str, "<Desc>", "</Desc>");
	return eo;
}

P1_Engine::Event_Combat EventsMgr::createCombat(std::string str)
{
	P1_Engine::Event_Combat ec;
	ec.eventID = P1_Engine::getStringBetween(str, "eventID=\"", "\" eventType");
	return ec;
}

P1_Engine::Event_Vente EventsMgr::getEventVente(std::string id)
{
	for (int i = 0; i < m_storesList.size(); i++) {
		if (m_storesList.at(i).eventID == id) {
			return m_storesList.at(i);
		}
	}
	P1_Engine::Event_Vente nullEvent;
	nullEvent.eventID = "NONE";
	nullEvent.pnjName = "NONE";
	return nullEvent;
}

P1_Engine::Event_Combat EventsMgr::getEventCombat(std::string id)
{
	for (int i = 0; i < m_battlesList.size(); i++) {
		if (m_battlesList.at(i).eventID == id) {
			return m_battlesList.at(i);
		}
	}
	P1_Engine::Event_Combat nullEvent;
	nullEvent.eventID = "NONE";
	return nullEvent;
}
