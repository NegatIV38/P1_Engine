#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <iostream>
#include <memory>

class Layer;
class MainMenu;
class Camera;
class Dialogue;
class VendorMenu;
class Personnage;

class QuestsMgr;
class EventsMgr;
class Objects_Mgr;

class BattleUI;
class PauseMenu;

//Enums

enum class Direction { //Directions (pour les mouvements des Characters)
	NONE, NORTH, SOUTH, EAST, WEST
};
enum class Shadow_Def {
	LONG_LEFT_VERT_1, LONG_LEFT_VERT_2, LONG_LEFT_VERT_3, LONG_LEFT_VERT_4,
	LONG_RIGHT_VERT_1, LONG_RIGHT_VERT_2, LONG_RIGHT_VERT_3, LONG_RIGHT_VERT_4,
	UP_LEFT_VERT_1, UP_LEFT_VERT_2, UP_LEFT_VERT_3, UP_LEFT_VERT_4,
	UP_RIGHT_VERT_1, UP_RIGHT_VERT_2, UP_RIGHT_VERT_3, UP_RIGHT_VERT_4,
	DOWN_LEFT_VERT_1, DOWN_LEFT_VERT_2, DOWN_LEFT_VERT_3, DOWN_LEFT_VERT_4,
	DOWN_RIGHT_VERT_1, DOWN_RIGHT_VERT_2, DOWN_RIGHT_VERT_3, DOWN_RIGHT_VERT_4,

	LONG_UP_HORIZ_1, LONG_UP_HORIZ_2, LONG_UP_HORIZ_3, LONG_UP_HORIZ_4,
	LONG_DOWN_HORIZ_1, LONG_DOWN_HORIZ_2, LONG_DOWN_HORIZ_3, LONG_DOWN_HORIZ_4,
	UP_LEFT_HORIZ_1, UP_LEFT_HORIZ_2, UP_LEFT_HORIZ_3, UP_LEFT_HORIZ_4,
	UP_RIGHT_HORIZ_1, UP_RIGHT_HORIZ_2, UP_RIGHT_HORIZ_3, UP_RIGHT_HORIZ_4,
	DOWN_LEFT_HORIZ_1, DOWN_LEFT_HORIZ_2, DOWN_LEFT_HORIZ_3, DOWN_LEFT_HORIZ_4,
	DOWN_RIGHT_HORIZ_1, DOWN_RIGHT_HORIZ_2, DOWN_RIGHT_HORIZ_3, DOWN_RIGHT_HORIZ_4,
	FULL
};
enum class MainStates {
	MAIN_MENU, IN_GAME_MENU, GAME, BATTLE, SPLASH
};
enum class COMMANDS {
	BEGIN_GAME, QUIT_GAME, UNKNOW, ENTER_OPTIONS, OPTIONS_BACK, BEGIN_BATTLE
};
enum class FILE_TYPES {
	LAYER_1, LAYER_2, LAYER_3, TILESET
};
enum class SPRITE_STYLE {
	FACE,RIGHT,LEFT,BACK
};

enum class EVENT_TYPE {
	VENTE, COMBAT
};

enum class OBJECT_TYPE {
	CONSOMMABLE
};


class P1_Engine
{
public:
	P1_Engine();
	~P1_Engine();

	//Structures-----------------------------------------------------

	struct Animation
	{
		std::shared_ptr<sf::Sprite> sp; //Sprite
		int animSize; //Nb Tiles dans le sprite
		int spSize; //taille en px du sprite
		int currentAnim; //id de la Tile courante [0;animSize-1]
		SPRITE_STYLE currentStyle;
		sf::Clock internalClock;
		bool isOn;
	};

	struct Quest_option {
		std::string targetID;
		std::string text;
	};

	struct Quete {
		std::string questID;
		bool hasOption;
		bool hasName;
		bool hasEvent;
		std::string name;
		std::string targetID;
		std::string text;
		std::vector<Quest_option> options;
		EVENT_TYPE eventType;
		std::string eventID;
	};

	struct Event_Objet{
		std::string objectID;
		std::string objectName;
		int prix;
		std::string description;
	};

