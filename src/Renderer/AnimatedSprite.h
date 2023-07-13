#pragma once
#include"Sprite.h"
#include<string>
#include<iostream>
#include<vector>
#include<map>
using namespace std;

namespace RenderEngine {
	class AnimatedSprite : public Sprite {
	public:
		AnimatedSprite(const shared_ptr<Texture2D> pTexture,
			const string initialSubTexture,
			const shared_ptr<ShaderProgram> pShaderProgram);
		void insertState(string state, vector <pair<string, uint64_t>> subTexturesDuration);
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation) const override;
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