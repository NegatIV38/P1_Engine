#include "P1_Engine.h"
#include "MainMenu.h"
#include "Camera.h"
#include "Layer.h"
#include "Dialogue.h"
#include "VendorMenu.h"
#include "Personnage.h"
#include "EventsMgr.h"
#include "Objects_Mgr.h"
#include "QuestsMgr.h"
#include "BattleUI.h"
#include "PauseMenu.h"

//------------INITIALISATION----------------------------------------
int P1_Engine::iNativeResolutionX = 1280;
int P1_Engine::iNativeResolutionY = 720;
int P1_Engine::iGridSizeX = 20;
int P1_Engine::iGridSizeY = 11;
int P1_Engine::iSpriteSizeUnit = 32;
std::string P1_Engine::sFontFileName = "UASQUARE.TTF";
std::string	P1_Engine::sMapDirectory = "maps/";
std::string P1_Engine::sGraphicalsRessourcesDirectory = "g_res/";


int P1_Engine::fLastDeltaCamMove_X = 0;
int P1_Engine::fLastDeltaCamMove_Y = 0;
MainStates P1_Engine::currentState = MainStates::SPLASH;
bool P1_Engine::isGameRunning = true;

sf::Font P1_Engine::t_font;
sf::Texture P1_Engine::shadows_tx;
sf::Time P1_Engine::accumulator = sf::Time::Zero;
sf::Time P1_Engine::ups = sf::seconds(1.f / 60.f);
sf::Clock P1_Engine::mainClock;
std::shared_ptr<sf::RenderWindow> P1_Engine::window;
std::shared_ptr<Camera> P1_Engine::cam = std::make_shared<Camera>();

std::shared_ptr<Layer> P1_Engine::ground = std::make_shared<Layer>(0, "map50");
std::shared_ptr<Layer> P1_Engine::surface = std::make_shared<Layer>(1, "map50");
std::shared_ptr<Layer> P1_Engine::sky = std::make_shared<Layer>(2, "map50");

std::shared_ptr<Dialogue> P1_Engine::ui_dialogue = std::make_shared<Dialogue>();
std::shared_ptr<VendorMenu> P1_Engine::ui_vendorMenu = std::make_shared<VendorMenu>();
std::shared_ptr<BattleUI> P1_Engine::ui_battle = std::make_shared<BattleUI>();
std::shared_ptr<PauseMenu> P1_Engine::ui_pauseMenu = std::make_shared<PauseMenu>();

Personnage* P1_Engine::personnage;

std::map<Shadow_Def, sf::Sprite > P1_Engine::shadows_list;
std::map<std::string, sf::SoundBuffer> P1_Engine::loadedSoundBuffers;
std::map<std::string, sf::Sound> P1_Engine::playableSounds;
std::map < std::string, std::shared_ptr<MainMenu>> P1_Engine::menus;

std::shared_ptr<QuestsMgr> P1_Engine::mgr_quetes = std::make_shared<QuestsMgr>("data/quests.xml");
std::shared_ptr<EventsMgr> P1_Engine::mgr_events = std::make_shared<EventsMgr>("data/events.xml");
std::shared_ptr<Objects_Mgr> P1_Engine::mgr_objects = std::make_shared<Objects_Mgr>("data/objects.xml");

//------------------------------------------------------------------

P1_Engine::P1_Engine()
{
}


P1_Engine::~P1_Engine()
{
}

float P1_Engine::getSpriteScaleX() {
	return (iNativeResolutionX / iGridSizeX) / iSpriteSizeUnit;
}

float P1_Engine::getSpriteScaleX(int spriteSize)
{
	return (iNativeResolutionX / iGridSizeX) / spriteSize;
}

float P1_Engine::getSpriteScaleY()
{
	return (iNativeResolutionY / iGridSizeY) / iSpriteSizeUnit;
}

float P1_Engine::getSpriteScaleY(int spriteSize)
{
	return (iNativeResolutionY / iGridSizeY) / spriteSize;
}

