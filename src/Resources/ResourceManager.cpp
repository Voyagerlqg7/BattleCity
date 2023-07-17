#include"ResourceManager.h"
#include"../Renderer/ShaderProgram.h"
#include"../Renderer/Texture2D.h"
#include"../Renderer/Sprite.h"
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
string ResourceManager::m_path;
vector<vector<string>> ResourceManager::m_levels;

void ResourceManager::unloadAllResources() {
	m_shaderPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
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
		pShader)).first->second;
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
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX+0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);
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
	auto textureAtlasesIT = document.FindMember("textureAtlases");
	if (textureAtlasesIT != document.MemberEnd()) {
		for (const auto& currentTextureAtlas : textureAtlasesIT->value.GetArray())
		{
			const string name = currentTextureAtlas["name"].GetString();
			const string filePath = currentTextureAtlas["filePath"].GetString();
			const string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetUint();

			const auto subTexturesArray = currentTextureAtlas["subTextures"].GetArray();
			vector<string> subTextures;
			subTextures.reserve(subTexturesArray.Size());
			for (const auto& currentSubTexture : subTexturesArray) {
				subTextures.emplace_back(currentSubTexture.GetString());
			}
			loadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
		}
	}
	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd())
	{
		for (const auto& currentSprite : spritesIt->value.GetArray())
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string subTexture = currentSprite["initialSubTexture"].GetString();
			auto pSprite = loadSprite(name, textureAtlas, shader, subTexture);
			if (!pSprite) {
				continue;
			}
			auto framesIt = currentSprite.FindMember("frames");
			if (framesIt!=currentSprite.MemberEnd()){
				const auto framesArray = framesIt->value.GetArray();
				vector < RenderEngine::Sprite::FrameDescription> framesDescriptions;
				framesDescriptions.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTextureStr = currentSprite["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint64();
					const auto pTextureAtlas = getTexture(textureAtlas);
					const auto pSubTexture = pTextureAtlas->getSubTexture(subTextureStr);
					framesDescriptions.emplace_back(pSubTexture.leftBottomUV, pSubTexture.rightTopUV, duration);
				}
				pSprite->insertFrames(framesDescriptions);
			}

		}
	}


	auto animatedSpritesIT = document.FindMember("animatedSprites");
	if (animatedSpritesIT!=document.MemberEnd()) {
		for(const auto& currentAnimatedSprite: animatedSpritesIT->value.GetArray()){
			const string name = currentAnimatedSprite["name"].GetString();
			const string textureAtlas = currentAnimatedSprite["textureAtlas"].GetString();
			const string shader = currentAnimatedSprite["shader"].GetString();
			const string initialSubTexture = currentAnimatedSprite["initialSubTexture"].GetString();
			auto pAnimatedSprite = loadAnimatedSprite(name, textureAtlas, shader, initialSubTexture);
			if (!pAnimatedSprite) {
				continue;
			}
			const auto statesArray = currentAnimatedSprite["states"].GetArray();
			for (const auto& currentState : statesArray) {
				const string stateName = currentState["stateName"].GetString();
				vector<pair<string, uint64_t>> frames;
				const auto framesArray = currentState["frames"].GetArray();
				frames.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray) {
					const string subTexture = currentFrame["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint64();
					frames.emplace_back(pair<string, uint64_t>(subTexture, duration));
				}
				pAnimatedSprite->insertState(stateName, move(frames));
			}
		}
	}

	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd()) {
		for (const auto& currentLevel : levelsIt->value.GetArray()) {
			const auto description = currentLevel["description"].GetArray();
			vector<string> levelRows;
			levelRows.reserve(description.Size());
			size_t maxLength = 0;
			for (const auto& currentRow :description) {
				levelRows.emplace_back(currentRow.GetString());
				if (maxLength< levelRows.back().length()) {
					maxLength = levelRows.back().length();
				}
			}
			for (auto currentRow : levelRows) {
				while (currentRow.length() < maxLength) {
					currentRow.append("D");
				}
			}
			m_levels.emplace_back(move(levelRows));

		}
		return true;
	}



}

