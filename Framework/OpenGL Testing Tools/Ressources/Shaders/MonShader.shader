#shader vertex
#version 330 core

//On ecrit les shaders
//VertexShader ==> executé une fois par point
//FragmentShader ==> executé une fois par pixel

//on indique quel est l'id et le type d'entrée
layout(location = 0) in vec4 position;

uniform mat4 u_MVP;	//ModelViewProjection Matrice => Matrice de projection

void main() {

	// /!\ L'ordre est important dans un produit Matriciel /!\ 
	gl_Position = u_MVP * position;

}


#shader fragment
#version 330 core

//Ce qui sort du fragment shader (ici que la couleur à afficher)
layout(location = 0) out vec4 color;

//Ce que l'on prend comme variable (sous forme d'uniform)
uniform vec4 u_Color;

void main() {
	color = u_Color;
}