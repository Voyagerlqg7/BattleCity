#include"Level.h"
#include"GameObjects/IGameObject.h"
#include"GameObjects/BrickWall.h"
#include"../Resources/ResourceManager.h"
#include<iostream>


const unsigned int BLOCK_SIZE = 16;
using namespace std;

shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation) {
	switch (description)
	{ 
	case '0':
		return make_shared<BrickWall>(ResourceManager::getSprite("brickWall_right"), position, size, rotation);
	case '1':
		return make_shared<BrickWall>(ResourceManager::getSprite("brickWall_bottom"), position, size, rotation);
	case '2':
		return make_shared<BrickWall>(ResourceManager::getSprite("brickWall_left"), position, size, rotation);
	case '3':
		return make_shared<BrickWall>(ResourceManager::getSprite("brickWall_top"), position, size, rotation);
	case '4':
		return make_shared<BrickWall>(ResourceManager::getSprite("brickWall"), position, size, rotation);
	default:
		break;
	}
}

Level::Level(const std::vector<string>& levelDescription) {
	if (levelDescription.empty()) {
		cerr << "Empty level description" << endl;
	}
	m_width = levelDescription[0].length();
	m_height = levelDescription.size();
	m_mapObject.reserve(m_width * m_height);
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (m_height - 1));
	for (const string& currentRow: levelDescription) {
		unsigned int currentLeftOffset = 0;
		for (const char currentElement : currentRow) {


			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
}
void Level::render()const {
	for (const auto& currentMapObject : m_mapObject) {
		if (currentMapObject) {
			currentMapObject->render();
		}
	}
}
void Level::update(const uint64_t delta) {
	for (const auto& currentMapObject : m_mapObject) {
		if (currentMapObject) {
			currentMapObject->update(delta);
		}
	}
}