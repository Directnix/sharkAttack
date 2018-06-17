#pragma once

#include "GameObject.h"

class Paddle : public GameObject {
private:
	Vec3f offset;
	float defangle;
public:
	float charge = 1;

	Paddle(ObjModel*, Vec3f, float);
	~Paddle();

	void update(float);
};