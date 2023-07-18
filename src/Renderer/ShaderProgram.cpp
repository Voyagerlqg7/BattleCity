#include"ShaderProgram.h"
#include<glm/gtc/type_ptr.hpp>
#include <iostream>

namespace RenderEngine {
	ShaderProgram::ShaderProgram(const string& vertexShader, const string& fragmentShader) {
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
			cout << "Vertex Shader compile time error" << endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			cout << "Fragment Shader compile time error" << endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infolog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infolog);
			cout << "Error: Shader LINK time error\n" << infolog << endl;
			
		}
		else {
			m_isCompiled = true;
		}
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

	}
	bool ShaderProgram::createShader(const string& source, const GLenum ShaderType, GLuint& shaderID) {
		shaderID = glCreateShader(ShaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);


		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infolog[1024];
			glGetShaderInfoLog(shaderID,1024,nullptr, infolog);
			cout << "Error: Shader compile time error\n" << infolog <<endl;
			return false;
		}
		return true;
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ID);
	}
	void ShaderProgram::use() const {
		glUseProgram(m_ID);
	}

	ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}
	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}
	void ShaderProgram::setInt(const string& name, const GLint value) {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void ShaderProgram::setFloat(const string& name, const GLfloat value) {
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}


	void ShaderProgram::setMatrix4(const string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1,GL_FALSE, glm::value_ptr(matrix));
	}

}