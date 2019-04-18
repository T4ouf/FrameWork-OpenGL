#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

#include "Macros.h"


Shader::Shader(const std::string & chemin) : m_CheminFichier(chemin), m_RendererID(0){

	//On récupère les codes sources des shaders
	struct ShaderSource source = ParseShader(chemin);

	//on créee les shaders
	m_RendererID = CreationShader(source.VertexSource, source.FragmentSource);


}

Shader::~Shader(){

	//on détruit le shader (s'il a été créé)
	GLCALL(glDeleteProgram(m_RendererID));

}

//Fonction qui récupère le code source des shaders depuis un fichier
ShaderSource Shader::ParseShader(const std::string& chemin) {

	std::ifstream fichier(chemin);
	std::stringstream ss[2];

	enum class TypeShader
	{
		VIDE = -1, VERTEX = 0, FRAGMENT = 1
	};

	TypeShader type = TypeShader::VIDE;

	std::string ligne;

	//Parcourt du fichier
	while (getline(fichier, ligne)) {

		//Si on arrive à un délimiteur de shader
		if (ligne.find("#shader") != std::string::npos) {

			if (ligne.find("vertex") != std::string::npos) {
				type = TypeShader::VERTEX;
			}
			else if (ligne.find("fragment") != std::string::npos) {
				type = TypeShader::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << ligne << "\n";
		}

	}


	return { ss[0].str(), ss[1].str() };

}

//Cette fonction va prendre le code source d'un shader pour le compiler
//Source est le code source du shader, type est le type du shader (GL_ENUM)
unsigned int Shader::CompileShader(unsigned int type, std::string& source) {

	GLCALL(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	//type shader, nombre de shader, adresse du code source, longueur données (si NULL= données se terminent par NULL)
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	//GESTION D'ERREUR
	//Verif que la compilation s'est bien passée
	int res;
	//iv pour integer vector --> renvoit un tableau d'entier pour savoir si compile ok
	glGetShaderiv(id, GL_COMPILE_STATUS, &res);

	//Si pb ==> on renvoit un message d'erreur
	if (res == GL_FALSE) {

		int longueur;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longueur);
		char* message = (char*)alloca(longueur * sizeof(char));

		glGetShaderInfoLog(id, longueur, &longueur, message);

		std::cout << "ERREUR COMPILATION " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ")
			<< "SHADER !\n" << message << '\n';

		glDeleteShader(id);
		return 0;
	}

	return id;

}

//Cette fonction va prendre le code source des shaders pour les compiler tous
unsigned int Shader::CreationShader(std::string& vertexShader, std::string& fragmentShader) {

	//On crée un programme qui va accueillir le code source
	GLCALL(unsigned int programme = glCreateProgram());

	//Utilise la fonction précedente
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//On lie un programme et un shader
	GLCALL(glAttachShader(programme, vs));
	GLCALL(glAttachShader(programme, fs));

	// On lie le programme au code actuel
	GLCALL(glLinkProgram(programme));
	// On valide le programme
	GLCALL(glValidateProgram(programme));

	//Une fois liés, on n'a plus besoin des shaders
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return programme;
}

void Shader::Bind() const {

	//On spécifie à OpenGL quel shader utiliser
	GLCALL(glUseProgram(m_RendererID));

}

void Shader::UnBind() const {
	//on n'utilise plus notre shader (on l'unbind)
	GLCALL(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string & nom){

	//Si la position de l'uniform est dans le cache => on la renvoit
	if (m_UniformLocationCache.find(nom) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[nom];
	}

	//sinon on la recherche (dans le shader)
	GLCALL(int location = glGetUniformLocation(m_RendererID, nom.c_str()));

	//si pas trouvee
	if (location == -1) {
		std::cerr << "Uniform " << nom << " non utilise ou non declare (introuvable) !\n";
	}
	
	//on stocke la position dans le cache (dans tous les cas)
	m_UniformLocationCache[nom] = location;

	/* //si trouvee...
	else {
		//on stocke la position dans le cache
		m_UniformLocationCache[nom] = location;
	}
	*/
	return location;
}

void Shader::SetUniform1i(const std::string & nom, int i0) {

	GLCALL(glUniform1i(GetUniformLocation(nom), i0));
}


void Shader::SetUniform1f(const std::string & nom, float f0) {

	GLCALL(glUniform1f(GetUniformLocation(nom), f0));
}

void Shader::SetUniform3f(const std::string & nom, float f0, float f1, float f2) {

	GLCALL(glUniform3f(GetUniformLocation(nom), f0, f1, f2));
}

void Shader::SetUniform4f(const std::string & nom, float f0, float f1, float f2, float f3){

	GLCALL(glUniform4f(GetUniformLocation(nom), f0, f1, f2, f3));
}

void Shader::SetUniformMat4f(const std::string & nom, const glm::mat4& matrice) {

	GLCALL(glUniformMatrix4fv(GetUniformLocation(nom),1,GL_FALSE, &matrice[0][0]));
}

Shader* ShaderMap::getShader(ShaderName shader){

	if (firstIn) {
		firstIn = false;
		map = { 
			{ LightSourceShader, new Shader("Ressources/Shaders/LightSource.shader") },
			{ MaterialShader, new Shader("Ressources/Shaders/MaterialLumiere.shader")} 
		};
	}

	return map.at(shader);
}
