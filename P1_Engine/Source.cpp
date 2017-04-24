#include <SFML/Graphics.hpp>
#include <iostream>
#include "Actor.h"
#include "Character.h"
#include "Layer.h"
#include "Dialogue.h"
#include "Camera.h"
#include "MainMenu.h"
#include "Filter.h"
#include "P1_Engine.h"
#include "Personnage.h"
#include "QuestsMgr.h"
#include "EventsMgr.h"
#include "VendorMenu.h"
#include "Objects_Mgr.h"
#include "Splashscreen.h"
#include "BattleUI.h"
#include "PauseMenu.h"

int main()
{
	using namespace sf;
	//Fenetre
	P1_Engine::initWindow();
	P1_Engine::getWindow()->setFramerateLimit(60);
	P1_Engine::getWindow()->setVerticalSyncEnabled(true);

	Splashscreen splash;

	//Font
	P1_Engine::initFont();

	P1_Engine::Quete currentQuest = P1_Engine::getMGR_Quetes()->getQuete("None");
	
	P1_Engine::getDialogue()->setDialogueFromQuete(currentQuest);

	Filter filter;
	filter.setColor(200, 25, 0, 50);

	P1_Engine::loadMap("map50");

	//Ombres
	P1_Engine::loadShadowsFromFile("g_res/shadows.png");
	
	//Menus Creation

	MainMenu menuPrincipal("Inserer Nom de Jeu ICI");
	MainMenu options("Options");


	menuPrincipal.setActive(true);
	menuPrincipal.pushOption("Start");
	menuPrincipal.setRowExecTable(1, COMMANDS::BEGIN_GAME);
	menuPrincipal.setTextColor(1, sf::Color::Green);
	menuPrincipal.pushOption("Battle_Test");
	menuPrincipal.setRowExecTable(2, COMMANDS::BEGIN_BATTLE);
	menuPrincipal.setTextColor(2, sf::Color::Yellow);
	menuPrincipal.pushOption("Options");
	menuPrincipal.setRowExecTable(3, COMMANDS::ENTER_OPTIONS);
	menuPrincipal.setRowParamTable(3, options.getName());
	menuPrincipal.setTextColor(3, sf::Color::Blue);
	menuPrincipal.pushOption("Quit");
	menuPrincipal.setRowExecTable(4, COMMANDS::QUIT_GAME);
	menuPrincipal.setTextColor(4, sf::Color::Red);
	menuPrincipal.reloadTextSelected();

	options.pushOption("Resolution");
	options.setRowExecTable(1, COMMANDS::UNKNOW);
	options.pushOption("Retour");
	options.setRowExecTable(2, COMMANDS::OPTIONS_BACK);
	options.setRowParamTable(2, menuPrincipal.getName());
	options.reloadTextSelected();


	//Sound Loading
	P1_Engine::loadSoundBuffer("a_res/Select_0.wav");
	P1_Engine::loadSoundBuffer("a_res/Validate_0.wav");
	P1_Engine::loadSoundBuffer("a_res/ValidateOption.wav");


	
	Personnage pers("g_res/charSHEET00.png", 6);
	pers.cleanTexture();
	pers.initPosition(20, 10);
	P1_Engine::setPersonnage(&pers);

	//RUNNING

	while (P1_Engine::getIsGameRunning())
	{

		//EVENTS

		Event event;
		while (P1_Engine::getWindow()->pollEvent(event))
		{
			if (event.type == Event::Closed) {
				P1_Engine::quitGame();
				P1_Engine::getWindow()->close();
			}

			if (event.type == Event::KeyPressed) {
				
				switch (P1_Engine::getCurrentState())
				{
				case MainStates::SPLASH:
					if (Keyboard::isKeyPressed(Keyboard::Return)) {
						P1_Engine::setCurrentState(MainStates::MAIN_MENU);
					}
					break;
				case MainStates::MAIN_MENU:
					if (Keyboard::isKeyPressed(Keyboard::Down)) {
						menuPrincipal.selectDown();
						options.selectDown();
					}
					if (Keyboard::isKeyPressed(Keyboard::Up)) {
						menuPrincipal.selectUp();
						options.selectUp();
					}
					if (Keyboard::isKeyPressed(Keyboard::Return)) {
						menuPrincipal.execute();
						options.execute();

						MainMenu::setStaticExec(false);
					}
					break;
				case MainStates::IN_GAME_MENU:
					if (Keyboard::isKeyPressed(Keyboard::Escape)) {
						P1_Engine::getPauseMenu()->exit();
						P1_Engine::setCurrentState(MainStates::GAME);
					}
					if (Keyboard::isKeyPressed(Keyboard::Down)) {
						P1_Engine::getPauseMenu()->selectDown();
					}
					if (Keyboard::isKeyPressed(Keyboard::Up)) {
						P1_Engine::getPauseMenu()->selectUp();
					}
					if (Keyboard::isKeyPressed(Keyboard::Return)) {

						P1_Engine::getPauseMenu()->execute();
					}
					break;
				case MainStates::GAME:
					if (Keyboard::isKeyPressed(Keyboard::Escape)) {
						if (!P1_Engine::isInMenu()) {
							P1_Engine::setCurrentState(MainStates::IN_GAME_MENU);
							P1_Engine::getPauseMenu()->open();
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Return)) {

						if (P1_Engine::getVendorMenu()->getIsShowing()) {
							P1_Engine::getVendorMenu()->executerAction();
						}


						if (P1_Engine::getDialogue()->getIsShowing()) {
							if (currentQuest.hasEvent) {
								switch (currentQuest.eventType)
								{
								case EVENT_TYPE::VENTE:
									P1_Engine::getVendorMenu()->loadFromVenteEvent(P1_Engine::getMGR_Events()->getEventVente(currentQuest.eventID));
									P1_Engine::getVendorMenu()->show(true);
									break;
								case EVENT_TYPE::COMBAT:
									break;
								default:
									break;
								}
							}
							if (!P1_Engine::getDialogue()->getHasOptions()) {
								currentQuest = P1_Engine::getMGR_Quetes()->getQuete(currentQuest.targetID);
								P1_Engine::getDialogue()->setDialogueFromQuete(currentQuest);
								if (currentQuest.targetID == "NONE") {
									P1_Engine::getDialogue()->show(false);
									
								}
								P1_Engine::playSound("a_res/Validate_0.wav");
							}
							
							else {
								currentQuest = P1_Engine::getMGR_Quetes()->getQuete(currentQuest.options.at(P1_Engine::getDialogue()->getCurrentOption()).targetID);
								P1_Engine::getDialogue()->setDialogueFromQuete(currentQuest);
								if (currentQuest.targetID == "NONE") {
									P1_Engine::getDialogue()->show(false);
								}
								P1_Engine::playSound("a_res/ValidateOption.wav");
								P1_Engine::getDialogue()->setCurrentOption(0);
							}
							
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Up)) {
						if (P1_Engine::getDialogue()->getIsShowing()) {
							if (P1_Engine::getDialogue()->getHasOptions()) {
								P1_Engine::getDialogue()->selectUp();
							}
						}
						if (P1_Engine::getVendorMenu()->getIsShowing()) {
							P1_Engine::getVendorMenu()->selectUp(P1_Engine::getVendorMenu()->getListToShow());
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Down)) {
						if (P1_Engine::getDialogue()->getIsShowing()) {
							if (P1_Engine::getDialogue()->getHasOptions()) {
								P1_Engine::getDialogue()->selectDown();
							}
						}
						if (P1_Engine::getVendorMenu()->getIsShowing()) {
							P1_Engine::getVendorMenu()->selectDown(P1_Engine::getVendorMenu()->getListToShow());
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Space)) {
						currentQuest = P1_Engine::getMGR_Quetes()->getQuete("Q1000");
						P1_Engine::getDialogue()->setDialogueFromQuete(currentQuest);
						P1_Engine::getDialogue()->show(true);
					}

					break;
				case MainStates::BATTLE:

					break;
				default:
					break;
				}
			}
			else {
				switch (P1_Engine::getCurrentState())
				{
				case MainStates::MAIN_MENU:
					break;
				case MainStates::IN_GAME_MENU:
					break;
				case MainStates::GAME:
						pers.stopMove();
					break;
				case MainStates::BATTLE:
					break;
				default:
					break;
				}
			}
		}
		//-------------------------------------------------------
		//INPUTS
		//-------------------------------------------------------
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			switch (P1_Engine::getCurrentState())
			{
			case MainStates::MAIN_MENU:
				break;
			case MainStates::IN_GAME_MENU:
				break;
			case MainStates::GAME:
				if (!P1_Engine::isInMenu()) {
					pers.moveLeft();
				}
				break;
			case MainStates::BATTLE:
				break;
			default:
				break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			switch (P1_Engine::getCurrentState())
			{
			case MainStates::MAIN_MENU:
				break;
			case MainStates::IN_GAME_MENU:
				break;
			case MainStates::GAME:
				if (!P1_Engine::isInMenu()) {
					pers.moveRight();
				}
				break;
			case MainStates::BATTLE:
				break;
			default:
				break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			switch (P1_Engine::getCurrentState())
			{
			case MainStates::MAIN_MENU:
				break;
			case MainStates::IN_GAME_MENU:
				break;
			case MainStates::GAME:
				if (!P1_Engine::isInMenu()) {
					pers.moveUp();
				}
				break;
			case MainStates::BATTLE:
				break;
			default:
				break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			switch (P1_Engine::getCurrentState())
			{
			case MainStates::MAIN_MENU:
				break;
			case MainStates::IN_GAME_MENU:
				break;
			case MainStates::GAME:
				if (!P1_Engine::isInMenu()) {
					pers.moveDown();
				}
				break;
			case MainStates::BATTLE:
				break;
			default:
				break;
			}
		}
		
		//----------------------------------------------------------

		//CALCULS PRE-RENDUS
		
		while (P1_Engine::getAccumulator() > P1_Engine::getUPS())
		{
			P1_Engine::setAccumulator(P1_Engine::getAccumulator() - P1_Engine::getUPS());
		}

		//RENDU

		P1_Engine::getWindow()->clear(sf::Color::Black);

		switch (P1_Engine::getCurrentState())
		{
		case MainStates::SPLASH:
			splash.update();
			splash.draw();
			break;
		case MainStates::MAIN_MENU:
			menuPrincipal.draw();
			options.draw();
			break;
		case MainStates::IN_GAME_MENU:
			P1_Engine::getPauseMenu()->update();
			P1_Engine::getPauseMenu()->draw();
			break;
		case MainStates::GAME:
			
			P1_Engine::getWindow()->setView(*P1_Engine::getCamera()->getView().get());

			//Update
			P1_Engine::getLayer_1()->update();
			P1_Engine::getLayer_2()->update();
			P1_Engine::getLayer_3()->update();
			pers.update();

			P1_Engine::getCamera()->centerOnActor(pers);
			P1_Engine::getDialogue()->update();
			P1_Engine::getVendorMenu()->update();

			//Layers
			//MAP
			P1_Engine::getLayer_1()->drawToScreen(); //1.
			P1_Engine::getLayer_2()->drawToScreen(); //2.
			pers.draw(); //Personnage.
			P1_Engine::getLayer_3()->drawToScreen(); //3.
			

			//POST-PROCESS
			filter.draw(); //4.
			//HUD
			P1_Engine::getDialogue()->draw(); //5.
			P1_Engine::getVendorMenu()->draw();

			break;
		case MainStates::BATTLE:
			P1_Engine::getBattleUI()->update();
			P1_Engine::getBattleUI()->draw();
			break;
		default:
			break;
		}
		//AFFICHAGE
		P1_Engine::getWindow()->display();

		//RE-INIT
		P1_Engine::setLastDeltaCamMove_X(0.0f);
		P1_Engine::setLastDeltaCamMove_Y(0.0f);
		P1_Engine::setAccumulator(P1_Engine::getAccumulator() + P1_Engine::getMainClock().restart());
	}
	P1_Engine::getWindow()->close();

	return 0;
}