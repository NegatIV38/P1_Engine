#include "Dialogue.h"

using namespace sf;

Dialogue::Dialogue()
{
	m_bTx = std::make_shared<sf::Texture>();
	m_text = std::make_shared<sf::Text>();
	g_cadre = std::make_shared<sf::RectangleShape>();
	m_back = std::make_shared<sf::Sprite>();
	g_name = std::make_shared<sf::RectangleShape>();
	m_name = std::make_shared<sf::Text>();
	g_options = std::make_shared<sf::RectangleShape>();


	m_outlineSize = 5;
	m_sizeBoxY = 150;
	m_currentOption = 0;
	initCadre();
	initBackground();
	initOptions();
	initText(m_text);
	initText(m_name);
	loadNameText();
	
	isShowing = false;
}


Dialogue::~Dialogue()
{
}

void Dialogue::initCadre()
{
	g_cadre->setSize(sf::Vector2f(P1_Engine::getNativeResolutionX() - (2 * m_outlineSize), m_sizeBoxY));
	g_cadre->setOrigin(g_cadre->getSize().x / 2, g_cadre->getSize().y / 2);
	g_cadre->setOutlineColor(sf::Color::White);
	g_cadre->setFillColor(sf::Color::Black);
	g_cadre->setOutlineThickness(m_outlineSize);
}

void Dialogue::initBackground()
{
	m_back->setOrigin(m_back->getLocalBounds().width / 2, m_back->getLocalBounds().height / 2);
	m_bTx->loadFromImage(getBackground(sf::Color::Black, sf::Color::Blue));
	m_back->setTexture(*m_bTx.get());
}

void Dialogue::initOptions()
{
	g_options->setOrigin(g_options->getSize().x / 2, g_options->getSize().y / 2);
	g_options->setOutlineColor(sf::Color::White);
	g_options->setFillColor(sf::Color::Black);
	g_options->setOutlineThickness(m_outlineSize);
}

void Dialogue::resetTextPosition()
{
	setTextRelativePosition(sf::Vector2f(10, 10));
}

void Dialogue::setPosition(sf::Vector2f pos)
{
	g_cadre->setPosition(pos);
}

void Dialogue::setOutlineColor(sf::Color col)
{
	g_cadre->setOutlineColor(col);
}

void Dialogue::setFillColor(sf::Color col)
{
	g_cadre->setFillColor(col);
}

void Dialogue::setSize(sf::Vector2f size)
{
	g_cadre->setSize(size);
}

void Dialogue::setText(std::string str)
{
	m_text->setString(str);
}

void Dialogue::setCharacterSize(int size)
{
	m_text->setCharacterSize(size);
}

int Dialogue::getCharacterSize()
{
	return m_text->getCharacterSize();
}

void Dialogue::setTextColor(sf::Color col)
{
	m_text->setColor(col);
}

void Dialogue::setTextRelativePosition(sf::Vector2f pos)
{
	m_textOffsetX = pos.x;
	m_text->setPosition(g_cadre->getPosition() + pos);
}

void Dialogue::resizeText()
{

	if (m_text->getString().getSize() > maxCharInARow()) {
		std::string str = m_text->getString();
		std::vector<std::string> t_str;
		while (str.find(" ") != std::string::npos)
		{
			std::size_t pos = str.find(" ");
			std::string temp = str.substr(0, pos+1);
			t_str.push_back(temp);
			str.replace(0, pos+1, "");
		}
		t_str.push_back(str);
		int currentSize = 0;
		str = "";
		for (int i = 0; i < t_str.size(); i++) {
			currentSize += t_str.at(i).size();
			if (currentSize < maxCharInARow()) {
				str.append(t_str.at(i));
			}
			else {
				currentSize = t_str.at(i).size();
				str.append("\n" + t_str.at(i));
			}
		}
		setText(str);
	}
}

void Dialogue::setName(std::string name)
{
	m_name->setString(name);
}

void Dialogue::setBackgroundGradient(sf::Color a, sf::Color b)
{
	m_bTx->loadFromImage(getBackground(a, b));
	loadNameText();
}

void Dialogue::recadre()
{
	
	g_cadre->setPosition(P1_Engine::getCamera()->getView()->getCenter() + 
		sf::Vector2f(0,P1_Engine::getNativeResolutionY()/2 - g_cadre->getSize().y/2 - g_cadre->getOutlineThickness()));

	m_back->setPosition(g_cadre->getPosition()- sf::Vector2f(m_back->getLocalBounds().width / 2, m_back->getLocalBounds().height / 2));
	m_text->setPosition(m_back->getPosition() + sf::Vector2f(10,10));
	recadreName();
	recadreOptions();
}

void Dialogue::recadreName()
{
	g_name->setPosition(g_cadre->getPosition() - sf::Vector2f(g_cadre->getSize().x/2-g_name->getSize().x, g_cadre->getSize().y / 2 + g_name->getSize().y));
	m_name->setPosition(g_name->getPosition() + sf::Vector2f(10,5));

}

