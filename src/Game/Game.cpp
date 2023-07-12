#include<glm/vec2.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Game.h"
#include"../Renderer/ShaderProgram.h"
#include"../Resources/ResourceManager.h"
#include"../Renderer/Texture2D.h"
#include"../Renderer/Sprite.h"
#include"../Renderer/AnimatedSprite.h"
#include<iostream>


using namespace std;

Game::Game(const glm::ivec2& windowSize)
	: m_eCurrentGameState(EGameState::Active)
    , m_windowSize(windowSize)
{
	m_keys.fill(false);
}
Game::~Game() {

}

void Game::render() {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}
void Game::update(const uint64_t delta) {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}
void Game::setkey(const int key, const int action) {
	m_keys[key] = action;
}
bool Game::init() {
    ResourceManager::loadJSONResources("res/resources.json");

   
    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    ResourceManager::loadTextures("DefaultTexture", "res/textures/map_16x16.png");
    


    vector<string> subTexturesNames = {
    };

    auto pTextureAtlas = ResourceManager::loadTextureAtlas("defaultTextureAtlas", "res/textures/map_16x16.png", move(subTexturesNames), 16, 16);
    auto pSprite = ResourceManager::loadSprite("newSprite", "defaultTextureAtlas", "SpriteShader", 100, 100, "");
    pSprite->setPosition(glm::vec2(300, 100));

    


    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 200.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 200.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    return true;
}