#include "PauseMenu.h"



PauseMenu::PauseMenu()
{
	g_cadreActions = std::make_shared<sf::RectangleShape>();
	g_mapName = std::make_shared<sf::RectangleShape>();
	g_money = std::make_shared<sf::RectangleShape>();
	g_cadreStatus = std::make_shared<sf::RectangleShape>();

	m_moneyText = std::make_shared<sf::Text>();
	m_mapNameText = std::make_shared<sf::Text>();

	m_outlineSize = 5;
	m_currentScreen = PauseMenuScreens::BASE;
	m_currentSelection = 0;

	initText(m_moneyText);
	initText(m_mapNameText);
	initActionList();
	initMoneyCadre();
	initMapNameCadre();
	initCadreActions();
	initCadreStatus();

}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::open()
{
	reloadOptionSelected();
}

void PauseMenu::update()
{
	m_moneyText->setString("Gold : " + std::to_string(P1_Engine::getPersonnage()->getGold()));
	m_mapNameText->setString("TestMap");
	recadreMoney();
	recadreMapName();
	recadreActions();
	recadreStatus();
	recadreInventory();
}

void PauseMenu::draw()
{
	drawMoney();
	drawMapName();
	drawActions();
	drawStatus();
	if (m_currentScreen == PauseMenuScreens::INVENTAIRE) {
		drawInventory();
	}
}

void PauseMenu::execute()
{
	switch (m_currentScreen)
	{
	case PauseMenuScreens::BASE:
		if (m_currentSelection == 0) {
			executeInventory();
		}
		break;
	case PauseMenuScreens::INVENTAIRE:
		break;
	default:
		break;
	}
}

void PauseMenu::exit()
{
	deselectOption();
	m_currentScreen = PauseMenuScreens::BASE;
	m_currentSelection = 0;
}

void PauseMenu::initText(std::shared_ptr<sf::Text>& text)
{
	text->setFont(*P1_Engine::getFont());
	text->setCharacterSize(32);
	text->setColor(sf::Color::White);
}

void PauseMenu::initActionList()
{
	m_actionList.push_back(std::make_shared<sf::Text>("Inventaire", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Arcanes", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Equipement", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Quetes", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Recherches", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Options", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Sauvegarder", *P1_Engine::getFont(), 32));
	m_actionList.push_back(std::make_shared<sf::Text>("Quitter", *P1_Engine::getFont(), 32));
}

void PauseMenu::initInventory()
{
	m_inventory.clear();
	for (std::map<std::string, int>::iterator it = P1_Engine::getPersonnage()->getInventory()->begin();
		it != P1_Engine::getPersonnage()->getInventory()->end(); it++) {
		m_inventory.push_back(std::make_shared<sf::Text>(P1_Engine::getMGR_Objects()->getObject(it->first).objectName + "\tx" + std::to_string(it->second), *P1_Engine::getFont(), 32));
	}
}

void PauseMenu::initMoneyCadre()
{
	g_money->setOutlineThickness(m_outlineSize);
	g_money->setOutlineColor(sf::Color::White);
	g_money->setFillColor(sf::Color::Black);
}

void PauseMenu::initMapNameCadre()
{
	g_mapName->setOutlineThickness(m_outlineSize);
	g_mapName->setOutlineColor(sf::Color::White);
	g_mapName->setFillColor(sf::Color::Black);
}

void PauseMenu::initCadreActions()
{
	g_cadreActions->setOutlineThickness(m_outlineSize);
	g_cadreActions->setOutlineColor(sf::Color::White);
	g_cadreActions->setFillColor(sf::Color::Black);
}

void PauseMenu::initCadreStatus()
{
	g_cadreStatus->setOutlineThickness(m_outlineSize);
	g_cadreStatus->setOutlineColor(sf::Color::White);
	g_cadreStatus->setFillColor(sf::Color::Black);
}

void PauseMenu::recadreMoney()
{
	g_money->setSize(sf::Vector2f(400, 75));
	g_money->setOrigin(g_money->getSize().x/2,g_money->getSize().y/2);
	g_money->setPosition(P1_Engine::getCamera()->getView()->getCenter() + sf::Vector2f(P1_Engine::getNativeResolutionX()/2 - g_money->getSize().x/2 - m_outlineSize, P1_Engine::getNativeResolutionY()/2 - g_money->getSize().y /2 - m_outlineSize));
	sf::FloatRect rect = m_moneyText->getLocalBounds();
	m_moneyText->setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	m_moneyText->setPosition(g_money->getPosition());
}