	struct Event_Vente {
		std::string eventID;
		std::string pnjName;
		std::vector<Event_Objet> objets;
	};

	struct Event_Combat {
		std::string eventID;
	};

	struct Objet_ID {
		std::string objectID;
		OBJECT_TYPE objectType;
		std::string objectName;
	};

	//Fonctions-----------------------------------------------------

	static void printLog(std::string arg);

	template<class T>
	static void printDebug(T arg);

	template <class T, class ...Args>
	static void printDebug(T arg, Args ... args);

	//Récupère le facteur de zoom {X,Y}
	static sf::Vector2f getSpriteScale();
	static sf::Vector2f getSpriteScale(int spriteSize);

	//Récupère le facteur de zoom des Sprites en X
	static float getSpriteScaleX(); 
	static float getSpriteScaleX(int spriteSize);

	//Récupère le facteur de zoom des Sprites en Y
	static float getSpriteScaleY(); 
	static float getSpriteScaleY(int spriteSize);

	//Récupère la position de sp sur la grille
	static sf::Vector2i getSpritePositionOnGrid(sf::Sprite& sp); 

	//Retourne vrai si sp est sur la grille, faux sinon
	static bool isPositionMatchGrid(sf::Sprite& sp, float precision = 0.0f); 

	//Retourne la distance entre a et b
	static float distance(sf::Vector2f a, sf::Vector2f b); 

	//Découpe une sprite dans une texture aux coordonnées (x,y)
	static sf::Sprite getSprite(sf::Texture& tx, int x, int y); 

	//Récupère la Texture d'un sprite
	static sf::Texture getTextureFromSprite(sf::Sprite& sp); 

	static void cleanTexture(sf::Texture& tx, int ptX = 0, int ptY = 0);

	static sf::Vector2i posToGridPos(sf::Vector2f pos);

	static int getCameraUnitMove();
	static std::string getFileNameMap(std::string mapName, FILE_TYPES type);
	static std::string getStringBetween(std::string text, std::string before, std::string after);

	static bool isInMenu();

	//Procedures-----------------------------------------------------


	//Place le Sprite sp à la position X,Y sur la grille
	static void setSpritePositionOnGrid(sf::Sprite& sp, int x, int y); 

	//Change le sprite à afficher dans une Animation, speed en millisecondes, si speed est petit, l'anim est rapide
	static void setCurrentSpriteAnimationTile(P1_Engine::Animation& anim, int speed); 

	//Initialise les membres de l'Animation
	static void initAnimation(P1_Engine::Animation& anim, int animSize); 

	//Charge les textures d'ombres
	static void loadShadowsFromFile(std::string file, int longueurX = 13, int hauteurY = 4); 

	//Affiche l'ombre shadow à la position (x,y)
	static void drawShadow(Shadow_Def shadow, int x, int y, sf::RenderWindow& win); 

	//Initialise le composant font
	static void initFont(); 

	static void initWindow();

	static void loadMap(std::string mapName);

	static void startAnimation(P1_Engine::Animation& anim);
	static void stopAnimation(P1_Engine::Animation& anim);

	static void loadSoundBuffer(std::string name);
	static void playSound(std::string name);


	//Accesseurs-----------------------------------------------------


	static const int getNativeResolutionX();
	static const int getNativeResolutionY();
	static const int getGridSizeX();
	static const int getGridSizeY();
	static const int getSpriteSizeUnit();
	static const std::string getFontFileName();
	static const std::string getMapDirectory();
	static const std::string getGraphicalsRessourcesDirectory();


	static const int getLastDeltaCamMove_X();
	static const int getLastDeltaCamMove_Y();
	static const MainStates getCurrentState();
	static const bool getIsGameRunning();


	static sf::Font* getFont();
	static sf::Texture getShadows_tx();
	static const sf::Time getAccumulator();
	static const sf::Time getUPS();
	static sf::Clock getMainClock();
	static std::shared_ptr<sf::RenderWindow> getWindow();
	static std::shared_ptr<Camera> getCamera();