sf::Vector2i P1_Engine::getSpritePositionOnGrid(sf::Sprite & sp)
{
	sf::Vector2f pos = sp.getPosition();
	sf::Vector2i gPos;
	gPos.x = pos.x / (iSpriteSizeUnit*P1_Engine::getSpriteScaleX());
	gPos.y = pos.y / (iSpriteSizeUnit*P1_Engine::getSpriteScaleY());
	return gPos;
}

bool P1_Engine::isPositionMatchGrid(sf::Sprite & sp, float precision)
{
	sf::Vector2f pos = sp.getPosition();
	pos.x /= (P1_Engine::iSpriteSizeUnit*P1_Engine::getSpriteScaleX());
	pos.y /= (P1_Engine::iSpriteSizeUnit*P1_Engine::getSpriteScaleY());
	sf::Vector2f gPos = sf::Vector2f(P1_Engine::getSpritePositionOnGrid(sp));
	if (distance(pos, gPos) < precision) {
		return true;
	}
	return false;
}

float P1_Engine::distance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

sf::Sprite P1_Engine::getSprite(sf::Texture & tx, int x, int y)
{
	sf::Sprite sp;
	sp.setTexture(tx);
	sp.setTextureRect(sf::IntRect(x*P1_Engine::getSpriteSizeUnit(), y*P1_Engine::getSpriteSizeUnit(), P1_Engine::getSpriteSizeUnit(), P1_Engine::getSpriteSizeUnit()));
	return sp;
}

sf::Texture P1_Engine::getTextureFromSprite(sf::Sprite & sp)
{
	return *sp.getTexture();
}

void P1_Engine::cleanTexture(sf::Texture & tx, int ptX, int ptY)
{
	
	sf::Image temp = tx.copyToImage();
	sf::Color ctemp = temp.getPixel(ptX, ptY);
	for (int i = 0; i < temp.getSize().x; i++) {
		for (int j = 0; j < temp.getSize().y; j++) {
			if (temp.getPixel(i, j) == ctemp) {
					temp.setPixel(i, j, sf::Color::Transparent);
			}
		}
	}
	tx.loadFromImage(temp);
}

sf::Vector2i P1_Engine::posToGridPos(sf::Vector2f pos)
{
	return sf::Vector2i(pos.x / P1_Engine::getSpriteSizeUnit() / P1_Engine::getSpriteScaleX(),
		pos.y/P1_Engine::getSpriteSizeUnit() / P1_Engine::getSpriteScaleY());
}

int P1_Engine::getCameraUnitMove()
{
	return P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX();
}

std::string P1_Engine::getFileNameMap(std::string mapName, FILE_TYPES type)
{
	switch (type)
	{
	case FILE_TYPES::LAYER_1:
		return P1_Engine::getMapDirectory() + mapName + "_1.pmap";
		break;
	case FILE_TYPES::LAYER_2:
		return P1_Engine::getMapDirectory() + mapName + "_2.pmap";
		break;
	case FILE_TYPES::LAYER_3:
		return P1_Engine::getMapDirectory() + mapName + "_3.pmap";
		break;
	case FILE_TYPES::TILESET:
		return P1_Engine::getGraphicalsRessourcesDirectory() + mapName + "_ts.png";
		break;
	default:
		return "";
		break;
	}
}

std::string P1_Engine::getStringBetween(std::string text, std::string before, std::string after)
{
	return text.substr(text.find(before) + std::string(before).size(), text.find(after) - (text.find(before) + std::string(before).size()));
}

bool P1_Engine::isInMenu()
{
	return (P1_Engine::getDialogue()->getIsShowing() || P1_Engine::getVendorMenu()->getIsShowing());
}

void P1_Engine::setSpritePositionOnGrid(sf::Sprite & sp, int x, int y)
{

	sp.setPosition(x*(iSpriteSizeUnit*P1_Engine::getSpriteScaleX()), y*(iSpriteSizeUnit*P1_Engine::getSpriteScaleY()));

}

