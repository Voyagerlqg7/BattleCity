#include"ResourceManager.h"
#include"../Renderer/ShaderProgram.h"
#include"../Renderer/Texture2D.h"
#include"../Renderer/Sprite.h"
#include"../Renderer/AnimatedSprite.h"
#include<sstream>
#include<fstream>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include"stb_image.h"
#include<vector>
#include<rapidjson/document.h>
#include<rapidjson/error/en.h>

using namespace std;


ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
string ResourceManager::m_path;
void ResourceManager::unloadAllResources() {
	m_shaderPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}
void ResourceManager::setExecutablePath(const string& executablePath){
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}
string ResourceManager::getFileString(const string& relativeFilePath)  {
	ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(),ios::in | ios::binary);
	if (!f.is_open()) {
		cout << "Failed to open file:"<< relativeFilePath << endl;
		return string{};
	}
	stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();

}
shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath)
{
	string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) {
		cout << "No vertex shader" << endl;
		return nullptr;
	}

	string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty()) {
		cout << "No fragment shader" << endl;
		return nullptr;
	}

	shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Cant load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}
shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it !=m_shaderPrograms.end()) {
		return it->second;
	}
	
	cout << "Cant find the shader program"<<shaderName << endl;
	return nullptr;
	
}
shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextures(const string& textureName, const string& texturePath) {
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char*pixels =  stbi_load(string(m_path + "/" + texturePath).c_str(), &width, &height, & channels, 0);

	if (!pixels) {
		cout << "Cant load image" << texturePath <<endl;
		return nullptr;
	}
	shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName, make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return newTexture;
}
shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}

	cout << "Cant find the texture " << textureName << endl;
	return nullptr;
}
shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const string& spriteName,
													     const string& textureName,
														 const string& shaderName,
														 const unsigned int spriteWidth,
														 const unsigned int spriteHeight,
														 const string& subTextureName ) {

	auto pTexture = getTexture(textureName);
	if (!pTexture) {
		cout << "Cant find the texture: " << textureName << "for the sprite:" << spriteName << endl;
		return nullptr;
	}
	auto pShader = getShaderProgram(shaderName);
	if (!pShader) {
		cout << "Cant find the shader: " << shaderName << "for the sprite:" << spriteName << endl;
		return nullptr;
	}
	shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName,
		make_shared<RenderEngine::Sprite>(
		pTexture,
		subTextureName,
		pShader, glm::vec2(0.f, 0.f),
		glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;

}
shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(const string& spriteName,
																		 const string& textureName,
																		 const string& shaderName,
																		 const unsigned int spriteWidth,
																		 const unsigned int spriteHeight,
																		 const string& subTextureName) {

	auto pTexture = getTexture(textureName);
	if (!pTexture) {
		cout << "Cant find the texture: " << textureName << "for the sprite:" << spriteName << endl;
		return nullptr;
	}
	auto pShader = getShaderProgram(shaderName);
	if (!pShader) {
		cout << "Cant find the shader: " << shaderName << "for the sprite:" << spriteName << endl;
		return nullptr;
	}
	shared_ptr<RenderEngine::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName,
		make_shared<RenderEngine::AnimatedSprite>(
			pTexture,
			subTextureName,
			pShader, glm::vec2(0.f, 0.f),
			glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;

}
shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const string& spriteName) {
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end()) {
		return it->second;
	}

	cout << "Cant find the sprite " << spriteName << endl;
	return nullptr;
}
shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const string& spriteName) {
	auto it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end()) {
		return it->second;
	}

	cout << "Cant find the animated sprites " << spriteName << endl;
	return nullptr;
}
shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(const string& textureName,
																  const string& texturePath,
																  const vector<string> subTextures,
																  const unsigned int subTextureWidth,
																  const unsigned int subTextureHeight) {

	auto pTexture = loadTextures(move(textureName),move(texturePath));
	if (pTexture) {
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (const auto& currentSubTextureName: subTextures) {
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
			pTexture->addSubTexture(move(currentSubTextureName), leftBottomUV, rightTopUV);
			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= subTextureWidth) {
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}

		}
	}
	return pTexture;

}

bool ResourceManager::loadJSONResources(const string& JSONPath) {

	const string JSONString = getFileString(JSONPath);
	if (JSONString.empty()){
		cerr << "No JSON res file" << endl;
		return false;
	}
	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult) {
		cerr << "JSON Parse Error" << rapidjson::GetParseError_En(parseResult.Code())<< "(" << parseResult.Offset()<<")" << endl;
		cerr << "IN JSON File: " << JSONPath << endl;
		return false;
	}
	auto shadersIT =  document.FindMember("shaders");
	if (shadersIT != document.MemberEnd()) {
		for (const auto& currentShader : shadersIT->value.GetArray())
		{
			const string name = currentShader["name"].GetString();
			const string filePath_v = currentShader["filePath_v"].GetString();
			const string filePath_f = currentShader["filePath_f"].GetString();
			loadShaders(name, filePath_v, filePath_f);
		}
	}
	

}

