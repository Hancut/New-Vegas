#pragma once

#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <Component/SimpleScene.h>

class Primitive
{
public:
	Primitive();
	Primitive(Mesh *mesh, Shader *shader, std::string texture1);
	~Primitive();
	
	void Primitive::addTexts(std::string t);
	Primitive* Primitive::Copy();

	Mesh *mesh;
	Shader *shader;
	std::string texture;
	std::string texture2;
	std::vector<std::string> texts;
	glm::vec3 pos;
	glm::vec3 dim;
	glm::vec3 scale;
	int type;
};