void Dialogue::recadreOptions()
{
	g_options->setSize(sf::Vector2f(75 + getMaxOptionSizeX(), m_options.size() * 50));
	g_options->setPosition(g_cadre->getPosition() + sf::Vector2f((P1_Engine::getNativeResolutionX() / 2) - (g_options->getSize().x ), -(g_cadre->getSize().y / 2 + m_outlineSize + g_options->getSize().y/2)));
	for (int i = 0; i < m_options.size(); i++) {
		m_options.at(i)->setPosition(g_options->getPosition() + sf::Vector2f(10, (i * 50) + 10));
	}
}

void Dialogue::setDialogueFromQuete(P1_Engine::Quete & quete)
{
	m_options.clear();
	setText(quete.text);
	if (quete.hasName) {
		showName(true);
		setName(quete.name);
	}
	showOptions(quete.hasOption);
	if (m_hasOptions) {
		for (int i = 0; i < quete.options.size(); i++) {
			m_options.push_back(std::make_shared<sf::Text>());
			m_options.back()->setString(quete.options.at(i).text);
			initText(m_options.back());
		}
		reloadOptionSelected();
	}
}

void Dialogue::initText(std::shared_ptr<sf::Text>& text)
{
	text->setFont(*P1_Engine::getFont());
	text->setCharacterSize(32);
	text->setColor(sf::Color::White);
}

void Dialogue::showName(bool arg)
{
	m_hasName = arg;
}

void Dialogue::showOptions(bool arg)
{
	m_hasOptions = arg;
}

void Dialogue::show(bool arg)
{
	isShowing = arg;
}

bool Dialogue::getIsShowing()
{
	return isShowing;
}

bool Dialogue::getHasOptions()
{
	return m_hasOptions;
}

void Dialogue::selectUp()
{
	if (m_hasOptions) {
		if (m_currentOption > 0) {
			deselectOption();
			m_currentOption--;
			reloadOptionSelected();
		}
		P1_Engine::playSound("a_res/Select_0.wav");
	}
}

void Dialogue::selectDown()
{
	if (m_hasOptions) {
		if (m_currentOption < m_options.size()-1) {
			deselectOption();
			m_currentOption++;
			reloadOptionSelected();
		}
		P1_Engine::playSound("a_res/Select_0.wav");
	}
}

void Dialogue::deselectOption()
{
	std::string str = m_options.at(m_currentOption)->getString();
	if (str.at(0) == '>' && str.at(1) == ' ') {
		std::size_t pos = str.find("> ");
		pos += 2;
		str = str.substr(pos);
	}
	m_options.at(m_currentOption)->setString(str);
}

void Dialogue::reloadOptionSelected()
{
	std::string str = m_options.at(m_currentOption)->getString();
	str = "> " + str;
	m_options.at(m_currentOption)->setString(str);
}

int Dialogue::getCurrentOption()
{
	return m_currentOption;
}

int Dialogue::getMaxOptionSizeX()
{
	int sizeMax = 0;
	for (int i = 0; i < m_options.size(); i++) {
		if (m_options.at(i)->getLocalBounds().width > sizeMax) {
			sizeMax = m_options.at(i)->getLocalBounds().width;
		}
	}
	return sizeMax;
}

void Dialogue::setCurrentOption(int arg)
{
	m_currentOption = arg;
}

void Dialogue::update()
{
	resizeText();
	recadre();
}

sf::Image Dialogue::getBackground(sf::Color a, sf::Color b)
{
	m_headColor = a;
	sf::Image im;
	im.create(g_cadre->getSize().x, g_cadre->getSize().y,a);
	sf::Color d = a;
	sf::Color e = b - a;
	e.r /= im.getSize().y;
	e.g /= im.getSize().y;
	e.b /= im.getSize().y;
	for (int i = 0; i < im.getSize().y; i++) {
		for (int j = 0; j < im.getSize().x; j++) {
			im.setPixel(j, i, d + sf::Color(i*e.r,i*e.g,i*e.b));
		}
	}
	return im;
}

int Dialogue::maxCharInARow()
{
	return (P1_Engine::getNativeResolutionX() - (2*m_outlineSize) - (2 * m_textOffsetX))/(getCharacterSize()/2);
}

void Dialogue::loadNameText()
{
	g_name->setFillColor(m_headColor);
	g_name->setOutlineColor(g_cadre->getOutlineColor());
	g_name->setOutlineThickness(m_outlineSize);
	g_name->setPosition(g_cadre->getPosition().x + 75, g_cadre->getPosition().y - 50);
	g_name->setSize(sf::Vector2f(150, 50));
}

void Dialogue::draw()
{
	if (isShowing) {
		if (m_hasName) {

			P1_Engine::getWindow()->draw(*g_name.get());
			P1_Engine::getWindow()->draw(*m_name.get());
		}
		P1_Engine::getWindow()->draw(*g_cadre.get());
		P1_Engine::getWindow()->draw(*m_back.get());
		P1_Engine::getWindow()->draw(*m_text.get());
		if (m_hasOptions) {
			P1_Engine::getWindow()->draw(*g_options.get());
			for (std::vector<std::shared_ptr<sf::Text>>::iterator it = m_options.begin(); it != m_options.end(); it++) {
				P1_Engine::getWindow()->draw(*it->get());
			}
		}
	}
}
