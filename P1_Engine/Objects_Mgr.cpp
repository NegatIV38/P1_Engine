#include "Objects_Mgr.h"



Objects_Mgr::Objects_Mgr(std::string doc)
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
		std::cerr << "Erreur lors de l'ouverture du fichier des objets" << std::endl;
	}

	std::string objet;
	file.replace(0, file.find("<Entry "), "");
	file.replace(file.find("</Objects>"), file.size() - 1, "");
	while (file != "") {
		objet = P1_Engine::getStringBetween(file, "<Entry", "</Entry>");
		m_objectList.push_back(createObject(objet));
		file.replace(0, file.find("</Entry>") + std::string("</Entry>").size(), "");
		file.replace(0, file.find("<Entry"), "");
	}
}


Objects_Mgr::~Objects_Mgr()
{
}

P1_Engine::Objet_ID Objects_Mgr::createObject(std::string str)
{
	P1_Engine::Objet_ID ob;
	ob.objectID = P1_Engine::getStringBetween(str, "objectID=\"", "\" objectType");
	std::string temp = P1_Engine::getStringBetween(str, "objectType=\"", "\">\t");
	if (temp == "Consommables") {
		ob.objectType = OBJECT_TYPE::CONSOMMABLE;
	}
	ob.objectName = P1_Engine::getStringBetween(str, "<Name>", "</Name>");
	return ob;
}

P1_Engine::Objet_ID Objects_Mgr::getObject(std::string id)
{
	for (int i = 0; i < m_objectList.size(); i++) {
		if (m_objectList.at(i).objectID == id) {
			return m_objectList.at(i);
		}
	}
	P1_Engine::Objet_ID nullObject;
	nullObject.objectID = "NONE";
	nullObject.objectName = "NONE";
	return nullObject;
}
