#include "CityGenerator.h"

#include <vector>
#include <string>
#include <iostream>
#include <CityGenerator/Building.h>
#include <Core/Engine.h>

using namespace std;
glm::vec3 old;
int numTree = 0;
vector<int> trees;

// VS
float materialShininess = 80;
float materialKd = 0.6;
float materialKs = 0.6;

// dimension and variables that can change form of the city
const int width = 30, height = 30;
const int maxTunnels = 50, maxLength = 20;
int city[width][height];


vector<int> roadsDirection[width][height];
vector<vector<int>> directions = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} };
vector<Primitive*> primitive;
vector<Building*> buildings;
std::vector<std::string> texts;

CityGenerator::CityGenerator()
{
}

CityGenerator::~CityGenerator()
{
}

bool isGoodDirection(vector<int> dir, int i, int j, vector<int> lastDirection) {
	// the new direction is valid or not 
	int newI = i + dir[0], newJ = j + dir[1];
	if (newI < width && newI >= 0 && newJ < height && newJ >= 0) {
		if (dir[0] != -1 * lastDirection[0] || dir[1] != -1 * lastDirection[1]) {
			return true;
		}
	}
	return false;
}

bool isIntersection(int i, int j) {
	if (i > 0 && j > 0) {
		if (i > 0 && j > 0) {
			if (city[i - 1][j] == 1 && roadsDirection[i - 1][j][0] == 0
				&& roadsDirection[i - 1][j][1] == 1) {
				return true;
			}
			if (city[i][j - 1] == 1 && roadsDirection[i][j - 1][0] == 0
				&& roadsDirection[i][j - 1][1] == 1) {
				return true;
			}
			if (city[i + 1][j] == 1 && roadsDirection[i + 1][j][0] == 0
				&& roadsDirection[i + 1][j][1] == 1) {
				return true;
			}
			if (city[i][j + 1] == 1 && roadsDirection[i][j + 1][0] == 0
				&& roadsDirection[i][j + 1][1] == 1) {
				return true;
			}
		}
	}
}

vector<int> neighbors(int i, int j) {
	int n = 0;
	vector<int> ne;
	if (i > 0 && j > 0) {
		if (city[i - 1][j] == 1) { n++; ne.push_back(0); }
		if (city[i][j - 1] == 1) { n++; ne.push_back(3); }
		if (city[i + 1][j] == 1) { n++; ne.push_back(1); }
		if (city[i][j + 1] == 1) { n++; ne.push_back(2); }
	}
	else {
		if (i == 0 && j == 0) {
			if (city[i + 1][j] == 1) { n++; ne.push_back(1); }
			if (city[i][j + 1] == 1) { n++; ne.push_back(2); }
		}
		else if (i == height && j == width) {
			if (city[i - 1][j] == 1) { n++; ne.push_back(0); }
			if (city[i][j - 1] == 1) { n++; ne.push_back(3); }
		}
		else if (i == 0 && j == width - 1) {
			if (city[i][j - 1] == 1) { n++; ne.push_back(3); }
			if (city[i + 1][j] == 1) { n++; ne.push_back(1); }
		}
		else if (i == height - 1 && j == 0) {
			if (city[i - 1][j] == 1) { n++; ne.push_back(0); }
			if (city[i][j + 1] == 1) { n++; ne.push_back(2); }
		}
		else if (i == 0) {
			if (city[i][j - 1] == 1) { n++; ne.push_back(3); }
			if (city[i + 1][j] == 1) { n++; ne.push_back(1); }
			if (city[i][j + 1] == 1) { n++; ne.push_back(2); }
		}
		else if (j == width - 1) {
			if (city[i - 1][j] == 1) { n++; ne.push_back(0); }
			if (city[i][j - 1] == 1) { n++; ne.push_back(3); }
			if (city[i + 1][j] == 1) { n++; ne.push_back(1); }
		}
		else if (i == height - 1) {
			if (city[i - 1][j] == 1) { n++; ne.push_back(0); }
			if (city[i][j - 1] == 1) { n++; ne.push_back(3); }
			if (city[i][j + 1] == 1) { n++; ne.push_back(2); }
		}
		else if (j == 0) {
			if (city[i - 1][j] == 1) { n++; ne.push_back(0); }
			if (city[i + 1][j] == 1) { n++; ne.push_back(1); }
			if (city[i][j + 1] == 1) { n++; ne.push_back(2); }
		}
	}
	return ne;
}