	static std::shared_ptr<Layer> getLayer_1();
	static std::shared_ptr<Layer> getLayer_2();
	static std::shared_ptr<Layer> getLayer_3();

	static std::shared_ptr<Dialogue> getDialogue();
	static std::shared_ptr<VendorMenu> getVendorMenu();
	static std::shared_ptr<BattleUI> getBattleUI();
	static std::shared_ptr<PauseMenu> getPauseMenu();

	static Personnage* getPersonnage();

	static std::shared_ptr<QuestsMgr> getMGR_Quetes();
	static std::shared_ptr<EventsMgr> getMGR_Events();
	static std::shared_ptr<Objects_Mgr> getMGR_Objects();

	//Mutateurs-----------------------------------------------------


	static void setLastDeltaCamMove_X(int arg);
	static void setLastDeltaCamMove_Y(int arg);
	static void setCurrentState(MainStates arg);
	static void setIsGameRunning(bool arg);

	static void setAccumulator(sf::Time arg);

	static void setPersonnage(Personnage* pers);

	//Commandes-----------------------------------------------------


	static void execute(COMMANDS cmd, std::string param = "");
	static COMMANDS strToCmd(std::string cmd);
	static void beginGame();
	static void quitGame();
	static void enterOptions(std::string optionMenuName);
	static void optionsBack(std::string mainMenuName);
	static void afficherMenu(std::string menuName);
	static void beginBattle();

	//Acces Lists-----------------------------------------------------


	static std::map<std::string, std::shared_ptr<MainMenu>>* getMenus_List();


private:
	//Constants-----------------------------------------------------

	//Resolution en X par défaut (px)
	static int iNativeResolutionX;

	//Resolution en Y par défaut (px)
	static int iNativeResolutionY;

	//Nombre de Sprites affichables dans la fenetre en X (int)
	static int iGridSizeX;

	//Nombre de Sprites affichables dans la fenetre en Y (int)
	static int iGridSizeY;

	//Taille des Sprites par défaut (px)
	static int iSpriteSizeUnit;
	
	static std::string sFontFileName; //Nom du fichier font
	static std::string sMapDirectory;
	static std::string sGraphicalsRessourcesDirectory;

	//Variables-----------------------------------------------------

	//Variable centralisée du deplacement de la camera
	static int fLastDeltaCamMove_X;


	static int fLastDeltaCamMove_Y;
	static MainStates currentState;
	static bool isGameRunning;
	
	//Objets de Rendu-----------------------------------------------------


	static sf::Font t_font;
	static sf::Texture shadows_tx;
	static sf::Time accumulator;
	static sf::Time ups;
	static sf::Clock mainClock;
	static std::shared_ptr<sf::RenderWindow> window;
	static std::shared_ptr<Camera> cam;

	static std::shared_ptr<Layer> ground;
	static std::shared_ptr<Layer> surface;
	static std::shared_ptr<Layer> sky;

	static std::shared_ptr<Dialogue> ui_dialogue;
	static std::shared_ptr<VendorMenu> ui_vendorMenu;
	static std::shared_ptr<BattleUI> ui_battle;
	static std::shared_ptr<PauseMenu> ui_pauseMenu;

	static Personnage* personnage;

	//Listes et Tableaux-----------------------------------------------------


	static std::map <Shadow_Def, sf::Sprite > shadows_list;
	static std::map<std::string, sf::SoundBuffer> loadedSoundBuffers;
	static std::map<std::string, sf::Sound> playableSounds;
	static std::map<std::string, std::shared_ptr<MainMenu>> menus;


	//Managers--------------------------------------------------------------

	static std::shared_ptr<QuestsMgr> mgr_quetes;
	static std::shared_ptr<EventsMgr> mgr_events;
	static std::shared_ptr<Objects_Mgr> mgr_objects;


};

//END OF CLASS

template<class T>
inline void P1_Engine::printDebug(T arg)
{
	std::cout << arg << std::endl;
}

template<class T, class ...Args>
inline void P1_Engine::printDebug(T arg, Args ...args)
{
	std::cout << arg << "\t";
	printDebug(args...);
}