void P1_Engine::setCurrentSpriteAnimationTile(P1_Engine::Animation & anim, int speed)
{
	//if(res::rdu::totalTime.asMilliseconds() % speed == 0){

	if (anim.internalClock.getElapsedTime().asMilliseconds() >= int((1.0f / speed)*1000.f) && anim.isOn) {
		anim.internalClock.restart();
		anim.sp->setTextureRect(sf::IntRect(anim.currentAnim*P1_Engine::iSpriteSizeUnit, int(anim.currentStyle)*P1_Engine::iSpriteSizeUnit, P1_Engine::iSpriteSizeUnit, P1_Engine::iSpriteSizeUnit));
		anim.currentAnim++;
		if (anim.currentAnim >= anim.animSize) {
			anim.currentAnim = 0;
		}
	}
	else if (!anim.isOn) {
		anim.currentAnim = 0;
		anim.sp->setTextureRect(sf::IntRect(anim.currentAnim*P1_Engine::iSpriteSizeUnit, int(anim.currentStyle)*P1_Engine::iSpriteSizeUnit, P1_Engine::iSpriteSizeUnit, P1_Engine::iSpriteSizeUnit));

	}
}

void P1_Engine::initAnimation(P1_Engine::Animation & anim, int animSize)
{
	anim.sp = std::make_shared<sf::Sprite>();
	anim.animSize = animSize;
	anim.spSize = animSize * P1_Engine::iSpriteSizeUnit;
	anim.currentAnim = 0;
	anim.currentStyle = SPRITE_STYLE::FACE;
	anim.isOn = false;
	anim.internalClock.restart();
}

