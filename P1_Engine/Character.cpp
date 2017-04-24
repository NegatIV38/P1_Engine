#include "Character.h"

class Layer;

Character::Character()
{
}

Character::Character(std::string txFile) :
	Actor(txFile)
{
}

Character::Character(std::string txFile, int animLenght, int animSpeed) :
	Actor(txFile,animLenght, animSpeed)
{
	m_speed = 4;
}


Character::~Character()
{
}

void Character::moveRight()
{
	if (!isMoving) {
		if (getIsAnimated()) {
			getAnimation()->currentStyle = SPRITE_STYLE::RIGHT;
			P1_Engine::startAnimation(*getAnimation().get());
		}
		move(P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX(), 0);
	}
}

void Character::moveLeft()
{
	if (!isMoving) {
		if (getIsAnimated()) {
			getAnimation()->currentStyle = SPRITE_STYLE::LEFT;
			P1_Engine::startAnimation(*getAnimation().get());
		}
		move(-P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX(), 0);
	}
}

void Character::moveUp()
{
	if (!isMoving) {
		if (getIsAnimated()) {
			getAnimation()->currentStyle = SPRITE_STYLE::BACK;
			P1_Engine::startAnimation(*getAnimation().get());
		}
		move(0, -P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX());
	}
}

void Character::moveDown()
{
	if (!isMoving) {
		if (getIsAnimated()) {
			getAnimation()->currentStyle = SPRITE_STYLE::FACE;
			P1_Engine::startAnimation(*getAnimation().get());
		}
		move(0, P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX());
	}
}

void Character::stopMove()
{
	if (getIsAnimated()) {
		stop = true;
	}
}

void Character::move(int dX, int dY)
{
	isMoving = true;
	m_finalPos = m_pos + sf::Vector2i(dX, dY);
	sf::Vector2i gridFinalPos = P1_Engine::posToGridPos(sf::Vector2f(m_finalPos));
	if (gridFinalPos.x >= 0 && gridFinalPos.y >= 0 &&
		gridFinalPos.x < P1_Engine::getLayer_2()->getDimensions().x &&
		gridFinalPos.y < P1_Engine::getLayer_2()->getDimensions().y) {
		if (P1_Engine::getLayer_2()->getLayer()->at(P1_Engine::getLayer_2()->posToID(gridFinalPos)) == 0) {

			stop = false;
			if (dX < 0) {
				m_factorPos.x = -1;
			}
			else if (dX > 0) {
				m_factorPos.x = 1;
			}
			else {
				m_factorPos.x = 0;
			}


			if (dY < 0) {
				m_factorPos.y = -1;
			}
			else if (dY > 0) {
				m_factorPos.y = 1;
			}
			else {
				m_factorPos.y = 0;
			}
		}
		else {
			m_finalPos = m_pos;
		}
	}
	else {
		m_finalPos = m_pos;
	}
}

void Character::update()
{
	Actor::update();

	m_pos.x += m_speed * m_factorPos.x;
	m_pos.y += m_speed * m_factorPos.y;

	getSprite()->setPosition(sf::Vector2f(m_pos));

	
	if (stop && !isMoving) {

		P1_Engine::stopAnimation(*getAnimation().get());
	}

	if (m_pos == m_finalPos) {
		m_factorPos = sf::Vector2i(0, 0);
		isMoving = false;
		
	}
}

bool Character::getIsMoving()
{
	return isMoving;
}

void Character::setSpeed(float newSpeed)
{
	m_speed = newSpeed;
}
