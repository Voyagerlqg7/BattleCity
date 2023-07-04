#pragma once
#include<iostream>
#include<memory>
#include<glm/vec2.hpp>

using namespace std;

namespace Renderer {
	class AnimatedSprite;
}
class Tank {
public:
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};
	Tank(shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position);
	void render()const;
	void setOrientation(const EOrientation eOrientation);
	void move(const bool move);
	void update(const uint64_t delta);
private:
	EOrientation m_eOrientation;
	shared_ptr < Renderer::AnimatedSprite> m_pSprite;
	bool m_move;
	float m_velocity;
	glm::vec2 position;
	glm::vec2 m_moveOffset;

};