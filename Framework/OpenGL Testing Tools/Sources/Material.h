#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"

class Material{
private:
	
	// A material is just a shader with some uniforms value
	Shader* m_shader;
	ShaderName m_shaderName;
	std::string m_shader_path;

	Texture* m_texture;
	std::string m_texture_path;
	
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	
	float m_shininess;

public:
	Material(Shader* shader, 
			 Texture* texture,
			 glm::vec3 ambient,
			 glm::vec3 diffuse,
			 glm::vec3 specular,
			 float shininess);

	Material(Shader* shader,
		std::string texture_path,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float shininess);

	Material(std::string shader_path,
		std::string texture_path,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float shininess);
	
	//// END OF DEPRECATED STUFF

	Material(ShaderName shader,
		std::string texture_path,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float shininess);

	~Material();

	Shader* getShader();

	void AttachTexture(Texture* texture);
	void AttachTexture(std::string texture_path);

	void Bind() const;

	void UnBind() const;

	ShaderName getShaderName() { return m_shaderName; }

};
