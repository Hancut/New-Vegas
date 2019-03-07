#pragma once

#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <Component/SimpleScene.h>
#include "Primitive.h"

class Building
{
public:
	Building();
	~Building();
	
	std::vector<Primitive*> primitive;
	glm::vec3 pos;
	glm::vec3 dim;
};

