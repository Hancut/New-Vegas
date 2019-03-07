#include "Primitive.h"
#include <vector>


Primitive::Primitive(Mesh *mesh, Shader *shader, std::string texture1)
{
	this->mesh = mesh;
	this->shader = shader;
	this->texture = texture1;
}

Primitive::Primitive() {

}


Primitive::~Primitive()
{
}

Primitive* Primitive::Copy() {
	Primitive* copy = new Primitive();
	int rand = std::rand() % 2;

	copy->mesh = this->mesh;
	copy->shader = this->shader;
	copy->pos = this->pos;
	if (this->type == 1) {
		copy->texture = this->texts[0];
	}
	else {
		copy->texture = this->texts[rand];
		copy->texture2 = this->texts[rand + 2];
	}
	
	copy->dim = this->dim;
	copy->scale = this->scale;
	copy->type = this->type;
	return copy;
}

void Primitive::addTexts(std::string t) {
	this->texts.push_back(t);
}
