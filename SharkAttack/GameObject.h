#pragma once

#include "Vec.h"
#include "ObjModel.h"

class GameObject {
protected:
	ObjModel* model;

public:
	GameObject(ObjModel*);

	~GameObject();

	Vec3f position;
	Vec3f rotation;
	Vec3f scale;

	void draw();
	virtual void update(float elapsedTime);
};