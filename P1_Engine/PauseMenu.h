#pragma once
#include "P1_Engine.h"
#include "Personnage.h"
#include <string>
#include "Camera.h"
#include "Objects_Mgr.h"

enum class PauseMenuScreens {
	BASE, INVENTAIRE
};

class PauseMenu
{
public:
	PauseMenu();
	~PauseMenu();

	void open();
	void update();
	void draw();
	void execute();
	void exit();

	void initText(std::shared_ptr<sf::Text>& text);
	void initActionList();
	void initInventory();

	void initMoneyCadre();
	void initMapNameCadre();
	void initCadreActions();
	void initCadreStatus();

	void recadreMoney();
	void recadreMapName();
	void recadreActions();
	void recadreStatus();
	void recadreInventory();

	void drawMoney();
	void drawMapName();
	void drawActions();
	void drawStatus();
	void drawInventory();

	void selectUp();
	void selectDown();
	void deselectOption();
	void reloadOptionSelected();

	void executeInventory();

private:
	std::shared_ptr<sf::RectangleShape> g_cadreActions;
	std::shared_ptr<sf::RectangleShape> g_money;
	std::shared_ptr<sf::RectangleShape> g_mapName;
	std::shared_ptr<sf::RectangleShape> g_cadreStatus;

	std::vector<std::shared_ptr<sf::Text>> m_actionList;
	std::vector<std::shared_ptr<sf::Text>> m_inventory;
	std::shared_ptr<sf::Text> m_moneyText;
	std::shared_ptr<sf::Text> m_mapNameText;

	int m_outlineSize;
	PauseMenuScreens m_currentScreen;
	int m_currentSelection;

};