void PauseMenu::recadreMapName()
{
	g_mapName->setSize(sf::Vector2f(400, 75));
	g_mapName->setOrigin(g_mapName->getSize().x / 2, g_mapName->getSize().y / 2);
	g_mapName->setPosition(P1_Engine::getCamera()->getView()->getCenter() + sf::Vector2f(P1_Engine::getNativeResolutionX() / 2 - g_mapName->getSize().x / 2 - m_outlineSize, P1_Engine::getNativeResolutionY() / 2 - g_mapName->getSize().y*1.5f - m_outlineSize));
	sf::FloatRect rect = m_moneyText->getLocalBounds();
	m_mapNameText->setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	m_mapNameText->setPosition(g_mapName->getPosition());
}

void PauseMenu::recadreActions()
{
	g_cadreActions->setSize(sf::Vector2f(400, P1_Engine::getNativeResolutionY() - 2*75 - 3*m_outlineSize));
	g_cadreActions->setOrigin(g_cadreActions->getSize().x / 2, 0);
	g_cadreActions->setPosition(P1_Engine::getCamera()->getView()->getCenter() + sf::Vector2f(P1_Engine::getNativeResolutionX() / 2 - g_cadreActions->getSize().x / 2 - m_outlineSize, -P1_Engine::getNativeResolutionY()/2 + m_outlineSize));
	for (int i = 0; i < m_actionList.size(); i++) {
		sf::FloatRect rect = m_actionList.at(i)->getLocalBounds();
		m_actionList.at(i)->setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		m_actionList.at(i)->setPosition(g_cadreActions->getPosition() + sf::Vector2f(0,30 + i*70));
	}
}

void PauseMenu::recadreStatus()
{
	g_cadreStatus->setSize(sf::Vector2f(P1_Engine::getNativeResolutionX() - 400 - m_outlineSize, P1_Engine::getNativeResolutionY() - 2  * m_outlineSize));
	g_cadreStatus->setOrigin(0, 0);
	g_cadreStatus->setPosition(P1_Engine::getCamera()->getView()->getCenter() + sf::Vector2f(-P1_Engine::getNativeResolutionX()/2 + m_outlineSize,-P1_Engine::getNativeResolutionY()/2+m_outlineSize));

}

void PauseMenu::recadreInventory()
{
	for (int i = 0; i < m_inventory.size(); i++) {
		sf::FloatRect rect = m_inventory.at(i)->getLocalBounds();
		//m_inventory.at(i)->setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		m_inventory.at(i)->setPosition(g_cadreStatus->getPosition() + sf::Vector2f(30, 30 + i * 70));
	}
}

void PauseMenu::drawMoney()
{
	P1_Engine::getWindow()->draw(*g_money.get());
	P1_Engine::getWindow()->draw(*m_moneyText.get());
}

void PauseMenu::drawMapName()
{
	P1_Engine::getWindow()->draw(*g_mapName.get());
	P1_Engine::getWindow()->draw(*m_mapNameText.get());
}

void PauseMenu::drawActions()
{
	P1_Engine::getWindow()->draw(*g_cadreActions.get());
	for (std::vector<std::shared_ptr<sf::Text>>::iterator it = m_actionList.begin(); it != m_actionList.end(); it++) {
		P1_Engine::getWindow()->draw(*it->get());
	}
}

void PauseMenu::drawStatus()
{
	P1_Engine::getWindow()->draw(*g_cadreStatus.get());
}

void PauseMenu::drawInventory()
{
	for (std::vector<std::shared_ptr<sf::Text>>::iterator it = m_inventory.begin(); it != m_inventory.end(); it++) {
		P1_Engine::getWindow()->draw(*it->get());
	}
}

void PauseMenu::selectUp()
{
	if (m_currentSelection > 0) {
		deselectOption();
		m_currentSelection--;
		reloadOptionSelected();
	}
	P1_Engine::playSound("a_res/Select_0.wav");
}

void PauseMenu::selectDown()
{
	if (m_currentSelection < m_actionList.size() - 1) {
		deselectOption();
		m_currentSelection++;
		reloadOptionSelected();
	}
	P1_Engine::playSound("a_res/Select_0.wav");
}

void PauseMenu::deselectOption()
{
	std::string str = m_actionList.at(m_currentSelection)->getString();
	if (str.at(0) == '>' && str.at(1) == ' ') {
		std::size_t pos = str.find("> ");
		pos += 2;
		str = str.substr(pos);
	}
	m_actionList.at(m_currentSelection)->setString(str);
}

void PauseMenu::reloadOptionSelected()
{
	std::string str = m_actionList.at(m_currentSelection)->getString();
	str = "> " + str;
	m_actionList.at(m_currentSelection)->setString(str);
}

void PauseMenu::executeInventory()
{
	initInventory();
	m_currentScreen = PauseMenuScreens::INVENTAIRE;
}
