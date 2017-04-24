#include "Layer.h"
 //DEFINITIONS DANS LE .h


Layer::Layer(int order, std::string mapName) : m_mapName(mapName), m_loadingOrder(order)
{
	m_tilesetTex = std::make_shared<sf::Texture>();
}

Layer::~Layer()
{

}

std::vector<int> Layer::getFile(std::string fileName)
{
	std::ifstream stream(fileName, std::ios::in);
	std::vector<int> file;
	if (stream) {
		std::string strtemp;
		std::getline(stream, strtemp);
		while (strtemp != "BEGIN")
		{
			std::getline(stream, strtemp);
		}
		while (!stream.eof())
		{
			int temp;
			stream >> temp;
			file.push_back(temp);
		}
		stream.close();
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier .pmap" << std::endl;
	}
	return file;
}

 void Layer::initLayer(std::vector<int> file)
{
	for (std::vector<int>::iterator it = file.begin(); it != file.end() - 2; it++) {
		m_NewLayer.push_back(*it);
	}
	m_dimensions.x = file.at(file.size() - 2);
	m_dimensions.y = file.at(file.size() - 1);
}

 int Layer::posToID(sf::Vector2i pos)
{
	return m_dimensions.x*pos.y + pos.x;
}

std::pair<int, int> Layer::idToPos(int id)
{
	return std::pair<int, int>((id%m_dimensions.x), id / m_dimensions.x);
}

std::string Layer::getMapName()
{
	return m_mapName;
}

void Layer::drawToScreen()
{

	for (int i = 0; i < m_NewLayer.size(); i++) {

		sf::Vector2f pos(idToPos(i).first*P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleX(), idToPos(i).second*P1_Engine::getSpriteSizeUnit()*P1_Engine::getSpriteScaleY());
		m_tilesetSprites.at(m_NewLayer.at(i))->setPosition(pos);
		P1_Engine::getWindow()->draw(*m_tilesetSprites.at(m_NewLayer.at(i)).get());
	}

}

void Layer::update()
{
	
}

void Layer::loadTileset()
{
	m_tilesetSprites.clear();
	std::string fileName = P1_Engine::getFileNameMap(m_mapName, FILE_TYPES::TILESET);
	m_tilesetTex->loadFromFile(fileName);
	int nrow = m_tilesetTex->getSize().y / P1_Engine::getSpriteSizeUnit();
	int ncol = m_tilesetTex->getSize().x / P1_Engine::getSpriteSizeUnit();
	for (int x = 0; x < ncol; x++) {
		for (int y = 0; y < nrow; y++) {
			m_tilesetSprites.push_back(std::make_shared<sf::Sprite>());
			m_tilesetSprites.back()->setTexture(*m_tilesetTex.get());
			//A CORRIGER ???
			m_tilesetSprites.back()->setTextureRect(sf::IntRect((y*P1_Engine::getSpriteSizeUnit()) % 320, ((y*P1_Engine::getSpriteSizeUnit()) / 320)*P1_Engine::getSpriteSizeUnit(),P1_Engine::getSpriteSizeUnit(), P1_Engine::getSpriteSizeUnit()));
			m_tilesetSprites.back()->setScale(P1_Engine::getSpriteScale());
		}
	}


}

std::vector<int>* Layer::getLayer()
{
	return &m_NewLayer;
}

sf::Vector2i Layer::getDimensions()
{
	return m_dimensions;
}

void Layer::loadMap(std::string mapName)
{
	m_mapName = mapName;
	loadTileset();
	initLayer(getFile(P1_Engine::getFileNameMap(m_mapName, FILE_TYPES(m_loadingOrder))));
}
