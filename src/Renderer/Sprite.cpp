#include "Sprite.h"
#include"ShaderProgram.h"
#include"VertexArray.h"
#include"VertexBuffer.h"
#include"Renderer.h"
#include"Texture2D.h"
#include<glm/mat4x4.hpp>
#include<glm/vec2.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "GLFW/glfw3.h"
#include<string>




using namespace std;

namespace RenderEngine {
	Sprite::Sprite(const shared_ptr<Texture2D> pTexture,
		const string initialSubTexture,
		const shared_ptr<ShaderProgram> pShaderProgram) 
		: m_pTexture(move(pTexture))
		, m_pShaderProgram(move(pShaderProgram))
	{
		const GLfloat vertexCoords[] = {
		    
			//1----2
			//|  / |
			//| /  |
			//0----3

			//x-----y
			0.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,
			1.f, 0.f,

		};
		auto subTexture = m_pTexture->getSubTexture(move(initialSubTexture));
		const GLfloat textureCoords[] = {
			//U-----V
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.leftBottomUV.y,

		};
		const GLuint indexes[] = {
			//U-----V
			0,1,2,
			2,3,0

		};
	

		

		m_vertexCoordsBuffer.init(vertexCoords, 2*4*sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
		
		
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.init(indexes, 6);
		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	Sprite::~Sprite() {

	}
	void Sprite::render(const glm::vec2& position, const glm::vec2& size, const float rotation) const {
		m_pShaderProgram->use();
		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
		model = glm::scale(model, glm::vec3(size, 1.f));
		m_pShaderProgram->setMatrix4("modelMat", model);
		glActiveTexture(GL_TEXTURE);
		m_pTexture->bind();

		Renderer::draw(m_vertexArray,m_indexBuffer,*m_pShaderProgram);

	}
}