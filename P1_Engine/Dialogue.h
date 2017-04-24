#pragma once
#include "SFML\Graphics.hpp"
#include <string>
#include <memory>
#include "P1_Engine.h"
#include "Camera.h"

class Dialogue
{
public:
	/*
		*hasName
		*name
		*sizeBoxY : hauteur de la boite en pixels
		*outlineSize : epaisseur de la bordure en pixel
	*/
	Dialogue();
	~Dialogue();

	void initCadre();
	void initBackground();
	void initOptions();

	//Reset la position du texte
	void resetTextPosition(); 

	//Mutateurs de la boite
	void setPosition(sf::Vector2f pos);
	void setOutlineColor(sf::Color col);
	void setFillColor(sf::Color col);
	void setSize(sf::Vector2f size);

	//Mutateurs du texte
	void setText(std::string str);
	void setCharacterSize(int size);
	int getCharacterSize();
	void setTextColor(sf::Color col);
	void setTextRelativePosition(sf::Vector2f pos);
	void resizeText();
	void setName(std::string name);
	void setBackgroundGradient(sf::Color a, sf::Color b);

	void recadre();
	void recadreName();
	void recadreOptions();

	void setDialogueFromQuete(P1_Engine::Quete& quete);
	void initText(std::shared_ptr<sf::Text>& text);

	void showName(bool arg);
	void showOptions(bool arg);
	void show(bool arg);
	bool getIsShowing();
	bool getHasOptions();

	void selectUp();
	void selectDown();
	void deselectOption();
	void reloadOptionSelected();
	int getCurrentOption();
	int getMaxOptionSizeX();
	void setCurrentOption(int arg);

	void update();

	//Genere le background de la boite (boite du nom non supportée)
	sf::Image getBackground(sf::Color a, sf::Color b);

	//Retourne le nombre de caracteres affichable sur une ligne (imprécis)
	int maxCharInARow();

	//Initialise par defaut les composants principaux de la boite de nom
	void loadNameText();

	//Affiche la boite, la boite de nom et le texte
	void draw();

private:
	std::shared_ptr<sf::RectangleShape> g_cadre;
	std::shared_ptr<sf::RectangleShape> g_name;
	std::shared_ptr<sf::RectangleShape> g_options;
	std::vector<std::shared_ptr<sf::Text>> m_options;
	std::shared_ptr<sf::Text> m_text;
	std::shared_ptr<sf::Text> m_name;
	std::shared_ptr<sf::Sprite> m_back;
	std::shared_ptr<sf::Texture> m_bTx;
	sf::Color m_headColor;

	float m_outlineSize;
	int m_sizeBoxY;
	float m_textOffsetX;
	bool m_hasName;
	bool isShowing;
	bool m_hasOptions;
	int m_currentOption;
};

