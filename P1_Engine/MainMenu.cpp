#include "MainMenu.h"

bool MainMenu::s_notExecuted = false;

MainMenu::MainMenu(std::string name)
{
	m_font = *P1_Engine::getFont();
	m_optionsText.push_back(std::make_shared<sf::Text>());
	m_optionsText.back()->setFont(m_font);
	m_optionsText.back()->setString(name);
	loadTextOption(m_optionsText.back().get(), 32, sf::Color::White);
	placeMainTitle(m_optionsText.back().get());
	m_optionsCount = 0;
	m_optionSelected = 1;
	m_optionsIndent = 120;
	m_optionsCharSize = 24;
	m_optionsStartY = 200;
	m_active = false;

	m_name = name;
	P1_Engine::getMenus_List()->emplace(m_name, std::shared_ptr<MainMenu>(this));
}


MainMenu::~MainMenu()
{
}

void MainMenu::setMainTitle(std::string name)
{
	m_optionsText.front()->setString(name);
}

void MainMenu::setOptionText(std::string name, int id)
{
	m_optionsText.at(id)->setString(name);
}

void MainMenu::pushOption(std::string name)
{
	m_optionsText.push_back(std::make_shared<sf::Text>());
	m_optionsText.back()->setFont(m_font);
	m_optionsText.back()->setString(name);
	loadTextOption(m_optionsText.back().get(), m_optionsCharSize, sf::Color::White);
	//Replace
	placeOption(m_optionsText.back().get(), m_optionsCount);
	m_optionsCount++;
}

void MainMenu::popOption()
{
	m_optionsText.pop_back();
	m_optionsCount--;
	
}

void MainMenu::placeMainTitle(sf::Text * mainTitle)
{
	sf::FloatRect textRect = mainTitle->getLocalBounds();
	mainTitle->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	mainTitle->setPosition(P1_Engine::getNativeResolutionX() / 2.0f, 50);
}

void MainMenu::placeOption(sf::Text * option, int id)
{
	option->setPosition(m_optionsIndent, (id*m_optionsCharSize*2) + m_optionsStartY);
}

void MainMenu::loadTextOption(sf::Text * option, int charSize, sf::Color textColor)
{
	option->setCharacterSize(charSize);
	option->setColor(textColor);
}

void MainMenu::draw()
{
	if (isActive()) {
		for (std::vector<std::shared_ptr<sf::Text>>::iterator it = m_optionsText.begin();
			it != m_optionsText.end(); it++) {
			P1_Engine::getWindow()->draw(*(*it).get());
		}
	}
}

void MainMenu::selectUp()
{
	if (isActive()) {
		if (m_optionSelected > 1) {
			deselectText();
			m_optionSelected--;
			reloadTextSelected();
		}

		P1_Engine::playSound("a_res/Select_0.wav");
	}
}

void MainMenu::selectDown()
{
	if (isActive()) {
		if (m_optionSelected < m_optionsCount) {
			deselectText();
			m_optionSelected++;
			reloadTextSelected();
		}
		P1_Engine::playSound("a_res/Select_0.wav");
	}
}

int MainMenu::getOptionSelected()
{
	return m_optionSelected;
}

void MainMenu::reloadTextSelected()
{
	std::string str = m_optionsText.at(m_optionSelected)->getString();
	str = "> " + str;
	m_optionsText.at(m_optionSelected)->setString(str);
}

void MainMenu::deselectText()
{
	std::string str = m_optionsText.at(m_optionSelected)->getString();

	if (str.at(0) == '>' && str.at(1) == ' ') {
		std::size_t pos = str.find("> ");
		pos += 2;
		str = str.substr(pos);
	}

	m_optionsText.at(m_optionSelected)->setString(str);
}

void MainMenu::setRowExecTable(int id, COMMANDS cmd)
{
	m_execTable[id] = cmd;
	setRowParamTable(id);
}

void MainMenu::setRowParamTable(int id, std::string param)
{
	m_paramTable[id] = param;
}

void MainMenu::execute()
{
	if (isActive() && !getStaticExec()) {
		P1_Engine::execute(m_execTable.at(m_optionSelected),m_paramTable.at(m_optionSelected));
		P1_Engine::playSound("a_res/ValidateOption.wav");
		setStaticExec(true);
	}
}

void MainMenu::setTextColor(int id, sf::Color col)
{
	m_optionsText.at(id)->setColor(col);
}

void MainMenu::setActive(bool isActive)
{
	m_active = isActive;
}

bool MainMenu::isActive()
{
	return m_active;
}

std::string MainMenu::getName()
{
	return m_name;
}

void MainMenu::setStaticExec(bool arg)
{
	s_notExecuted = arg;
}

bool MainMenu::getStaticExec()
{
	return s_notExecuted;
}
