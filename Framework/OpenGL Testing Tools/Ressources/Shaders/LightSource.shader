#shader vertex
#version 330 core

//On ecrit les shaders
//VertexShader ==> executé une fois par point
//FragmentShader ==> executé une fois par pixel

//on indique quel est l'id et le type d'entrée
layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

//Varying => passer des données du VS au FS (les in et out)
//v_ pour varying

uniform mat4 u_ProjectionMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ModelMat;

void main() {

	// /!\ L'ordre est important dans un produit Matriciel /!\ 
	gl_Position = u_ProjectionMat * u_ViewMat * u_ModelMat * vec4(positions, 1.0f);

}


#shader fragment
#version 330 core

//Ce qui sort du fragment shader (ici que la couleur à afficher)
layout(location = 0) out vec4 color;

//Ce que l'on prend comme variable (sous forme d'uniform)
uniform vec4 u_LightColor;

uniform sampler2D u_Texture;
uniform int u_isTextured;

void main() {
	
	color = u_LightColor;

}