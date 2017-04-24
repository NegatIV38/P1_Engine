#include "Actor.h"


Actor::Actor()
{
}

Actor::Actor(std::string txFile)
{
	m_tx = std::make_shared<sf::Texture>();
	m_tx->loadFromFile(txFile);
	g_sprite = std::make_shared<sf::Sprite>();
	g_sprite->setTexture(*m_tx.get());
	g_sprite->setScale(P1_Engine::getSpriteScale(m_tx->getSize().x));
	m_isAnimated = false;
	m_isActive = true;

	//g_sprite->setOrigin(P1_Engine::getSpriteSizeUnit() / 2, P1_Engine::getSpriteSizeUnit() / 2);
}

Actor::Actor(std::string txFile, int animLenght, int animSpeed) : m_animationSpeed(animSpeed)
{
	m_tx = std::make_shared<sf::Texture>();
	m_tx->loadFromFile(txFile);

	g_anim = std::make_shared<P1_Engine::Animation>();
	P1_Engine::initAnimation(*g_anim.get(), animLenght);
	g_anim.get()->sp->setTexture(*m_tx.get());
	g_anim.get()->sp->setScale(P1_Engine::getSpriteScale());

	g_sprite = std::make_shared<sf::Sprite>();
	g_sprite.reset(g_anim.get()->sp.get());
	
	m_isAnimated = true;
	m_isActive = true;
	//g_sprite->setOrigin(P1_Engine::getSpriteSizeUnit() / 2, P1_Engine::getSpriteSizeUnit() / 2);
}

Actor::~Actor()
{
}

void Actor::setPosition(int x, int y)
{
	P1_Engine::setSpritePositionOnGrid(*g_sprite.get(), x, y);
}

sf::Vector2i Actor::getPositionInPixels()
{
	return sf::Vector2i(g_sprite->getPosition());
}

void Actor::draw()
{
	if (m_isActive) {
		P1_Engine::getWindow()->draw(*getSprite());
	}
}

void Actor::update()
{
	if (m_isAnimated && m_isActive) {
		P1_Engine::setCurrentSpriteAnimationTile(*g_anim.get(),m_animationSpeed);
	}
}

bool Actor::getIsAnimated()
{
	return m_isAnimated;
}

bool Actor::getIsActive()
{
	return m_isActive;
}

std::shared_ptr<P1_Engine::Animation> Actor::getAnimation()
{
	return g_anim;
}

void Actor::setActive(bool arg)
{
	m_isActive = arg;
}

std::shared_ptr<sf::Sprite> Actor::getSprite()
{
	if (m_isAnimated) {
		return g_anim.get()->sp;
	}
	else {
		return g_sprite;
	}
	return nullptr;
}

void Actor::executeEvent()
{
}

void Actor::cleanTexture()
{
	P1_Engine::cleanTexture(*m_tx.get());
}
