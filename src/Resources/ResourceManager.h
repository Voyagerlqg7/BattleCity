#pragma once
#include<string>
#include<memory>
#include<map>
namespace Renderer {
	class ShaderProgram;
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

private:
	string getFileString(const string& relativeFilePath) const;
	typedef map<const string, shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	string m_path;
};