int diagNeighbors(int i, int j) {
	int n = 0;
	if (city[i + 1][j + 1] == 1) n++; 
	if (city[i + 1][j - 1] == 1) n++;
	if (city[i - 1][j + 1] == 1) n++;
	if (city[i - 1][j - 1] == 1) n++;
	return n;
}

bool continueRoad(vector<int> n, int i, int j){
	if ((directions[n[0]][0] == directions[n[1]][0] &&
			directions[n[0]][1] == directions[n[1]][1]) ||
			(directions[n[0]][0] == -1 * directions[n[1]][0] &&
				directions[n[0]][1] == -1 * directions[n[1]][1])) {
		return true;
	}
	return false;

}

int whatsMissing(vector<int> n, int i, int j) {
	std::sort(n.begin(), n.end());
	for (int i = 0; i < n.size(); i++)
	{
		if (i == n[i])
			continue;
		else
			return n[i];
	}

}

bool isSpaceFor4(int i, int j) {
	if (city[i + 1][j] == 0 &&
		city[i + 1][j + 1] == 0 &&
		city[i][j + 1] == 0) {
		return true;
	}
	return false;
}

Building* generateBuilding(std::vector<Primitive*> vec, glm::vec3 pos) {
	Building* v = new Building();
	float newHeight = 0;
	int rand = std::rand() % 2;
	string text1 = texts[rand];
	string text2 = texts[rand + 2];
	string text3 = texts[rand + 4];

	for (int i = 0; i < vec.size(); i++)
	{
		vec[i]->pos = pos;
		if (vec[i]->type != 1) {
			vec[i]->texture = text1;
			vec[i]->texture2 = text2;
		}
		else {
			vec[i]->texture = text3;
		}
		
		if (i > 0) {
			vec[i]->pos += glm::vec3(0, newHeight, 0);

			float x, y, z;
			int x1, y1, z1;
			x1 = vec[i - 1]->scale.x * 50;
			x = (std::rand() % x1) + vec[i - 1]->scale.x * 50;
			x /= 100;
			y1 = vec[i - 1]->scale.y * 50;
			y = (std::rand()) % y1 + vec[i - 1]->scale.y * 50;
			y /= 100;
			z1 = vec[i - 1]->scale.z * 50;
			z = (std::rand()) % z1 + vec[i - 1]->scale.z * 50;
			z /= 100;

			if (vec[i - 1]->type == 2 && vec[i]->type != 2) {
				x *= 0.8;
				z *= 0.8;
			}

			vec[i]->scale = glm::vec3(x, y, z);
			vec[i]->dim = glm::vec3(vec[i]->dim[0] * x, vec[i]->dim[1] * y, vec[i]->dim[2] * z);
		}
		else {
			float x, y, z;
			int x1, y1, z1;
			int aux1 = 30, aux2 = 70;
			if (vec[i]->type == 1) {
				aux1 = 50;
				aux2 = 50;
			}
			x1 = vec[i]->scale.x * aux1;
			x = (std::rand() % x1) + vec[i]->scale.x * aux2;
			x /= 100;
			y1 = vec[i]->scale.y * aux1;
			y = (std::rand()) % y1 + vec[i]->scale.y * 90;
			y /= 100;
			z1 = vec[i]->scale.z * aux1;
			z = (std::rand()) % z1 + vec[i]->scale.z * aux2;
			z /= 100;

			vec[i]->scale = glm::vec3(x, y, z);
			vec[i]->dim = glm::vec3(vec[i]->dim[0] * x, vec[i]->dim[1] * y, vec[i]->dim[2] * z);
		}

		vec[i]->pos[1] += vec[i]->dim[1] / 2;
		newHeight += vec[i]->dim[1];		
	}
	v->primitive = vec;
	v->dim[0] = vec[0]->dim[0];
	v->dim[2] = vec[0]->dim[2];
	v->dim[1] = newHeight;
	v->pos = vec[0]->pos;
	return v;
}

