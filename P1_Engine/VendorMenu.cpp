#include "VendorMenu.h"



VendorMenu::VendorMenu()
{
	g_cadreList = std::make_shared<sf::RectangleShape>();
	g_vendorInfos = std::make_shared<sf::RectangleShape>();
	g_persInventaire = std::make_shared<sf::RectangleShape>();

	m_outlineSize = 5;
	m_currentItemSelected = 0;
	m_menuSelected = 0;
	initCadreList();

	initActionList();
	isShowing = false;
}


VendorMenu::~VendorMenu()
{
}

void VendorMenu::initCadreList()
{
	
	
	g_cadreList->setOutlineColor(sf::Color::White);
	g_cadreList->setOutlineThickness(m_outlineSize);
	g_cadreList->setFillColor(sf::Color::Black);
}

void VendorMenu::initText(std::shared_ptr<sf::Text>& text)
{
	text->setFont(*P1_Engine::getFont());
	text->setCharacterSize(32);
	text->setColor(sf::Color::White);
	
}

void VendorMenu::initActionList()
{
	m_actionList.push_back(std::make_shared<sf::Text>("Acheter", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Vendre", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Quitter", *P1_Engine::getFont(), 32));
}

void VendorMenu::recadre()
{
	recadreCadreList();
	recadreObjectList(getListToShow());
}

void VendorMenu::recadreCadreList()
{
	g_cadreList->setSize(sf::Vector2f(getLargerObjectText() + 40, P1_Engine::getNativeResolutionY() - (2 * m_outlineSize)));
	g_cadreList->setOrigin(g_cadreList->getSize().x / 2, g_cadreList->getSize().y / 2);
	g_cadreList->setPosition(P1_Engine::getCamera()->getView()->getCenter() +
		sf::Vector2f(P1_Engine::getNativeResolutionX()/2 - g_cadreList->getSize().x/2 - m_outlineSize,0));
}

void VendorMenu::recadreObjectList(std::vector<std::shared_ptr<sf::Text>>* list)
{
	for (int i = 0; i < list->size(); i++) {
		list->at(i)->setPosition(g_cadreList->getPosition() - sf::Vector2f(g_cadreList->getSize().x/2,g_cadreList->getSize().y/2) + sf::Vector2f(10, 10 + (i * 50)));
	}
}

void VendorMenu::selectUp(std::vector<std::shared_ptr<sf::Text>>* list)
{
	if (m_currentItemSelected > 0) {
		deselectOption(list);
		m_currentItemSelected--;
		reloadOptionSelected(list);
	}
	P1_Engine::playSound("a_res/Select_0.wav");
}

void VendorMenu::selectDown(std::vector<std::shared_ptr<sf::Text>>* list)
{
	if (m_currentItemSelected < list->size()-1) {
		deselectOption(list);
		m_currentItemSelected++;
		reloadOptionSelected(list);
	}
	P1_Engine::playSound("a_res/Select_0.wav");
}

void VendorMenu::deselectOption(std::vector<std::shared_ptr<sf::Text>>* list)
{
	std::string str = list->at(m_currentItemSelected)->getString();
	if (str.at(0) == '>' && str.at(1) == ' ') {
		std::size_t pos = str.find("> ");
		pos += 2;
		str = str.substr(pos);
	}
	list->at(m_currentItemSelected)->setString(str);
}

void VendorMenu::reloadOptionSelected(std::vector<std::shared_ptr<sf::Text>>* list)
{
	std::string str = list->at(m_currentItemSelected)->getString();
	str = "> " + str;
	list->at(m_currentItemSelected)->setString(str);
}

void VendorMenu::makeInventoryList()
{
	m_inventoryList.clear();
	m_inventoryIDList.clear();
	for (std::map<std::string, int>::iterator it = P1_Engine::getPersonnage()->getInventory()->begin();
		it != P1_Engine::getPersonnage()->getInventory()->end(); it++) {
		m_inventoryList.push_back(std::make_shared<sf::Text>(P1_Engine::getMGR_Objects()->getObject(it->first).objectName + "\tx" + std::to_string(it->second), *P1_Engine::getFont(), 32));
		m_inventoryIDList.push_back(P1_Engine::getMGR_Objects()->getObject(it->first).objectID);
	}
	m_inventoryList.push_back(std::make_shared<sf::Text>("Retour", *P1_Engine::getFont(), 32));
}

int VendorMenu::getLargerObjectText()
{
	int l = 0;
	for (int i = 0; i < m_objectList.size(); i++) {
		if (l < m_objectList.at(i)->getLocalBounds().width) {
			l = m_objectList.at(i)->getLocalBounds().width;
		}
	}
	return l;
}