void P1_Engine::loadShadowsFromFile(std::string file, int longueurX, int hauteurY)
{
	if (!P1_Engine::getShadows_tx().loadFromFile(file)) {
		std::cerr << "Couldn't load texture file" << std::endl;
	}
	P1_Engine::shadows_list[Shadow_Def::LONG_LEFT_VERT_1] = getSprite(shadows_tx, 0, 0);
	P1_Engine::shadows_list[Shadow_Def::LONG_RIGHT_VERT_1] = getSprite(shadows_tx, 0, 1);
	P1_Engine::shadows_list[Shadow_Def::LONG_UP_HORIZ_1] = getSprite(shadows_tx, 0, 2);
	P1_Engine::shadows_list[Shadow_Def::LONG_DOWN_HORIZ_1] = getSprite(shadows_tx, 0, 3);

	P1_Engine::shadows_list[Shadow_Def::LONG_LEFT_VERT_2] = getSprite(shadows_tx, 1, 0);
	P1_Engine::shadows_list[Shadow_Def::LONG_RIGHT_VERT_2] = getSprite(shadows_tx, 1, 1);
	P1_Engine::shadows_list[Shadow_Def::LONG_UP_HORIZ_2] = getSprite(shadows_tx, 1, 2);
	P1_Engine::shadows_list[Shadow_Def::LONG_DOWN_HORIZ_2] = getSprite(shadows_tx, 1, 3);

	P1_Engine::shadows_list[Shadow_Def::LONG_LEFT_VERT_3] = getSprite(shadows_tx, 2, 0);
	P1_Engine::shadows_list[Shadow_Def::LONG_RIGHT_VERT_3] = getSprite(shadows_tx, 2, 1);
	P1_Engine::shadows_list[Shadow_Def::LONG_UP_HORIZ_3] = getSprite(shadows_tx, 2, 2);
	P1_Engine::shadows_list[::Shadow_Def::LONG_DOWN_HORIZ_3] = getSprite(shadows_tx, 2, 3);

	P1_Engine::shadows_list[::Shadow_Def::LONG_LEFT_VERT_4] = getSprite(shadows_tx, 3, 0);
	P1_Engine::shadows_list[::Shadow_Def::LONG_RIGHT_VERT_4] = getSprite(shadows_tx, 3, 1);
	P1_Engine::shadows_list[Shadow_Def::LONG_UP_HORIZ_4] = getSprite(shadows_tx, 3, 2);
	P1_Engine::shadows_list[Shadow_Def::LONG_DOWN_HORIZ_4] = getSprite(shadows_tx, 3, 3);

	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_VERT_1] = getSprite(shadows_tx, 4, 0);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_VERT_1] = getSprite(shadows_tx, 4, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_HORIZ_1] = getSprite(shadows_tx, 4, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_HORIZ_1] = getSprite(shadows_tx, 4, 3);

	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_VERT_2] = getSprite(shadows_tx, 5, 0);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_VERT_2] = getSprite(shadows_tx, 5, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_HORIZ_2] = getSprite(shadows_tx, 5, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_HORIZ_2] = getSprite(shadows_tx, 5, 3);

	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_VERT_3] = getSprite(shadows_tx, 6, 0);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_VERT_3] = getSprite(shadows_tx, 6, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_HORIZ_3] = getSprite(shadows_tx, 6, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_HORIZ_3] = getSprite(shadows_tx, 6, 3);

	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_VERT_4] = getSprite(shadows_tx, 7, 0);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_VERT_4] = getSprite(shadows_tx, 7, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_RIGHT_HORIZ_4] = getSprite(shadows_tx, 7, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_HORIZ_4] = getSprite(shadows_tx, 7, 3);

	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_VERT_1] = getSprite(shadows_tx, 8, 0);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_VERT_1] = getSprite(shadows_tx, 8, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_HORIZ_1] = getSprite(shadows_tx, 8, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_HORIZ_1] = getSprite(shadows_tx, 8, 3);

	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_VERT_2] = getSprite(shadows_tx, 9, 0);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_VERT_2] = getSprite(shadows_tx, 9, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_HORIZ_2] = getSprite(shadows_tx, 9, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_HORIZ_2] = getSprite(shadows_tx, 9, 3);

	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_VERT_3] = getSprite(shadows_tx, 10, 0);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_VERT_3] = getSprite(shadows_tx, 10, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_HORIZ_3] = getSprite(shadows_tx, 10, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_HORIZ_3] = getSprite(shadows_tx, 10, 3);

	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_VERT_4] = getSprite(shadows_tx, 11, 0);
	P1_Engine::shadows_list[Shadow_Def::DOWN_RIGHT_VERT_4] = getSprite(shadows_tx, 11, 1);
	P1_Engine::shadows_list[Shadow_Def::UP_LEFT_HORIZ_4] = getSprite(shadows_tx, 11, 2);
	P1_Engine::shadows_list[Shadow_Def::DOWN_LEFT_HORIZ_4] = getSprite(shadows_tx, 11, 3);

	P1_Engine::shadows_list[Shadow_Def::FULL] = getSprite(shadows_tx, 12, 0);

	for (int i = 0; i < int(Shadow_Def::FULL); i++) {
		P1_Engine::shadows_list[Shadow_Def(i)].setScale(P1_Engine::getSpriteScale());
	}

}

void P1_Engine::drawShadow(Shadow_Def shadow, int x, int y, sf::RenderWindow & win)
{
	sf::Sprite sp = P1_Engine::shadows_list[shadow];
	P1_Engine::setSpritePositionOnGrid(sp, x, y);
	sp.setScale(P1_Engine::getSpriteScale());
	win.draw(sp);
}

void P1_Engine::initFont()
{
	if (!t_font.loadFromFile(P1_Engine::getFontFileName())) {
		std::cerr << "Couldn't load font" << std::endl;
	}
}

void P1_Engine::initWindow()
{
	P1_Engine::window = std::make_shared<sf::RenderWindow>(sf::VideoMode(P1_Engine::getNativeResolutionX(), P1_Engine::getNativeResolutionY()), "SFML works!", sf::Style::Default);
}

