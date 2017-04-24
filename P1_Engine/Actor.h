#pragma once
#include "SFML/Graphics.hpp"
#include "P1_Engine.h"
#include <memory>

class Actor
{
public:
	Actor();
	Actor(std::string txFile); //Constructeur d'Actor statique
	Actor(std::string txFile, int animLenght, int animSpeed); //Constructeur d'Actor animé
	~Actor();

	void setPosition(int x, int y); //Change la position de l'Actor selon la grille
	sf::Vector2i getPositionInPixels();
	void draw(); //Dessine l'Actor, ne l'affiche pas
	void update(); //Recharge l'Actor à chaque frame
	bool getIsAnimated();
	bool getIsActive();
	std::shared_ptr<P1_Engine::Animation> getAnimation();
	void setActive(bool arg);
	/*Retourne un pointeur sur	:
	 * Le sprite de l'Actor si il est statique
	 * Le sprite courant de l'animation de l'Actor si il est animé */
	std::shared_ptr<sf::Sprite> getSprite();
	void executeEvent();

	void cleanTexture();

protected:
	std::shared_ptr<sf::Sprite> g_sprite;
	std::shared_ptr<P1_Engine::Animation> g_anim;
	bool m_isAnimated;
	bool m_isActive;
	std::shared_ptr<sf::Texture> m_tx;
	int m_animationSpeed;
};

