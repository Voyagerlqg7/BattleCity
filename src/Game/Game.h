#pragma once
#include<iostream>
#include<array>
#include<glm/vec2.hpp>
using namespace std;
class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const uint64_t detla);
	void setkey(const int key, const int action);
	bool init();
private:
	array<bool, 349> m_keys;
	enum class EGameState {
		Active,
		Pause
	};
	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
};