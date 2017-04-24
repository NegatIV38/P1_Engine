#include "QuestsMgr.h"



QuestsMgr::QuestsMgr(std::string doc)
{
	std::ifstream stream(doc, std::ios::in);
	std::string file;
	std::string line;
	if (stream) {
		while (std::getline(stream,line))
		{
			file.append(line);
		}
		stream.close();
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier de quetes" << std::endl;
	}
	std::string quete;
	file.replace(0, file.find("<Entry "), "");
	file.replace(file.find("</Quests>"), file.size() - 1, "");
	while (file != "") {
		quete = P1_Engine::getStringBetween(file, "<Entry", "</Entry>");
		m_questsList.push_back(createQuete(quete));
		file.replace(0, file.find("</Entry>") + std::string("</Entry>").size(), "");
		file.replace(0, file.find("<Entry"), "");
	} 
}

P1_Engine::Quete QuestsMgr::createQuete(std::string str) {
	P1_Engine::Quete qt;
	qt.questID = P1_Engine::getStringBetween(str, "questID=\"", "\" hasOption");
	std::string temp = P1_Engine::getStringBetween(str, "hasOption=\"", "\" target");
	qt.hasOption = temp == "true";
	qt.targetID = P1_Engine::getStringBetween(str, "target=\"", "\" hasName");
	qt.hasName = P1_Engine::getStringBetween(str, "hasName=\"", "\" hasEvent") == "true";
	qt.hasEvent = P1_Engine::getStringBetween(str, "hasEvent=\"", "\">\t") == "true";
	if (qt.hasName) {
		qt.name = P1_Engine::getStringBetween(str, "<Name>", "</Name>");
	}
	qt.text = P1_Engine::getStringBetween(str, "<Text>", "</Text>");
	if (qt.hasOption) {
		std::string options = P1_Engine::getStringBetween(str, "<Options>", "</Options>");
		while (options != "")
		{
			std::string op = P1_Engine::getStringBetween(options, "<Option", ">");
			qt.options.push_back(createOption(op));
			options.replace(0, options.find("/>"), "");
			options.replace(0, options.find("<Option"), "");
		}

	}
	if (qt.hasEvent) {
		qt.eventID = P1_Engine::getStringBetween(str, "eventID=\"", "\" eventType");
		temp = P1_Engine::getStringBetween(str, "eventType=\"", "\"/>\t");
		if (temp == "Vente") {
			qt.eventType = EVENT_TYPE::VENTE;
		}
		else if(temp == "Combat"){
			qt.eventType = EVENT_TYPE::COMBAT;
		}
	}
	return qt;
}

P1_Engine::Quest_option QuestsMgr::createOption(std::string str)
{
	P1_Engine::Quest_option op;
	op.targetID = P1_Engine::getStringBetween(str, "target=\"", "\" text");
	op.text = P1_Engine::getStringBetween(str, "text=\"","\"/" );
	return op;
}

std::vector<P1_Engine::Quete>* QuestsMgr::getQuestList()
{
	return &m_questsList;
}

P1_Engine::Quete QuestsMgr::getQuete(std::string id)
{
	for (int i = 0; i < m_questsList.size(); i++) {
		if (m_questsList.at(i).questID == id) {
			return m_questsList.at(i);
		}
	}
	P1_Engine::Quete nullQuest;
	nullQuest.questID = "NONE";
	nullQuest.hasOption = false;
	nullQuest.targetID = "NONE";
	nullQuest.text = "";
	return nullQuest;

}

QuestsMgr::~QuestsMgr()
{
}