void CityGenerator::Init()
{
	const string textureLoc = "Source/CityGenerator/Textures/";
	old = GetSceneCamera()->transform->GetLocalOZVector();

	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "road3.jpg").c_str(), GL_REPEAT);
		mapTextures["road"] = texture;
		
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "wood2.png").c_str(), GL_REPEAT);
			mapTextures["wood2"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "wood.png").c_str(), GL_REPEAT);
			mapTextures["wood"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "lamp.png").c_str(), GL_REPEAT);
			mapTextures["lamp"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "tiles.jpg").c_str(), GL_REPEAT);
			mapTextures["tiles"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "ceva6.jpg").c_str(), GL_REPEAT);
			mapTextures["glass6"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "ceva5.jpg").c_str(), GL_REPEAT);
			mapTextures["glass5"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "ceva.jpg").c_str(), GL_REPEAT);
			mapTextures["glass"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "ceva4.jpg").c_str(), GL_REPEAT);
			mapTextures["glass2"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "ceva2.jpg").c_str(), GL_REPEAT);
			mapTextures["glass3"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "ceva3.jpg").c_str(), GL_REPEAT);
			mapTextures["glass4"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "roadSimple.jpg").c_str(), GL_REPEAT);
			mapTextures["roadSimple"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "roadInt.jpg").c_str(), GL_REPEAT);
			mapTextures["roadInt"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "roadJustLeft.jpg").c_str(), GL_REPEAT);
			mapTextures["roadJustLeft"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "roadNoDown.jpg").c_str(), GL_REPEAT);
			mapTextures["roadNoDown"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "grass.jpg").c_str(), GL_REPEAT);
			mapTextures["grass"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "grass2.jpg").c_str(), GL_REPEAT);
			mapTextures["grass2"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "bird.jpg").c_str(), GL_REPEAT);
			mapTextures["bird"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "CROW.png").c_str(), GL_REPEAT);
			mapTextures["crow"] = texture;
		}
	}
	// Load meshes
	{
		Mesh* mesh = new Mesh("crow");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "CROW.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("bird");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "bird.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("tree");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("quad");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("cylinder");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "cylinder2.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("lamp");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "lamp.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("bin");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "bin.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("bench");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "bench.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(1, 1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(1, 1, 1)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple hex
	{
		vector<glm::vec3> vertices
		{
			// jos
			glm::vec3(-0.5,   -0.5, 0.0),	// A	B__C
			glm::vec3(-0.25,  -0.5, -0.5),	// B   A/  \D	
			glm::vec3(0.25f, -0.5f, -0.5f),	// C	\__/
			glm::vec3(0.5f,  -0.5f, 0.0f),	// D	F  E
			glm::vec3(0.25,  -0.5, 0.5),	// E
			glm::vec3(-0.25,  -0.5, 0.5),	// F

			// sus
			glm::vec3(-0.5,   0.5, 0.0),	// A'	B'__C'
			glm::vec3(-0.25,  0.5, -0.5),	// B'  A'/  \D'	
			glm::vec3(0.25f, 0.5f, -0.5f),	// C'	 \__/
			glm::vec3(0.5f,  0.5f, 0.0f),	// D'	 F' E'
			glm::vec3(0.25,  0.5, 0.5),	// E'
			glm::vec3(-0.25,  0.5, 0.5),	// F'
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f)
		};

		vector<unsigned short> indices =
		{
			0, 6, 11,
			11, 0, 5,

			11, 10, 5,
			10, 5, 4,

			10, 4, 9,
			4, 9, 3,

			9, 3, 2,
			9, 2, 8,

			8, 2, 1,
			8, 1, 7,

			7, 1, 0,
			7, 0, 6,

			7, 8, 9,
			6, 7, 9,
			6, 11, 9,
			11, 10, 9
		};

		Mesh* mesh = new Mesh("hex");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple hexagon
	{
		vector<glm::vec3> vertices
		{
			// jos
			glm::vec3(-0.5,   0, 0.0),	// A	B__C
			glm::vec3(-0.25,  0, -0.5),	// B   A/  \D	
			glm::vec3(0.25f, 0, -0.5f),	// C	\__/
			glm::vec3(0.5f,  0, 0.0f),	// D	F  E
			glm::vec3(0.25,  0, 0.5),	// E
			glm::vec3(-0.25,  0, 0.5),	// F

		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.5f, 0.0f),
			glm::vec2(0.0f, 0.25f),
			glm::vec2(0.0f, 0.75f),
			glm::vec2(0.5f, 1.0f),
			glm::vec2(1.0f, 0.75f),
			glm::vec2(1.0f, 0.25f),

		};

		vector<unsigned short> indices =
		{
			1, 2, 3,
			0, 1, 3,
			0, 3, 5,
			3, 5, 4
		};

		Mesh* mesh = new Mesh("hexi");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("Shader");
		shader->AddShader("Source/CityGenerator/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/CityGenerator/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader 
	{
		Shader *shader = new Shader("ShaderB");
		shader->AddShader("Source/CityGenerator/Shaders/BuildingVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/CityGenerator/Shaders/BuildingFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Init buildings
	// cubs
	primitive.push_back(new Primitive(meshes["box"], shaders["Shader"], "road"));
	primitive[0]->type = 0;
	primitive[0]->pos = glm::vec3(0, 0, 0);
	primitive[0]->scale = glm::vec3(0.75, 1, 0.75);
	primitive[0]->dim = glm::vec3(0.75, 1, 0.75);
	primitive[0]->addTexts("glass");
	primitive[0]->addTexts("glass4");
	primitive[0]->addTexts("glass2");
	primitive[0]->addTexts("glass3");

	// cylinder
	primitive.push_back(new Primitive(meshes["cylinder"], shaders["Shader"], "glass5"));
	primitive[1]->type = 1;
	primitive[1]->pos = glm::vec3(0, 0, 0);
	primitive[1]->scale = glm::vec3(0.7, 1, 0.7);
	primitive[1]->dim = glm::vec3(0.7, 1, 0.7);
	primitive[1]->addTexts("glass5");
	primitive[1]->addTexts("glass6");

	// hex
	primitive.push_back(new Primitive(meshes["hex"], shaders["Shader"], "glass5"));
	primitive[2]->type = 2;
	primitive[2]->pos = glm::vec3(0, 0, 0);
	primitive[2]->scale = glm::vec3(0.75, 1, 0.75);
	primitive[2]->dim = glm::vec3(0.75, 1, 0.75);
	primitive[2]->addTexts("glass");
	primitive[2]->addTexts("glass4");
	primitive[2]->addTexts("glass2");
	primitive[2]->addTexts("glass3");

	// all texts
	texts.push_back("glass");
	texts.push_back("glass4");
	texts.push_back("glass2");
	texts.push_back("glass3");
	texts.push_back("glass5");
	texts.push_back("glass6");
	

	// Generate roads
	int auxmaxTunnels = maxTunnels / 2 + std::rand() % (maxTunnels / 2);
	int auxMaxLength = maxLength / 2 + std::rand() % (maxLength / 2);

	int i, j, dir, lastDir = 0;
	vector<int> lastDirection = {0,0};
	i = std::rand() % width;
	j = std::rand() % height;

	// ALG FOR GENERATE MAP
	while (auxmaxTunnels > 0)
	{
		dir = std::rand() % 4;
		while (!isGoodDirection(directions[dir], i, j, lastDirection) ||
			(directions[dir][0] == lastDirection[0] && directions[dir][1] == lastDirection[1])) {
			dir = std::rand() % 4;
		}

		city[i][j] = 1;
		if (lastDirection[0] == 0 && lastDirection[1] == 0) {
			roadsDirection[i][j] = directions[dir];
		}
		else {
			if (lastDir == 0) {
				if (dir == 3) {
					roadsDirection[i][j] = { 1,2 };
				}
				else {
					roadsDirection[i][j] = { 1,3 };
				}
			}
			if (lastDir == 1) {
				if (dir == 3) {
					roadsDirection[i][j] = { 1,1 };
				}
				else {
					roadsDirection[i][j] = { 1,4 };
				}
			}
			if (lastDir == 2) {
				if (dir == 1) {
					roadsDirection[i][j] = { 1,2 };
				}
				else {
					roadsDirection[i][j] = { 1,1 };
				}
			}
			if (lastDir == 3) {
				if (dir == 1) {
					roadsDirection[i][j] = { 1,3 };
				}
				else {
					roadsDirection[i][j] = { 1,4 };
				}
			}
		}
		i += directions[dir][0];
		j += directions[dir][1];
		auxMaxLength--;
		lastDirection = directions[dir];
		lastDir = dir;
		
		while (isGoodDirection(directions[dir], i, j, lastDirection))
		{
			city[i][j] = 1;
			roadsDirection[i][j] = lastDirection;
			i += directions[dir][0];
			j += directions[dir][1];
			auxMaxLength--;
			if (auxMaxLength == 0) {
				break;
			}
		}
		auxMaxLength = maxLength / 2 + std::rand() % (maxLength / 2);
		auxmaxTunnels--;
	}

	for(int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			// ROAD
			if (city[i][j] == 1) {
				vector<int> n = neighbors(i, j);
				// left or right road
				if (n.size() == 2 && !continueRoad(n, i, j)) {
					int sum = n[0] + n[1];
					if (sum == 4) {
						roadsDirection[i][j] = { 1,2 };
					}else if (sum == 3 && (n[0] == 3 || n[1] == 3)) {
						roadsDirection[i][j] = { 1,1 };
					}
					else if (sum == 3 && (n[0] == 1 || n[1] == 1)) {
						roadsDirection[i][j] = { 1,3 };
					}else if (sum == 2) {
						roadsDirection[i][j] = { 1,4 };
					}
				}
				// is intersection
				if (n.size() == 4 && (diagNeighbors(i, j) < 3)) {
					roadsDirection[i][j] = { 0,0 };
				}
				// intersection with 3 streets
				if (n.size() == 3 && (diagNeighbors(i, j) < 2 )) {
					int num = whatsMissing(n, i, j) - 1;
			
					if (num == 0) {
						roadsDirection[i][j] = { 5,4 };
					}
					if (num == 1) {
						roadsDirection[i][j] = { 5,6};
					}
					if (num == 2) {
						roadsDirection[i][j] = { 5,7 };
					}
					if (num == 3) {
						roadsDirection[i][j] = { 5,5 };
					}
				}
			} // BIG BUILDING OR PARK
			else if(city[i][j] == 0){
				glm::vec3 mul = glm::vec3(1, 1, 1);
				int random = std::rand() % 25;
				float x, y, z;
				x = -width / 2 + j * (0.5 * 2);
				y = 0;
				z = -height / 2 + i * (0.5 * 2);

				if (i < height - 1 && j < width - 1) {
					if (isSpaceFor4(i, j) && random < 6) {
						if (random % 2 == 0) {
							city[i][j] = 6;
							city[i + 1][j] = 8;
							city[i][j + 1] = 7;
							city[i + 1][j + 1] = 9;
							numTree += 4;
							continue;
						}
						x += 0.5;
						z += 0.5;
						mul = glm::vec3(2, 2, 2);
						city[i][j] = 2;
						city[i+1][j] = 4;
						city[i][j+1] = 3;
						city[i+1][j+1] = 5;
						
					}
				}
				
				Building* building = new Building();
				vector<Primitive*> nou;
				int rand = std::rand() % 4 + 1;
				for (int i = 0; i < rand; i++)
				{
					int rand1 = std::rand() % 3;
					nou.push_back(primitive[rand1]->Copy());
					nou[i]->scale *= mul;
				}
				building = generateBuilding(nou, glm::vec3(x, y, z));
				nou = building->primitive;
				buildings.push_back(building);
			}
		}
		//printf("\n");
	}

	for (int i = 0; i < numTree; i++)
	{
		trees.push_back(std::rand() % 10);
	}

}

void CityGenerator::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	float time = Engine::GetElapsedTime() / 5;
	float x = 0.3;
	glClearColor(0, 0,1 - abs(sin(time)) + x, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void CityGenerator::Update(float deltaTimeSeconds)
{	
	int numOfTrees = numTree - 1;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int x, y, z;
			x = -width / 2+ j * (0.5 * 2);
			y = 0;
			z = -height / 2 + i * (0.5 * 2);
			if (city[i][j] == 1) {
				// ROADS
				glm::mat4 modelMatrix = glm::mat4(1);
				float teta = RADIANS(90);
				int sum = roadsDirection[i][j][0] + roadsDirection[i][j][1];

				modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
				if (roadsDirection[i][j][0] == 0 && sum < 2) {
					modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(0, 1, 0));
				}
				else if (sum >= 2){
					int times = roadsDirection[i][j][1] - 1;
					modelMatrix = glm::rotate(modelMatrix, teta * (times-1), glm::vec3(0, 1, 0));
				}
				modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
				if (sum != 0) {
					if (sum > 1 && sum < 6) {
						RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["roadJustLeft"]);
					}
					else if (sum >= 6) {
						RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["roadNoDown"]);
					}
					else{
						RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["road"]);
					}
				}
				else
					RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["roadInt"]);
			}
			else {

				// PARK

				glm::mat4 modelMatrix = glm::mat4(1);
				float teta = RADIANS(90);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
				modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
				RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["grass"]);

				if (city[i][j] != 3 && city[i][j] != 5 && city[i][j] != 7 && city[i][j] != 9) {
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.5, 0.003, z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.2, 1));
					modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
					RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["tiles"]);

					// lamp
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.4, 0, z - 0.4));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08, 0.08, 0.08));
					RenderSimpleMesh(meshes["lamp"], shaders["Shader"], modelMatrix, mapTextures["lamp"]);
					glm::mat4 modelMatrix = glm::mat4(1);

					// bench
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.45, 0, z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001, 0.001, 0.001));
					//RenderSimpleMesh(meshes["bench"], shaders["Shader"], modelMatrix, mapTextures["wood"]);
					RenderSimple2Mesh(meshes["bench"], shaders["ShaderB"], modelMatrix, mapTextures["wood"], glm::vec3(x - 0.45, 0.5, z));

					// bin
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.4, 0, z - 0.3));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001, 0.001, 0.001));
					//RenderSimpleMesh(meshes["bin"], shaders["Shader"], modelMatrix, mapTextures["glass2"]);
					RenderSimple2Mesh(meshes["bin"], shaders["ShaderB"], modelMatrix, mapTextures["glass2"], glm::vec3(x - 0.4, 0.5, z - 0.3));
				}
				
				if (city[i][j] != 4 && city[i][j] != 5 && city[i][j] != 8 && city[i][j] != 9) {
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0.003, z - 0.5));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1, 0.2, 0.1));
					modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(0, 1, 0));
					modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
					RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["tiles"]);

					// lamp
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.4, 0, z - 0.4));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08, 0.08, 0.08));
					RenderSimpleMesh(meshes["lamp"], shaders["Shader"], modelMatrix, mapTextures["lamp"]);

				}

				if (city[i][j] != 4 && city[i][j] != 2 && city[i][j] != 8 && city[i][j] != 6) {
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.5, 0.003, z));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.2, 1));
					modelMatrix = glm::rotate(modelMatrix, teta * 2, glm::vec3(0, 1, 0));
					modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
					RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["tiles"]);

					// lamp
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.4, 0, z + 0.4));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08, 0.08, 0.08));
					RenderSimpleMesh(meshes["lamp"], shaders["Shader"], modelMatrix, mapTextures["lamp"]);

					// bench
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.45, 0, z));
					modelMatrix = glm::rotate(modelMatrix, teta * 2, glm::vec3(0, 1, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001, 0.001, 0.001));
					RenderSimple2Mesh(meshes["bench"], shaders["ShaderB"], modelMatrix, mapTextures["wood"], glm::vec3(x + 0.45, 0.7, z));
				}

				if (city[i][j] != 3 && city[i][j] != 2 && city[i][j] != 7 && city[i][j] != 6) {
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0.003, z + 0.5));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1, 0.2, 0.1));
					modelMatrix = glm::rotate(modelMatrix, teta * 3, glm::vec3(0, 1, 0));
					modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
					RenderSimpleMesh(meshes["quad"], shaders["Shader"], modelMatrix, mapTextures["tiles"]);

					// lamp
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.4, 0, z + 0.4));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08, 0.08, 0.08));
					RenderSimpleMesh(meshes["lamp"], shaders["Shader"], modelMatrix, mapTextures["lamp"]);

					// bin
					modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.3, 0, z + 0.4));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001, 0.001, 0.001));
					RenderSimpleMesh(meshes["bin"], shaders["Shader"], modelMatrix, mapTextures["glass2"]);

				}
				// BIG PARK
				if (city[i][j] > 5) {
					float time = Engine::GetElapsedTime() / trees[numOfTrees] * 3;
					if(trees[numOfTrees] % 2 == 0)
					{
						float p = cos(time);
						glm::mat4 modelMatrix = glm::mat4(1);
						/*if (cos(time) < 0) {
							c = RADIANS(90);
						}*/
						float teta = RADIANS(p * 100);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x + sin(time) / 2, 1 - 0.5 + trees[numOfTrees] / 100, z + cos(time) / 2));
						modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(0, 1, 0));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
						RenderSimpleMesh(meshes["bird"], shaders["Shader"], modelMatrix, mapTextures["bird"]);

						modelMatrix = glm::mat4(1);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 1 - 0.05, z));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
						RenderSimpleMesh(meshes["crow"], shaders["Shader"], modelMatrix, mapTextures["bird"]);
					}
					{
						glm::mat4 modelMatrix = glm::mat4(1);
						float teta = RADIANS(90);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0.2, z));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.06, 0.7, 0.06));
						RenderSimpleMesh(meshes["cylinder"], shaders["Shader"], modelMatrix, mapTextures["wood2"]);
					}
					{
						glm::mat4 modelMatrix = glm::mat4(1);
						float teta = RADIANS(90);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.1, 0.55, z + 0.1));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.5, 0.4));
						RenderSimple2Mesh(meshes["tree"], shaders["ShaderB"], modelMatrix, mapTextures["grass2"], glm::vec3(x, 2, z));
					}
					{
						glm::mat4 modelMatrix = glm::mat4(1);
						float teta = RADIANS(90);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0.6, z));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.7, 0.4));
						RenderSimple2Mesh(meshes["tree"], shaders["ShaderB"], modelMatrix, mapTextures["grass2"], glm::vec3(x, 2, z));
					}
					{
						glm::mat4 modelMatrix = glm::mat4(1);
						float teta = RADIANS(90);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.1, 0.45, z));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.5, 0.4));
						RenderSimple2Mesh(meshes["tree"], shaders["ShaderB"], modelMatrix, mapTextures["grass2"], glm::vec3(x, 2, z));
					}

					{
						glm::mat4 modelMatrix = glm::mat4(1);
						float teta = RADIANS(90);
						modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.1, 0.45, z - 0.05));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.5, 0.4));
						RenderSimple2Mesh(meshes["tree"], shaders["ShaderB"], modelMatrix, mapTextures["grass2"], glm::vec3(x, 2, z));
					}
					numOfTrees--;
				}
			}
		}
	}

	// BUILDINGS

	for (int i = 0; i < buildings.size(); i++)
	{
		
		for (int j = 0; j < buildings[i]->primitive.size(); j++)
		{
			float teta = RADIANS(90);
			float pos = 2; 
			switch (buildings[i]->primitive[j]->type)
			{
			case 0:
				pos = 0.6;
				break;
			case 1:
				pos = 3;
				break;
			case 2:
				pos = 5;
				break;
			default:
				break;
			}

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, buildings[i]->primitive[j]->pos);
			modelMatrix = glm::scale(modelMatrix, buildings[i]->primitive[j]->scale);
			RenderSimple2Mesh(buildings[i]->primitive[j]->mesh, shaders["ShaderB"], 
				modelMatrix, mapTextures[buildings[i]->primitive[j]->texture], 
				buildings[i]->primitive[j]->pos + glm::vec3(0, pos + buildings[i]->primitive[j]->dim[1] / 2, 0));
			

			if (buildings[i]->primitive[j]->type == 0) {
				glm::vec3 scale = buildings[i]->primitive[j]->scale * glm::vec3(1, 0.01, 1);
				float aux = buildings[i]->primitive[j]->dim[1] / 2;
				modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, buildings[i]->primitive[j]->pos + glm::vec3(0, aux + 0.001, 0));
				modelMatrix = glm::scale(modelMatrix, scale);
				//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5,0.5,0.5));
				//modelMatrix = glm::rotate(modelMatrix, teta, glm::vec3(1, 0, 0));

				materialShininess = 100;
				materialKd = 0.9;
				materialKs = 0.9;
				RenderSimple2Mesh(meshes["box"], shaders["ShaderB"], modelMatrix, 
					mapTextures[buildings[i]->primitive[j]->texture2], buildings[i]->primitive[j]->pos + glm::vec3(0, aux+1, 0));

				
				materialShininess = 80;
				materialKd = 0.6;
				materialKs = 0.6;
			}
			else if (buildings[i]->primitive[j]->type == 2) {
				float aux = buildings[i]->primitive[j]->dim[1] / 2;
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, buildings[i]->primitive[j]->pos + glm::vec3(0, aux + 0.001, 0));
				modelMatrix = glm::scale(modelMatrix, buildings[i]->primitive[j]->scale);
				RenderSimple2Mesh(meshes["hexi"], shaders["ShaderB"], modelMatrix, 
					mapTextures[buildings[i]->primitive[j]->texture2], buildings[i]->primitive[j]->pos + glm::vec3(0, aux + 0.8, 0));
			}
			
		}
		
	}
}

void CityGenerator::FrameEnd()
{
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//DrawCoordinatSystem();
}

void CityGenerator::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float time = Engine::GetElapsedTime();
	int location = glGetUniformLocation(shader->program, "Time");
	glUniform1f(location, time);

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
		//TODO : activate texture location 0
		//TODO : Bind the texture1 ID
		//TODO : Send texture uniform value
	}

	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
		//TODO : activate texture location 1
		//TODO : Bind the texture2 ID
		//TODO : Send texture uniform value
	}
	else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void CityGenerator::RenderSimple2Mesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, glm::vec3 pos)
{
	

	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, pos.x, pos.y, pos.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, 0, -1, 0);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void CityGenerator::OnInputUpdate(float deltaTime, int mods)
{
	/*float speed = 2;

	old = GetSceneCamera()->transform->GetLocalOZVector();
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}*/

}

void CityGenerator::OnKeyPress(int key, int mods)
{
	// add key press event
}

void CityGenerator::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void CityGenerator::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void CityGenerator::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void CityGenerator::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void CityGenerator::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void CityGenerator::OnWindowResize(int width, int height)
{
}
