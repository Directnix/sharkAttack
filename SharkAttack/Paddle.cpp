#include "Paddle.h"
#define _USE_MATH_DEFINES
#include <math.h>

Paddle::Paddle(ObjModel * model, Vec3f offset, float defangle)
: GameObject(model){
	this->offset = offset;
	this->defangle = defangle;
}

Paddle::~Paddle()
{
	delete model;
}

void Paddle::update(float deltaTime)
{
}
