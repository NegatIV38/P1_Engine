#pragma once
#include "P1_Engine.h"
#include "Camera.h"
#include "Personnage.h"
#include "Objects_Mgr.h"


class VendorMenu
{
public:
	VendorMenu();
	~VendorMenu();

	void initCadreList();
	void initText(std::shared_ptr<sf::Text>& text);
	void initActionList();

	void recadre();
	void recadreCadreList();
	void recadreObjectList(std::vector<std::shared_ptr<sf::Text>>* list);

	void selectUp(std::vector<std::shared_ptr<sf::Text>>* list);
	void selectDown(std::vector<std::shared_ptr<sf::Text>>* list);
	void deselectOption(std::vector<std::shared_ptr<sf::Text>>* list);
	void reloadOptionSelected(std::vector<std::shared_ptr<sf::Text>>* list);
	void makeInventoryList();

	int getLargerObjectText();

	void loadFromVenteEvent(P1_Engine::Event_Vente& ev);
	void draw();
	void update();

	void executerAction();
	void actionAcheter();
	void actionQuitter();
	void actionRetour();
	void actionVendre();


	void show(bool arg);
	bool getIsShowing();

	std::vector<std::shared_ptr<sf::Text>>* getListToShow();

private:
	std::shared_ptr<sf::RectangleShape> g_cadreList;
	std::shared_ptr<sf::RectangleShape> g_vendorInfos;
	std::shared_ptr<sf::RectangleShape> g_persInventaire;

	std::vector<std::shared_ptr<sf::Text>> m_objectList;
	std::vector<std::shared_ptr<sf::Text>> m_actionList;
	std::vector<std::shared_ptr<sf::Text>> m_inventoryList;
	std::vector<std::string> m_objectIDList;
	std::vector<std::string> m_inventoryIDList;

	bool isShowing;
	int m_outlineSize;
	int m_currentItemSelected;
	int m_menuSelected;
	
};

