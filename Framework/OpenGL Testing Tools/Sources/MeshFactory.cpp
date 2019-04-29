#include "MeshFactory.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <sstream>
#include <string>



/* MODEL LOADER (not done yet)
#include <iostream>

void MeshFactory::LoadOBJ(std::string path, std::vector<Vertex>& data) {

	std::ifstream file(path, std::ios::in);

	if (file) {                                          //Si on a réussi à ouvrir le fichier...
		std::cout << "Ouverture du fichier reussie !\n"; //On le dit
	}
	else {                                           //Sinon, on sort directement de la fonction
		std::cerr << "ERREUR ! Lecture impossible !\n";
		return;
	}


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<unsigned int> IndicesVertices, IndicesNormals, IndicesTexCoords;

	std::string line;

	while (std::getline(file, line)){

		std::istringstream reader(line.substr(2));

		if (line.at(0) == 'v' && line.at(1) == ' ') {
			float x, y, z;
			reader >> x >> y >> z;
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (line.at(0) == 'v' && line.at(1) == 't') {
			float u,v;
			reader >> u >> v;
			texCoords.push_back(glm::vec2(u,v));
		}
		else if (line.at(0) == 'v' && line.at(1) == 'n') {
			float x, y, z;
			reader >> x >> y >> z;
			normals.push_back(glm::vec3(x, y, z));
		}
		else if (line.at(0) == 'f' && line.at(1) == ' '){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			reader >> vertexIndex[0] >>	vertexIndex[1] >>vertexIndex[2];

			reader >> uvIndex[0] >> uvIndex[1] >> uvIndex[2];

			reader >> normalIndex[0] >> normalIndex[1] >> normalIndex[2];

			IndicesVertices.push_back(vertexIndex[0]-1);
			IndicesVertices.push_back(vertexIndex[1]-1);
			IndicesVertices.push_back(vertexIndex[2]-1);
			IndicesTexCoords.push_back(uvIndex[0]-1);
			IndicesTexCoords.push_back(uvIndex[1]-1);
			IndicesTexCoords.push_back(uvIndex[2]-1);
			IndicesNormals.push_back(normalIndex[0]-1);
			IndicesNormals.push_back(normalIndex[1]-1);
			IndicesNormals.push_back(normalIndex[2]-1);



		}
	}

	const unsigned int size = IndicesVertices.size();

	data.clear();	//clear just in case

	// For each vertex of each triangle
	for (unsigned int i = 0; i < IndicesVertices.size(); i++) {

		unsigned int vertexIndex = IndicesVertices[i];
		unsigned int normalIndex = IndicesNormals[i];
		unsigned int uvIndex	 = IndicesTexCoords[i];

		glm::vec3 vertex = vertices.at(vertexIndex);
		glm::vec3 normal = normals.at(normalIndex);
		glm::vec2 uv = texCoords.at(uvIndex);

		data.push_back({ vertex,normal, uv });
	}

}
*/

Mesh * MeshFactory::CreateQuad(float x, float y, float width, float height, Material * material) {

	struct Vertex2D {
		glm::vec3 position;
		glm::vec2 uv;
	};

	Vertex2D data[4];

	data[0].position = glm::vec3(x, y, 0);
	data[0].uv = glm::vec2(0, 1);

	data[1].position = glm::vec3(x + width, y, 0);
	data[1].uv = glm::vec2(0, 0);

	data[2].position = glm::vec3(x + width, y + height, 0);
	data[2].uv = glm::vec2(1, 0);

	data[3].position = glm::vec3(x, y + height, 0);
	data[3].uv = glm::vec2(1, 1);


	VertexBuffer* vb = new VertexBuffer(data, sizeof(Vertex2D) * 4);

	VertexBufferLayout layout;
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec2>(1);


	VertexArray* va = new VertexArray();
	va->AddBuffer(*vb, layout);
	unsigned int* indices = new unsigned int[6]{ 0, 1, 2, 2, 3, 0, };
	IndexBuffer* ib = new IndexBuffer(indices, 6);

	return new Mesh(va, ib, material);

}

