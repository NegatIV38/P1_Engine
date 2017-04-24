#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include "P1_Engine.h"

enum class COMMANDS;

class MainMenu
{
public:
	MainMenu(std::string name);
	~MainMenu();

	void setMainTitle(std::string name);
	void setOptionText(std::string name, int id);
	void pushOption(std::string name);
	void popOption();
	void placeMainTitle(sf::Text* mainTitle);
	void placeOption(sf::Text* option, int id);
	void loadTextOption(sf::Text* option, int charSize, sf::Color textColor);
	void draw();
	void selectUp();
	void selectDown();
	int getOptionSelected();
	void reloadTextSelected();
	void deselectText();
	void setRowExecTable(int id, COMMANDS cmd);
	void setRowParamTable(int id, std::string param = "");
	void execute();
	void setTextColor(int id, sf::Color col);
	void setActive(bool isActive);
	bool isActive();
	std::string getName();

	static void setStaticExec(bool arg);
	static bool getStaticExec();

private :
	int m_optionsCount;
	int m_optionSelected;
	int m_optionsIndent;
	int m_optionsStartY;
	int m_optionsCharSize;
	bool m_active;
	std::string m_name;
	std::vector<std::shared_ptr<sf::Text>> m_optionsText;
	std::map<int, COMMANDS> m_execTable;
	std::map<int, std::string> m_paramTable;
	sf::Font m_font;

	static bool s_notExecuted;
};

