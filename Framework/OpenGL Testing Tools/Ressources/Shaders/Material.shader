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
out vec3 v_Positions;
out vec3 v_Normals;
out vec2 v_TextureCoords;

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
	v_Normals = normals;
	v_TextureCoords = texCoords;

	//Changement de référenciel (transforms light from world coordinates to model coordinates)
	v_Light = (u_InverseModelMat * vec4(u_LightPos, 0.0)).xyz;

	v_View = vec3(u_ViewMat);
}




#shader fragment
#version 330 core

in vec3 v_Positions;
in vec3 v_Normals;
in vec2 v_TextureCoords;

in vec3 v_Light;

in vec3 v_View;

//Ce qui sort du fragment shader (ici que la couleur à afficher)
layout(location = 0) out vec4 color;

//Ce que l'on prend comme variable (sous forme d'uniform)

//Material management
uniform vec3 u_Ambient;
uniform vec3 u_Diffuse;
uniform vec3 u_Specular;
uniform float u_Shininess;

uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;

uniform sampler2D u_Texture;
uniform int u_isTextured;

void main() {

	vec4 texColor;

	//if the object is textured
	if (u_isTextured == 1) {
		texColor = texture(u_Texture, v_TextureCoords);
		//color = vec4(v_TextureCoords, 0.0f, 1.0f);
	}
	else {
		texColor = vec4(1.0f);
	}
	
	//Vecteurs définis dans le plan partant du pixel à calculer
	vec3 L = normalize(v_Light - v_View);		//vecteur normalisé du pixel à la source de lumière
	vec3 V = normalize(-v_View);   //vecteur vue normalisé (du pixel à la position de la caméra)
	vec3 R = normalize(-reflect(L, v_Normals));	//vecteur rayon réfléchi

												// ambient
	float ambientStrength = 0.2f;
	vec3 Iambient = ambientStrength * u_LightColor * u_Ambient;

	// diffuse 
	vec3 norm = normalize(v_Normals);

	float diff = max(dot(norm, L), 0.0f);
	vec3 Idiffuse = diff * u_LightColor * u_Diffuse;

	// specular
	vec3 viewDir = normalize(u_ViewPos - v_Positions);
	vec3 reflectDir = reflect(-L, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Shininess);
	vec3 Ispecular = u_LightColor * (spec * u_Specular);

	vec3 result = Iambient + Idiffuse + Ispecular;

	//color = texColor;
	//color = vec4(v_TextureCoords.x, v_TextureCoords.y,0.0f,1.0f);
	color = vec4(result, 1.0f) * texColor;
	
	

}