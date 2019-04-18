#include "Material.h"

/**Deprecated version*/
Material::Material(Shader* shader,Texture* texture, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess):
m_shader(shader), m_texture(texture), m_ambient(ambient), m_diffuse(diffuse),m_specular(specular), m_shininess(shininess){
	
	m_shaderName = MaterialShader;
	m_texture_path = "";
	m_shader_path = "";
}

Material::Material(Shader * shader, std::string texture_path, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	m_shader(shader), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {

	m_shader_path = "";
	m_shaderName = MaterialShader;

	m_texture = new Texture(texture_path);
	m_texture_path = texture_path;
}

Material::Material(std::string shader_path, std::string texture_path, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {

	m_shader = new Shader(shader_path);
	m_shader_path = shader_path;
	m_shaderName = MaterialShader;

	m_texture = new Texture(texture_path);
	m_texture_path = texture_path;
}
//////////END OF Deprecated 


Material::Material(ShaderName shader, std::string texture_path, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : 
	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {

	m_shader = ShaderMap::getShader(shader);
	m_shader_path = "";
	m_shaderName = shader;

	m_texture = new Texture(texture_path);
	m_texture_path = texture_path;

}

Material::~Material(){
	if (m_texture_path != "") {
		m_texture->UnBind();
		delete m_texture;
	}

	if (m_shader_path != "") {
		m_shader->UnBind();
		delete m_shader; //We owe the shader ONLY IF the path constructor has been use
	}
}

Shader* Material::getShader(){
	return m_shader;
}

void Material::AttachTexture(Texture* texture){

	if (m_texture_path != "") {
		m_texture->UnBind();
		delete m_texture;
	}

	m_texture = texture;
	m_texture_path = "";
}

void Material::AttachTexture(std::string texture_path){

	if (m_texture_path != "") {
		m_texture->UnBind();
		delete m_texture;
	}

	m_texture = new Texture(texture_path);
	m_texture_path = texture_path;

}

void Material::Bind() const{

	m_shader->Bind();

	//Setting up the right uniforms
	if (m_shaderName == MaterialShader) {
	
		m_shader->SetUniform3f("u_Ambient", m_ambient.r, m_ambient.g, m_ambient.b);
		m_shader->SetUniform3f("u_Diffuse", m_diffuse.r, m_diffuse.g, m_diffuse.b);
		m_shader->SetUniform3f("u_Specular", m_specular.r, m_specular.g, m_specular.b);
		m_shader->SetUniform1f("u_Shininess", m_shininess);

	}
	else if(m_shaderName == LightSourceShader){

	}

	if (m_texture != nullptr) {
		m_texture->Bind(0);
		m_shader->SetUniform1i("u_Texture", 0);
		m_shader->SetUniform1i("u_isTextured", 1);
	}
	else {
		//the material si not textured
		m_shader->SetUniform1i("u_isTextured", 0);
	}


}

void Material::UnBind() const{
	m_shader->UnBind();
	m_texture->UnBind();
}



