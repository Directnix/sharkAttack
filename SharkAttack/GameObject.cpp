#include "GameObject.h"

extern std::vector<GameObject*> objects;

GameObject::GameObject(ObjModel* model)
{
	this->model = model;
	this->position = Vec3f(0, 0, 0);
	this->rotation = Vec3f(0, 0, 0);
	this->scale = Vec3f(1, 1, 1);

	objects.push_back(this);
}

GameObject::~GameObject()
{
	delete model;
}

void GameObject::draw()
{
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);
	glRotatef(rotation.x, 1, 0, 0);
	glRotatef(rotation.y, 0, 1, 0);
	glRotatef(rotation.z, 0, 0, 1);

	model->draw();
	glPopMatrix();
}

void GameObject::update(float elapsedTime)
{
}
