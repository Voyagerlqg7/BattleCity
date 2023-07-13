#pragma once
#include<glm/vec2.hpp>
#include<vector>
#include<string>
#include<memory>
#include "GameObjects/IGameObject.h"
using namespace std;

class Level {
public:
	Level(const std::vector<string>& levelDescription);
	void render()const;
	void update(const uint64_t delta);
private:
	size_t m_width;
	size_t m_height;
	std::vector<shared_ptr<IGameObject>> m_mapObject;
};