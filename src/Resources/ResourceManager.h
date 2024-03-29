#pragma once
#include<string>
#include<memory>
#include<map>
#include<vector>

namespace RenderEngine {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
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

	static shared_ptr<RenderEngine::ShaderProgram> loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath);
	static shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(const string& shaderName);
	static shared_ptr<RenderEngine::Texture2D> loadTextures(const string& textureName, const string& texturePath);
	static shared_ptr<RenderEngine::Texture2D> getTexture(const string& textureName);
	static shared_ptr<RenderEngine::Sprite> loadSprite(const string& spriteName, 
												   const string& textureName,
												   const string& shaderName, 
												   const string& subTextureName= "Default");
	static shared_ptr<RenderEngine::Sprite> getSprite(const string& spriteName);

	static shared_ptr<RenderEngine::Texture2D> loadTextureAtlas(const string& textureName,
															const string& texturePath,
															const vector<string> subTextures,
															const unsigned int subTextureWidth,
															const unsigned int subTextureHeight);
	static bool loadJSONResources(const string& JSONPath);

	static const vector<vector<string>>& getLevels() { return m_levels; }

private:
	static string getFileString(const string& relativeFilePath);
	typedef map<const string, shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;
	typedef map<const string, shared_ptr<RenderEngine::Texture2D>> TexturesMap; 
	static TexturesMap m_textures;

	typedef map<const string, shared_ptr<RenderEngine::Sprite>> SpritesMap;
	static SpritesMap m_sprites;


	static vector<vector<string>> m_levels;


	static string m_path;
};