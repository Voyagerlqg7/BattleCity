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
	size_t getLevelWidth();
	size_t getLevelHeight();

	const glm::ivec2& getPlayerRespawn_1()const { return m_playerRespawn_1; }
	const glm::ivec2& getPlayerRespawn_2()const { return m_playerRespawn_2; }
					
	const glm::ivec2& getEnemyRespawn_1() const { return m_enemyRespawn_1; }
	const glm::ivec2& getEnemyRespawn_2() const { return m_enemyRespawn_2; }
	const glm::ivec2& getEnemyRespawn_3() const { return m_enemyRespawn_3; }

private:
	size_t m_width = 0;
	size_t m_height = 0;
	glm::ivec2 m_playerRespawn_1;
	glm::ivec2 m_playerRespawn_2;
	glm::ivec2 m_enemyRespawn_1;
	glm::ivec2 m_enemyRespawn_2;
	glm::ivec2 m_enemyRespawn_3;

	std::vector<shared_ptr<IGameObject>> m_mapObject;
};