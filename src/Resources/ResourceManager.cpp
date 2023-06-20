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


ResourceManager::ResourceManager(const string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}
string ResourceManager::getFileString(const string& relativeFilePath) const {
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

shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath)
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

	shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Cant load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}


shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it !=m_shaderPrograms.end()) {
		return it->second;
	}
	
	cout << "Cant find the shader program"<<shaderName << endl;
	return nullptr;
	
}
shared_ptr<Renderer::Texture2D> ResourceManager::loadTextures(const string& textureName, const string& texturePath) {
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char*pixels =  stbi_load(string(m_path + "/" + texturePath).c_str(), &width, &height, & channels, 0);

	if (!pixels) {
		cout << "Cant load image" << texturePath <<endl;
		return nullptr;
	}
	shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName, make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return newTexture;
}

shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}

	cout << "Cant find the texture " << textureName << endl;
	return nullptr;
}
shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const string& spriteName,
													     const string& textureName,
														 const string& shaderName,
														 const unsigned int spriteWidth,
														 const unsigned int spriteHeight) {

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
	shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(textureName,
		make_shared<Renderer::Sprite>(
		pTexture,
		pShader, glm::vec2(0.f, 0.f),
		glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;

}
shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const string& spriteName) {
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end()) {
		return it->second;
	}

	cout << "Cant find the sprite " << spriteName << endl;
	return nullptr;
}


