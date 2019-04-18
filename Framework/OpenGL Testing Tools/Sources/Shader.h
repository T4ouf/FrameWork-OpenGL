#pragma once

#include <string>

//include map (= HashMap et HashTable)
#include <unordered_map>

#include "glm/glm.hpp"

typedef enum {

	LightSourceShader,
	MaterialShader

}ShaderName;

//Strucuture pour que la fonction ParseShader puisse renvoyer 2 valeurs de retour
struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {

private:
	
	std::string m_CheminFichier;
	unsigned int m_RendererID;

	//cache pour stocker les uniforms (pas faire des getLocation tout le temps car lent...)
	std::unordered_map<std::string, int>m_UniformLocationCache;

	//fonction pour retrouver notre uniform
	int GetUniformLocation(const std::string& nom);

	//Parse le shader
	ShaderSource Shader::ParseShader(const std::string& chemin);
	//Compile le shader
	unsigned int CompileShader(unsigned int type, std::string& source);
	//Creation du shader
	unsigned int CreationShader(std::string& vertexShader, std::string& fragmentShader);

public:

	//Constructeur
	Shader(const std::string& chemin);
	//Destructeur
	~Shader();

	//Ici en openGL, Bind s'appelle UseProgram,
	//pour être cohérent avec les autres classes on appellera les fonctions bind et unbind
	//Pour lier notre shader au renderer
	void Bind() const;

	//délier notre shader au renderer
	void UnBind() const;


	//Ajouter des Uniforms à notre shader
	void SetUniform1i(const std::string& nom, int i0);
	void SetUniform1f(const std::string& nom, float f0);
	void SetUniform3f(const std::string& nom, float f0, float f1, float f2);
	void SetUniform4f(const std::string& nom, float f0, float f1, float f2, float f3);
	void SetUniformMat4f(const std::string& nom, const glm::mat4& matrice);
};

//Struct that maps one of our shader with an existing shader
namespace ShaderMap {

	//Anonymous namespace makes the map "invisible" from the outside
	namespace {
		
		static bool firstIn = true;

		std::unordered_map<ShaderName, Shader*> map;
	}
	
	Shader* getShader(ShaderName shader);

};