void P1_Engine::loadMap(std::string mapName)
{
	P1_Engine::getLayer_1()->loadMap(mapName);
	P1_Engine::getLayer_2()->loadMap(mapName);
	P1_Engine::getLayer_3()->loadMap(mapName);
}

void P1_Engine::startAnimation(P1_Engine::Animation & anim)
{
	if (!anim.isOn) {
		anim.currentAnim = 1;
		anim.sp->setTextureRect(sf::IntRect(anim.currentAnim*P1_Engine::iSpriteSizeUnit, 0, P1_Engine::iSpriteSizeUnit, P1_Engine::iSpriteSizeUnit));
	}
	anim.isOn = true;
	
}

void P1_Engine::stopAnimation(P1_Engine::Animation & anim)
{
	anim.isOn = false;
}

void P1_Engine::loadSoundBuffer(std::string name)
{
	P1_Engine::loadedSoundBuffers.emplace(name, sf::SoundBuffer());
	if (!P1_Engine::loadedSoundBuffers.at(name).loadFromFile(name)) {
		std::cerr << "Erreur lors de l'ouvertur du fichier .wav" << std::endl;
	}
	else {
		P1_Engine::playableSounds.emplace(name, sf::Sound());
		P1_Engine::playableSounds.at(name).setBuffer(P1_Engine::loadedSoundBuffers.at(name));
	}
}

void P1_Engine::playSound(std::string name)
{
	P1_Engine::playableSounds.at(name).play();
}

const int P1_Engine::getNativeResolutionX()
{
	return iNativeResolutionX;
}

const int P1_Engine::getNativeResolutionY()
{
	return iNativeResolutionY;
}

const int P1_Engine::getGridSizeX()
{
	return iGridSizeX;
}

const int P1_Engine::getGridSizeY()
{
	return iGridSizeY;
}

const int P1_Engine::getSpriteSizeUnit()
{
	return iSpriteSizeUnit;
}

const std::string P1_Engine::getFontFileName()
{
	return sFontFileName;
}

const std::string P1_Engine::getMapDirectory()
{
	return sMapDirectory;
}

const std::string P1_Engine::getGraphicalsRessourcesDirectory()
{
	return sGraphicalsRessourcesDirectory;
}

const int P1_Engine::getLastDeltaCamMove_X()
{
	return fLastDeltaCamMove_X;
}

const int P1_Engine::getLastDeltaCamMove_Y()
{
	return fLastDeltaCamMove_Y;
}

const MainStates P1_Engine::getCurrentState()
{
	return currentState;
}

const bool P1_Engine::getIsGameRunning()
{
	return isGameRunning;
}

sf::Font* P1_Engine::getFont()
{
	return &t_font;
}

sf::Texture P1_Engine::getShadows_tx()
{
	return shadows_tx;
}

const sf::Time P1_Engine::getAccumulator()
{
	return accumulator;
}

const sf::Time P1_Engine::getUPS()
{
	return ups;
}

sf::Clock P1_Engine::getMainClock()
{
	return mainClock;
}

std::shared_ptr<sf::RenderWindow> P1_Engine::getWindow()
{
	return window;
}

std::shared_ptr<Camera> P1_Engine::getCamera()
{
	return cam;
}

std::shared_ptr<Layer> P1_Engine::getLayer_1()
{
	return ground;
}

std::shared_ptr<Layer> P1_Engine::getLayer_2()
{
	return surface;
}

std::shared_ptr<Layer> P1_Engine::getLayer_3()
{
	return sky;
}

std::shared_ptr<Dialogue> P1_Engine::getDialogue()
{
	return ui_dialogue;
}

std::shared_ptr<VendorMenu> P1_Engine::getVendorMenu()
{
	return ui_vendorMenu;
}

std::shared_ptr<BattleUI> P1_Engine::getBattleUI()
{
	return ui_battle;
}

std::shared_ptr<PauseMenu> P1_Engine::getPauseMenu()
{
	return ui_pauseMenu;
}

