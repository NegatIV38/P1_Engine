#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <SFML\Graphics.hpp>
#include "Actor.h"
#include <SFML\System.hpp>

enum class FILE_TYPES;

class Layer
{
public:

	Layer(int order, std::string mapName);

	~Layer();

	std::vector<int> getFile(std::string fileName);
	void initLayer(std::vector<int> file);
	int posToID(sf::Vector2i pos);
	std::pair<int, int> idToPos(int id);

	std::string getMapName();

	void drawToScreen();
	void update();
	void loadTileset();

	std::vector<int>* getLayer();
	sf::Vector2i getDimensions();

	void loadMap(std::string mapName);

private:
	int m_loadingOrder;
	sf::Vector2i m_dimensions;
	std::string m_mapName;

	std::shared_ptr<sf::Texture> m_tilesetTex;
	std::vector<std::shared_ptr<sf::Sprite>> m_tilesetSprites;
	std::vector<int> m_NewLayer;
};

