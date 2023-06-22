#pragma once
#include<string>
#include<memory>
#include<map>
#include<vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
}
using namespace std;
class ResourceManager {
public:
	ResourceManager(const string& executablePath);
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;
	ResourceManager& operator = (ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	shared_ptr<Renderer::ShaderProgram> loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath);
	shared_ptr<Renderer::ShaderProgram> getShaderProgram(const string& shaderName);
	shared_ptr<Renderer::Texture2D> loadTextures(const string& textureName, const string& texturePath);
	shared_ptr<Renderer::Texture2D> getTexture(const string& textureName);
	shared_ptr<Renderer::Sprite> loadSprite(const string& spriteName, 
		const string& textureName,
		const string& shaderName, 
		const unsigned int spriteWidth,
		const unsigned int spriteHeight,
		const string& subTextureName= "Default");
	shared_ptr<Renderer::Sprite> getSprite(const string& spriteName);

	shared_ptr<Renderer::Texture2D> loadTextureAtlas(const string& textureName,
													 const string& texturePath,
													 const vector<string> subTextures,
													 const unsigned int subTextureWidth,
													 const unsigned int subTextureHeight);
private:
	string getFileString(const string& relativeFilePath) const;
	typedef map<const string, shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;
	typedef map<const string, shared_ptr<Renderer::Texture2D>> TexturesMap; 
	TexturesMap m_textures;

	typedef map<const string, shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	string m_path;
};