#pragma once
#include<string>
#include<memory>
#include<map>
#include<vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}
using namespace std;
class ResourceManager {
public:
	static void setExecutablePath(const string& executablePath);
	static void unloadAllResources();
	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;
	ResourceManager& operator = (ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static shared_ptr<Renderer::ShaderProgram> loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath);
	static shared_ptr<Renderer::ShaderProgram> getShaderProgram(const string& shaderName);
	static shared_ptr<Renderer::Texture2D> loadTextures(const string& textureName, const string& texturePath);
	static shared_ptr<Renderer::Texture2D> getTexture(const string& textureName);
	static shared_ptr<Renderer::Sprite> loadSprite(const string& spriteName, 
												   const string& textureName,
												   const string& shaderName, 
												   const unsigned int spriteWidth,
												   const unsigned int spriteHeight,
												   const string& subTextureName= "Default");
	static shared_ptr<Renderer::Sprite> getSprite(const string& spriteName);

	static shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(const string& spriteName,
																   const string& textureName,
																   const string& shaderName,
																   const unsigned int spriteWidth,
															       const unsigned int spriteHeight,
																   const string& subTextureName = "Default");
	static shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(const string& spriteName);

	static shared_ptr<Renderer::Texture2D> loadTextureAtlas(const string& textureName,
															const string& texturePath,
															const vector<string> subTextures,
															const unsigned int subTextureWidth,
															const unsigned int subTextureHeight);
private:
	static string getFileString(const string& relativeFilePath);
	typedef map<const string, shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;
	typedef map<const string, shared_ptr<Renderer::Texture2D>> TexturesMap; 
	static TexturesMap m_textures;

	typedef map<const string, shared_ptr<Renderer::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	typedef map<const string, shared_ptr<Renderer::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animatedSprites;

	static string m_path;
};