Personnage * P1_Engine::getPersonnage()
{
	return personnage;
}

std::shared_ptr<QuestsMgr> P1_Engine::getMGR_Quetes()
{
	return mgr_quetes;
}

std::shared_ptr<EventsMgr> P1_Engine::getMGR_Events()
{
	return mgr_events;
}

std::shared_ptr<Objects_Mgr> P1_Engine::getMGR_Objects()
{
	return mgr_objects;
}

void P1_Engine::setLastDeltaCamMove_X(int arg)
{
	fLastDeltaCamMove_X = arg;
}

void P1_Engine::setLastDeltaCamMove_Y(int arg)
{
	fLastDeltaCamMove_Y = arg;
}

void P1_Engine::setCurrentState(MainStates arg)
{
	currentState = arg;
}

void P1_Engine::setIsGameRunning(bool arg)
{
	isGameRunning = arg;
}

void P1_Engine::setAccumulator(sf::Time arg)
{
	accumulator = arg;
}

void P1_Engine::setPersonnage(Personnage * pers)
{
	personnage = pers;
}

void P1_Engine::execute(COMMANDS cmd, std::string param)
{
	switch (cmd)
	{
	case COMMANDS::BEGIN_GAME:
		P1_Engine::beginGame();
		break;
	case COMMANDS::ENTER_OPTIONS:
		P1_Engine::enterOptions(param);
		break;
	case COMMANDS::OPTIONS_BACK:
		P1_Engine::optionsBack(param);
		break;
	case COMMANDS::QUIT_GAME:
		P1_Engine::quitGame();
		break;
	case COMMANDS::BEGIN_BATTLE:
		P1_Engine::beginBattle();
		break;
	default:
		break;
	}
}

COMMANDS P1_Engine::strToCmd(std::string cmd)
{
	if (cmd == "BEGIN_GAME") {
		return COMMANDS::BEGIN_GAME;
	}
	else if (cmd == "QUIT_GAME") {
		return COMMANDS::QUIT_GAME;
	}
	else if (cmd == "ENTER_OPTIONS") {
		return COMMANDS::ENTER_OPTIONS;
	}
	else if (cmd == "OPTIONS_BACK") {
		return COMMANDS::OPTIONS_BACK;
	}
	else if (cmd == "BEGIN_BATTLE") {
		return COMMANDS::BEGIN_BATTLE;
	}
	else {
		return COMMANDS::UNKNOW;
	}
}

void P1_Engine::beginGame()
{
	P1_Engine::setCurrentState(MainStates::GAME);
}

void P1_Engine::quitGame()
{
	P1_Engine::setIsGameRunning(false);
}

void P1_Engine::enterOptions(std::string optionMenuName)
{
	P1_Engine::afficherMenu(optionMenuName);
}

void P1_Engine::optionsBack(std::string mainMenuName)
{
	P1_Engine::afficherMenu(mainMenuName);
}

void P1_Engine::afficherMenu(std::string menuName)
{
	for (std::map<std::string, std::shared_ptr<MainMenu>>::iterator it = P1_Engine::menus.begin();
		it != P1_Engine::menus.end(); it++) {
		if (it->second->isActive()) {
			it->second->setActive(false);
		}
	}
	P1_Engine::menus.at(menuName)->setActive(true);
}

void P1_Engine::beginBattle()
{
	P1_Engine::setCurrentState(MainStates::BATTLE);
}

std::map<std::string, std::shared_ptr<MainMenu>>* P1_Engine::getMenus_List()
{
	return &menus;
}

void P1_Engine::printLog(std::string arg)
{
	std::cout << arg << std::endl;
}

sf::Vector2f P1_Engine::getSpriteScale()
{
	return sf::Vector2f(getSpriteScaleX(),getSpriteScaleY());
}

sf::Vector2f P1_Engine::getSpriteScale(int spriteSize)
{
	return sf::Vector2f(getSpriteScaleX(spriteSize),getSpriteScaleY(spriteSize));
}
