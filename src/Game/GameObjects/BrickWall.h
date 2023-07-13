#pragma once

#include"IGameObject.h"
#include<memory>
using namespace std;
namespace RenderEngine {
	class Sprite;
}
class BrickWall : public IGameObject {
	public:
		BrickWall(const shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation);
		virtual void render() const override;
		virtual void update(const uint64_t delta) override;
private:
	shared_ptr<RenderEngine::Sprite> m_pCurrentSprite;

};