Mesh * MeshFactory::CreateQuad(const glm::vec2 & position, const glm::vec2 & size, Material * material) {
	return nullptr;
}

Mesh * MeshFactory::CreateBoxe(float Xsize,float Ysize, float Zsize, Material* material) {
	Vertex data[36];

	memset(data, 0, 36 * sizeof(Vertex));

	//back face (plane z = -1)
	data[0].position = glm::vec3(-Xsize, -Ysize, -Zsize);
	data[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[0].uv = glm::vec2(0.0f, 0.0f);

	data[1].position = glm::vec3(Xsize, -Ysize, -Zsize);
	data[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[1].uv = glm::vec2(1.0f, 0.0f);

	data[2].position = glm::vec3(Xsize, Ysize, -Zsize);
	data[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[2].uv = glm::vec2(1.0f, 1.0f);

	data[3].position = glm::vec3(Xsize, Ysize, -Zsize);
	data[3].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[3].uv = glm::vec2(1.0f, 1.0f);

	data[4].position = glm::vec3(-Xsize, Ysize, -Zsize);
	data[4].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[4].uv = glm::vec2(0.0f, 1.0f);

	data[5].position = glm::vec3(-Xsize, -Ysize, -Zsize);
	data[5].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[5].uv = glm::vec2(0.0f, 0.0f);

	data[6].position = glm::vec3(-Xsize, -Ysize, Zsize);
	data[6].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[6].uv = glm::vec2(0.0f, 0.0f);

	data[7].position = glm::vec3(Xsize, -Ysize, Zsize);
	data[7].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[7].uv = glm::vec2(1.0f, 0.0f);

	data[8].position = glm::vec3(Xsize, Ysize, Zsize);
	data[8].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[8].uv = glm::vec2(1.0f, 1.0f);

	data[9].position = glm::vec3(Xsize, Ysize, Zsize);
	data[9].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[9].uv = glm::vec2(1.0f, 1.0f);

	data[10].position = glm::vec3(-Xsize, Ysize, Zsize);
	data[10].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[10].uv = glm::vec2(0.0f, 1.0f);

	data[11].position = glm::vec3(-Xsize, -Ysize, Zsize);
	data[11].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[11].uv = glm::vec2(0.0f, 0.0f);

	data[12].position = glm::vec3(-Xsize, Ysize, Zsize);
	data[12].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[12].uv = glm::vec2(1.0f, 0.0f);

	data[13].position = glm::vec3(-Xsize, Ysize, -Zsize);
	data[13].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[13].uv = glm::vec2(1.0f, 1.0f);

	data[14].position = glm::vec3(-Xsize, -Ysize, -Zsize);
	data[14].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[14].uv = glm::vec2(0.0f, 1.0f);

	data[15].position = glm::vec3(-Xsize, -Ysize, -Zsize);
	data[15].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[15].uv = glm::vec2(0.0f, 1.0f);

	data[16].position = glm::vec3(-Xsize, -Ysize, Zsize);
	data[16].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[16].uv = glm::vec2(0.0f, 0.0f);

	data[17].position = glm::vec3(-Xsize, Ysize, Zsize);
	data[17].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[17].uv = glm::vec2(1.0f, 0.0f);

	data[18].position = glm::vec3(Xsize, Ysize, Zsize);
	data[18].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[18].uv = glm::vec2(1.0f, 0.0f);

	data[19].position = glm::vec3(Xsize, Ysize, -Zsize);
	data[19].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[19].uv = glm::vec2(1.0f, 1.0f);

	data[20].position = glm::vec3(Xsize, -Ysize, -Zsize);
	data[20].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[20].uv = glm::vec2(0.0f, 1.0f);

	data[21].position = glm::vec3(Xsize, -Ysize, -Zsize);
	data[21].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[21].uv = glm::vec2(0.0f, 1.0f);

	data[22].position = glm::vec3(Xsize, -Ysize, Zsize);
	data[22].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[22].uv = glm::vec2(0.0f, 0.0f);

	data[23].position = glm::vec3(Xsize, Ysize, Zsize);
	data[23].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[23].uv = glm::vec2(1.0f, 0.0f);

	data[24].position = glm::vec3(-Xsize, -Ysize, -Zsize);
	data[24].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[24].uv = glm::vec2(0.0f, 1.0f);

	data[25].position = glm::vec3(Xsize, -Ysize, -Zsize);
	data[25].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[25].uv = glm::vec2(1.0f, 1.0f);

	data[26].position = glm::vec3(Xsize, -Ysize, Zsize);
	data[26].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[26].uv = glm::vec2(1.0f, 0.0f);

	data[27].position = glm::vec3(Xsize, -Ysize, Zsize);
	data[27].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[27].uv = glm::vec2(1.0f, 0.0f);

	data[28].position = glm::vec3(-Xsize, -Ysize, Zsize);
	data[28].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[28].uv = glm::vec2(0.0f, 0.0f);

	data[29].position = glm::vec3(-Xsize, -Ysize, -Zsize);
	data[29].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[29].uv = glm::vec2(0.0f, 1.0f);

	data[30].position = glm::vec3(-Xsize, Ysize, -Zsize);
	data[30].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[30].uv = glm::vec2(0.0f, 1.0f);

	data[31].position = glm::vec3(Xsize, Ysize, -Zsize);
	data[31].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[31].uv = glm::vec2(1.0f, 1.0f);

	data[32].position = glm::vec3(Xsize, Ysize, Zsize);
	data[32].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[32].uv = glm::vec2(1.0f, 0.0f);

	data[33].position = glm::vec3(Xsize, Ysize, Zsize);
	data[33].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[33].uv = glm::vec2(1.0f, 0.0f);

	data[34].position = glm::vec3(-Xsize, Ysize, Zsize);
	data[34].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[34].uv = glm::vec2(0.0f, 0.0f);

	data[35].position = glm::vec3(-Xsize, Ysize, -Zsize);
	data[35].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[35].uv = glm::vec2(0.0f, 1.0f);


	VertexBuffer* vb = new VertexBuffer(&data[0].position, 36 * sizeof(Vertex));

	VertexBufferLayout layout;
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec2>(1);

	VertexArray* va = new VertexArray();
	va->AddBuffer(*vb, layout);

	unsigned int* indices = new unsigned int[36]
	{
		0,1,2,	//Devant
		3,4,5,

		6,7,8,	//Gauche
		9,10,11,

		12,13,14,	//Derrière
		15,16,17,

		18,19,20,	//Dessous
		21,22,23,

		24,25,26,	//Droite
		27,28,29,

		30,31,32,	//Dessus
		33,34,35
	};

	IndexBuffer* ib = new IndexBuffer(indices, 36);
	return new Mesh(va, ib, material);

}

Mesh * MeshFactory::CreateCube(float size, Material* material) {
	Vertex data[36];

	memset(data, 0, 36 * sizeof(Vertex));

	//back face (plane z = -1)
	data[0].position = glm::vec3(-size, -size, -size);
	data[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[0].uv = glm::vec2(0.0f, 0.0f);

	data[1].position = glm::vec3(size, -size, -size);
	data[1].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[1].uv = glm::vec2(1.0f, 0.0f);

	data[2].position = glm::vec3(size, size, -size);
	data[2].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[2].uv = glm::vec2(1.0f, 1.0f);

	data[3].position = glm::vec3(size, size, -size);
	data[3].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[3].uv = glm::vec2(1.0f, 1.0f);

	data[4].position = glm::vec3(-size, size, -size);
	data[4].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[4].uv = glm::vec2(0.0f, 1.0f);

	data[5].position = glm::vec3(-size, -size, -size);
	data[5].normal = glm::vec3(0.0f, 0.0f, -1.0f);
	data[5].uv = glm::vec2(0.0f, 0.0f);

	data[6].position = glm::vec3(-size, -size, size);
	data[6].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[6].uv = glm::vec2(0.0f, 0.0f);

	data[7].position = glm::vec3(size, -size, size);
	data[7].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[7].uv = glm::vec2(1.0f, 0.0f);

	data[8].position = glm::vec3(size, size, size);
	data[8].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[8].uv = glm::vec2(1.0f, 1.0f);

	data[9].position = glm::vec3(size, size, size);
	data[9].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[9].uv = glm::vec2(1.0f, 1.0f);

	data[10].position = glm::vec3(-size, size, size);
	data[10].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[10].uv = glm::vec2(0.0f, 1.0f);

	data[11].position = glm::vec3(-size, -size, size);
	data[11].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	data[11].uv = glm::vec2(0.0f, 0.0f);

	data[12].position = glm::vec3(-size, size, size);
	data[12].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[12].uv = glm::vec2(1.0f, 0.0f);

	data[13].position = glm::vec3(-size, size, -size);
	data[13].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[13].uv = glm::vec2(1.0f, 1.0f);

	data[14].position = glm::vec3(-size, -size, -size);
	data[14].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[14].uv = glm::vec2(0.0f, 1.0f);

	data[15].position = glm::vec3(-size, -size, -size);
	data[15].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[15].uv = glm::vec2(0.0f, 1.0f);

	data[16].position = glm::vec3(-size, -size, size);
	data[16].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[16].uv = glm::vec2(0.0f, 0.0f);

	data[17].position = glm::vec3(-size, size, size);
	data[17].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	data[17].uv = glm::vec2(1.0f, 0.0f);

	data[18].position = glm::vec3(size, size, size);
	data[18].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[18].uv = glm::vec2(1.0f, 0.0f);

	data[19].position = glm::vec3(size, size, -size);
	data[19].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[19].uv = glm::vec2(1.0f, 1.0f);

	data[20].position = glm::vec3(size, -size, -size);
	data[20].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[20].uv = glm::vec2(0.0f, 1.0f);

	data[21].position = glm::vec3(size, -size, -size);
	data[21].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[21].uv = glm::vec2(0.0f, 1.0f);

	data[22].position = glm::vec3(size, -size, size);
	data[22].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[22].uv = glm::vec2(0.0f, 0.0f);

	data[23].position = glm::vec3(size, size, size);
	data[23].normal = glm::vec3(1.0f, 0.0f, 0.0f);
	data[23].uv = glm::vec2(1.0f, 0.0f);

	data[24].position = glm::vec3(-size, -size, -size);
	data[24].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[24].uv = glm::vec2(0.0f, 1.0f);

	data[25].position = glm::vec3(size, -size, -size);
	data[25].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[25].uv = glm::vec2(1.0f, 1.0f);

	data[26].position = glm::vec3(size, -size, size);
	data[26].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[26].uv = glm::vec2(1.0f, 0.0f);

	data[27].position = glm::vec3(size, -size, size);
	data[27].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[27].uv = glm::vec2(1.0f, 0.0f);

	data[28].position = glm::vec3(-size, -size, size);
	data[28].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[28].uv = glm::vec2(0.0f, 0.0f);

	data[29].position = glm::vec3(-size, -size, -size);
	data[29].normal = glm::vec3(0.0f, -1.0f, 0.0f);
	data[29].uv = glm::vec2(0.0f, 1.0f);

	data[30].position = glm::vec3(-size, size, -size);
	data[30].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[30].uv = glm::vec2(0.0f, 1.0f);

	data[31].position = glm::vec3(size, size, -size);
	data[31].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[31].uv = glm::vec2(1.0f, 1.0f);

	data[32].position = glm::vec3(size, size, size);
	data[32].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[32].uv = glm::vec2(1.0f, 0.0f);

	data[33].position = glm::vec3(size, size, size);
	data[33].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[33].uv = glm::vec2(1.0f, 0.0f);

	data[34].position = glm::vec3(-size, size, size);
	data[34].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[34].uv = glm::vec2(0.0f, 0.0f);

	data[35].position = glm::vec3(-size, size, -size);
	data[35].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	data[35].uv = glm::vec2(0.0f, 1.0f);


	VertexBuffer* vb = new VertexBuffer(&data[0].position, 36 * sizeof(Vertex));

	VertexBufferLayout layout;
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec2>(1);

	VertexArray* va = new VertexArray();
	va->AddBuffer(*vb, layout);

	unsigned int* indices = new unsigned int[36]
	{
		0,1,2,	//Devant
		3,4,5,

		6,7,8,	//Gauche
		9,10,11,

		12,13,14,	//Derrière
		15,16,17,

		18,19,20,	//Dessous
		21,22,23,

		24,25,26,	//Droite
		27,28,29,

		30,31,32,	//Dessus
		33,34,35
	};

	IndexBuffer* ib = new IndexBuffer(indices, 36);
	return new Mesh(va, ib, material);

}



Mesh * MeshFactory::CreateSphere(float radius, Material* material) {
	/*
	CE QUE L'ON A TESTE CE MATIN A L'ECOLE */
	/*
	// array is 71 floats (24 * 3 = 71).
	float deltaTheta, deltaPhi;

	int nbSlices = 100;
	int nbStacks = 100;

	// vertex, normal and texture coordinates

	float _radius = radius;
	//int _size = nbStacks * (nbSlices + 1) * 2;
	const int _size = 100 * (100 + 1) * 2;

	Vertex data[_size];

	// angular distance between two latitudinal lines
	deltaPhi = -M_PI / nbSlices;

	// angular distance between two longitudinal lines
	deltaTheta = 2.0f * M_PI / nbStacks;

	int k = 0;

	// triangle strips made of vertical stacks
	for (int i = 0; i < nbStacks; i++) {
	// azimuth angles of the two arcs defining the stack (longitudes)
	float theta = i * deltaTheta;
	float thetaPrime = theta + deltaTheta;

	// projection of the first theta in the horizontal plane
	float x0 = cos(theta);
	float y0 = sin(theta);

	// projection of the second theta in the horizontal plane
	float x1 = cos(thetaPrime);
	float y1 = sin(thetaPrime);

	// tesselating the triangle strip into nbSlices
	// note <= instead of < for making sure that the last angke is used
	for (int j = 0; j <= nbSlices; j++) {
	// polar angle
	float phi = j * deltaPhi;

	// polar vector in a vertical plane
	float xPolar = cos(phi);
	float yPolar = sin(phi);

	//INDICES A REVOIR

	// vertex #1 (theta , phi)
	data[k].position = glm::vec3(yPolar * x0 * radius, yPolar * y0 * radius, xPolar * radius); // per each ?
	data[k].normal = glm::vec3(yPolar * x0, yPolar*y0, xPolar);
	data[k].uv = glm::vec2((float)i / (float)nbStacks, (float)j / (float)nbSlices);

	// vertex #2 (theta , phiPrime)
	data[k+1].position = glm::vec3(yPolar * x1 * radius, yPolar * y1 * radius, xPolar * radius); // per each ?
	data[k+1].normal = glm::vec3(yPolar * x1, yPolar*y1, xPolar);
	data[k+1].uv = glm::vec2((float)(i + 1) / (float)nbStacks, (float)j / (float)nbSlices);

	k+=2;
	}
	}
	*/

	/* TEST D'UNE 2e METHODE  https://gamedev.stackexchange.com/questions/150191/opengl-calculate-uv-sphere-vertices*/

	//// One vertex at every latitude-longitude intersection,
	//// plus one for the north pole and one for the south.
	//// One meridian serves as a UV seam, so we double the vertices there.

	const int numLatitudeLines = 100;
	const int numLongitudeLines = 100;
	const int numVertices = numLatitudeLines * (numLongitudeLines + 1) + 2;

	Vertex data[numVertices];

	//// North pole.
	data[0].position = glm::vec3(0, radius, 0);
	data[0].uv = glm::vec2(0, 1);

	//// South pole.
	data[numVertices - 1].position = glm::vec3(0, -radius, 0);
	data[numVertices - 1].uv = glm::vec2(0, 0);

	//// +1.0f because there's a gap between the poles and the first parallel.
	float latitudeSpacing = 1.0f / (numLatitudeLines + 1.0f);
	float longitudeSpacing = 1.0f / (numLongitudeLines);

	//// start writing new vertices at position 1
	int v = 1;
	for (int latitude = 0; latitude < numLatitudeLines; latitude++) {
		for (int longitude = 0; longitude <= numLongitudeLines; longitude++) {




			if ((longitude + 1) % 3 == 0) {
				data[v + 100].uv = glm::vec2(longitude * longitudeSpacing, 1.0f - (latitude + 1) * latitudeSpacing);

				// Convert to spherical coordinates:
				// theta is a longitude angle (around the equator) in radians.
				// phi is a latitude angle (north or south of the equator).
				float theta = data[(latitude + 1)*(longitude + 1) + 2].uv.x * 2.0f * M_PI;
				float phi = (data[(latitude + 1)*(longitude + 1) + 2].uv.y - 0.5f) * M_PI;

				// This determines the radius of the ring of this line of latitude.
				// It's widest at the equator, and narrows as phi increases/decreases.
				float c = cos(phi);

				// Usual formula for a vector in spherical coordinates.
				// You can exchange x & z to wind the opposite way around the sphere.
				data[(latitude + 1)*(longitude + 1) + 2].position = glm::vec3(c * cos(theta), sin(phi), c * sin(theta)) * radius;
				data[(latitude + 1)*(longitude + 1) + 2].normal = glm::vec3(sin(phi) * cos(theta), sin(phi)*sin(theta), cos(phi));
			}
			else {
				// Scale coordinates into the 0...1 texture coordinate range,
				// with north at the top (y = 1).
				data[v].uv = glm::vec2(longitude * longitudeSpacing, 1.0f - (latitude + 1) * latitudeSpacing);

				// Convert to spherical coordinates:
				// theta is a longitude angle (around the equator) in radians.
				// phi is a latitude angle (north or south of the equator).
				float theta = data[v].uv.x * 2.0f * M_PI;
				float phi = (data[v].uv.y - 0.5f) * M_PI;

				// This determines the radius of the ring of this line of latitude.
				// It's widest at the equator, and narrows as phi increases/decreases.
				float c = cos(phi);

				// Usual formula for a vector in spherical coordinates.
				// You can exchange x & z to wind the opposite way around the sphere.
				data[v].position = glm::vec3(c * cos(theta), sin(phi), c * sin(theta)) * radius;
				data[v].normal = glm::vec3(sin(phi) * cos(theta), sin(phi)*sin(theta), cos(phi));
			}
			// Proceed to the next vertex.
			v++;

		}
	}
	
	/*
	std::vector<Vertex> sphere;

	if (SphereData == nullptr) {
		LoadOBJ("Ressources/objs/sphere.txt", sphere);
		SphereData = sphere.data();
		sphereVertices = sphere.size();
	}
	*/

	VertexBuffer* vb = new VertexBuffer(data, numVertices * sizeof(Vertex));

	VertexBufferLayout layout;
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec3>(1);
	layout.Push<glm::vec2>(1);

	VertexArray* va = new VertexArray();
	va->AddBuffer(*vb, layout);

	unsigned int* indices = new unsigned int[numVertices];
	for (int k = 0; k <= numVertices; k++) {
		indices[k] = k;
	}

	IndexBuffer* ib = new IndexBuffer(indices, numVertices);

	return new Mesh(va, ib, material);

}
