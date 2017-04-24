#include "Camera.h"

Camera::Camera()
{
	m_speed = std::pow(2,0);
	m_view = std::make_shared<sf::View>();
	m_view->setSize(P1_Engine::getNativeResolutionX(), P1_Engine::getNativeResolutionY());
	m_view->setCenter(0, 0);
	m_pos = sf::Vector2i(m_view->getCenter());
	m_finalPos = sf::Vector2i(m_view->getCenter());
	m_factorPos = sf::Vector2i(0, 0);
	isMoving = false;
	
}

Camera::~Camera()
{
}

void Camera::drawView(sf::RenderWindow & win)
{
	win.setView(*m_view.get());
}

void Camera::update()
{
	/*m_pos.x += m_speed * m_factorPos.x;
	m_pos.y += m_speed * m_factorPos.y;

	m_view->setCenter(sf::Vector2f(m_pos));
	
	if (m_pos == m_finalPos) {
		m_factorPos = sf::Vector2i(0, 0);
		isMoving = false;
	}*/
}

std::shared_ptr<sf::View> Camera::getView()
{
	return m_view;
}

void Camera::move(int dX, int dY)
{
	m_finalPos = m_pos + sf::Vector2i(dX, dY);
	isMoving = true;
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

	P1_Engine::setLastDeltaCamMove_X(dX);
	P1_Engine::setLastDeltaCamMove_Y(dY);
}

sf::Vector2f Camera::adjustPos(sf::Vector2i vect)
{
	int x = vect.x;
	int y = vect.y;
	if (x%P1_Engine::getSpriteSizeUnit() >= P1_Engine::getSpriteSizeUnit() / 2) {
		int k = x / P1_Engine::getSpriteSizeUnit();
		k++;
		x = k * P1_Engine::getSpriteSizeUnit();
	}
	else {
		int k = x / P1_Engine::getSpriteSizeUnit();
		x = k * P1_Engine::getSpriteSizeUnit();
	}

	if (y%P1_Engine::getSpriteSizeUnit() >= P1_Engine::getSpriteSizeUnit() / 2) {
		int k = y / P1_Engine::getSpriteSizeUnit();
		k++;
		y = k * P1_Engine::getSpriteSizeUnit();
	}
	else {
		int k = y / P1_Engine::getSpriteSizeUnit();
		y = k * P1_Engine::getSpriteSizeUnit();
	}
	return sf::Vector2f(x,y);
}

sf::Vector2f Camera::adjustPos(sf::Vector2f vect)
{
	int x = vect.x;
	int y = vect.y;
	if (x%P1_Engine::getSpriteSizeUnit() >= P1_Engine::getSpriteSizeUnit() / 2) {
		int k = x / P1_Engine::getSpriteSizeUnit();
		k++;
		x = k * P1_Engine::getSpriteSizeUnit();
	}
	else {
		int k = x / P1_Engine::getSpriteSizeUnit();
		x = k * P1_Engine::getSpriteSizeUnit();
	}

	if (y%P1_Engine::getSpriteSizeUnit() >= P1_Engine::getSpriteSizeUnit() / 2) {
		int k = y / P1_Engine::getSpriteSizeUnit();
		k++;
		y = k * P1_Engine::getSpriteSizeUnit();
	}
	else {
		int k = y / P1_Engine::getSpriteSizeUnit();
		y = k * P1_Engine::getSpriteSizeUnit();
	}
	return sf::Vector2f(x, y);
}

bool Camera::getIsMoving()
{
	return isMoving;
}

void Camera::centerOnActor(Actor & target){
	//this->move(target.getPositionInPixels().x - m_pos.x, target.getPositionInPixels().y - m_pos.y);
	m_view->setCenter(sf::Vector2f(target.getPositionInPixels()));
}

