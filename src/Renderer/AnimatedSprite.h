#pragma once
#include"Sprite.h"
#include<string>
#include<iostream>
#include<vector>
#include<map>
using namespace std;

namespace Renderer {
	class AnimatedSprite : public Sprite {
	public:
		AnimatedSprite(const shared_ptr<Texture2D> pTexture,
			const string initialSubTexture,
			const shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2& position = glm::vec2(0.f),
			const glm::vec2& size = glm::vec2(1.f),
			const float rotation = 0.f);
		void insertState(string state, vector <pair<string, uint64_t>> subTexturesDuration);
		void render() const override;
		void update(const uint64_t delta);
		void setState(const string& newState);
	private:
		map<string, vector<pair<string, uint64_t>>> m_statesMap;
		size_t m_currentFrame = 0;
		uint64_t m_currentAnimationTime = 0;
		map<string, vector<pair<string, uint64_t>>>::const_iterator m_pCurrentAnimationDurations;
		mutable bool m_dirty = false;
	};
}