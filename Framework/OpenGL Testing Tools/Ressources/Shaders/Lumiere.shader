#shader vertex
#version 330 core

//On ecrit les shaders
//VertexShader ==> executé une fois par point
//FragmentShader ==> executé une fois par pixel

//on indique quel est l'id et le type d'entrée
layout(location = 0) in vec3 positions;
//layout(location = 1) in vec2 texCoords;
layout(location = 1) in vec3 normals;

//Varying => passer des données du VS au FS (les in et out)
//v_ pour varying
out vec3 v_Positions;
out vec2 v_TextureCoords;
out vec3 v_Normals;

out vec3 v_Light;
out vec3 v_View;

uniform mat4 u_ProjectionMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ModelMat;

uniform mat4 u_InverseModelMat;

uniform vec3 u_LightPos;

//uniform mat4 u_MVP;	//ModelViewProjection Matrice => Matrice de projection

void main() {

	// /!\ L'ordre est important dans un produit Matriciel /!\ 
	gl_Position = u_ProjectionMat * u_ViewMat * u_ModelMat * vec4(positions,1.0f);

	v_Positions = positions;
	//v_TextureCoords = texCoords;
	v_Normals = normals;

	//Changement de référenciel (transforms light from world coordinates to model coordinates)
	v_Light = (u_InverseModelMat * vec4(u_LightPos, 0.0)).xyz;

	v_View = vec3(u_ViewMat);
}






#shader fragment
#version 330 core

in vec3 v_Positions;
//in vec2 v_TextureCoords;
in vec3 v_Normals;

in vec3 v_Light;

in vec3 v_View;

//Ce qui sort du fragment shader (ici que la couleur à afficher)
layout(location = 0) out vec4 color;

//Ce que l'on prend comme variable (sous forme d'uniform)
uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;


void main() {

	//Vecteurs définis dans le plan partant du pixel à calculer
	vec3 L = normalize(v_Light - v_View);		//vecteur normalisé du pixel à la source de lumière
	vec3 V = normalize(-v_View);   //vecteur vue normalisé (du pixel à la position de la caméra)
	vec3 R = normalize(-reflect(L, v_Normals));	//vecteur rayon réfléchi

	// ambient
	float ambientStrength = 0.1f;
	vec3 Iambient = ambientStrength * u_LightColor;

	// diffuse 
	vec3 norm = normalize(v_Normals);

	float diff = max(dot(norm, L), 0.0);
	vec3 Idiffuse = diff * u_LightColor;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_ViewPos - v_Positions);
	vec3 reflectDir = reflect(-L, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 Ispecular = specularStrength * spec * u_LightColor;
	

	vec3 result = (Iambient + Idiffuse + Ispecular) * u_ObjectColor;

	color = vec4(result, 1.0f);

}