void VendorMenu::loadFromVenteEvent(P1_Engine::Event_Vente & ev)
{
	for (int i = 0; i < ev.objets.size(); i++) {
		m_objectList.push_back(std::make_shared<sf::Text>());
		m_objectList.back()->setString(ev.objets.at(i).objectName + "\t" + std::to_string(ev.objets.at(i).prix));
		initText(m_objectList.back());
		m_objectIDList.push_back(ev.objets.at(i).objectID);
	}
	m_objectList.push_back(std::make_shared<sf::Text>("Retour", *P1_Engine::getFont(), 32));
	reloadOptionSelected(getListToShow());
}

void VendorMenu::draw()
{
	if (isShowing) {
		P1_Engine::getWindow()->draw(*g_cadreList.get());
		if (m_menuSelected == 0) {
			for (int i = 0; i < m_actionList.size(); i++) {
				P1_Engine::getWindow()->draw(*m_actionList.at(i).get());
			}
		}
		else if (m_menuSelected == 1) {
			for (int i = 0; i < m_objectList.size(); i++) {
				P1_Engine::getWindow()->draw(*m_objectList.at(i).get());
			}
		}
		else if (m_menuSelected == 2) {
			for (int i = 0; i < m_inventoryList.size(); i++) {
				P1_Engine::getWindow()->draw(*m_inventoryList.at(i).get());
			}
		}
	}
}

void VendorMenu::update()
{
	recadre();
}

void VendorMenu::executerAction()
{
	switch (m_menuSelected) {
	case 0:
		switch (m_currentItemSelected) {
		case 0:
			actionAcheter();
			break;
		case 1:
			actionVendre();
			break;
		case 2:
			actionQuitter();
			break;
		}
		break;
	case 1:
		if (m_currentItemSelected == m_objectList.size() - 1) {
			actionRetour();
			break;
		}
		else {
			if (P1_Engine::getPersonnage()->getInventory()->find(m_objectIDList.at(m_currentItemSelected)) == 
					P1_Engine::getPersonnage()->getInventory()->end()) {
				P1_Engine::getPersonnage()->getInventory()->emplace(m_objectIDList.at(m_currentItemSelected), 1);
			}
			else {
				P1_Engine::getPersonnage()->getInventory()->at(m_objectIDList.at(m_currentItemSelected))++;
			}
			break;
		}
	case 2:
		if (m_currentItemSelected == m_inventoryList.size() - 1) {
			actionRetour();
			break;
		}
		else {
			if (P1_Engine::getPersonnage()->getInventory()->size() > 0) {
				P1_Engine::getPersonnage()->getInventory()->at(m_inventoryIDList.at(m_currentItemSelected))--;
				makeInventoryList();
				reloadOptionSelected(getListToShow());
				if (P1_Engine::getPersonnage()->getInventory()->at(m_inventoryIDList.at(m_currentItemSelected)) <= 0) {
					P1_Engine::getPersonnage()->getInventory()->erase(m_inventoryIDList.at(m_currentItemSelected));
					if (m_currentItemSelected > 0) {
						m_currentItemSelected--;
					}
					deselectOption(getListToShow());
					makeInventoryList();
					reloadOptionSelected(getListToShow());
				}
			}
			break;
		}
	default:
		break;
	}
	P1_Engine::playSound("a_res/ValidateOption.wav");
}

void VendorMenu::actionAcheter()
{
	deselectOption(getListToShow());
	m_menuSelected = 1;
	m_currentItemSelected = 0;
	reloadOptionSelected(getListToShow());
}

void VendorMenu::actionQuitter()
{
	deselectOption(getListToShow());
	m_menuSelected = 0;
	m_currentItemSelected = 0;
	show(false);
	m_objectList.clear();
}

void VendorMenu::actionRetour()
{
	deselectOption(getListToShow());
	m_menuSelected = 0;
	m_currentItemSelected = 0;
	reloadOptionSelected(getListToShow());
}

void VendorMenu::actionVendre()
{
	deselectOption(getListToShow());
	m_menuSelected = 2;
	m_currentItemSelected = 0;
	makeInventoryList();
	reloadOptionSelected(getListToShow());
}

void VendorMenu::show(bool arg)
{
	isShowing = arg;
}

bool VendorMenu::getIsShowing()
{
	return isShowing;
}

std::vector<std::shared_ptr<sf::Text>>* VendorMenu::getListToShow()
{
	switch (m_menuSelected)
	{
	case 0:
		return &m_actionList;
		break;
	case 1:
		return &m_objectList;
		break;
	case 2:
		return &m_inventoryList;
	default:
		return nullptr;
		break;
	}
}
