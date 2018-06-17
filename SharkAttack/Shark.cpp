
#include "Shark.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define DISTANCE_DANGER_3 20
#define DISTANCE_DANGER_2 45
#define DISTANCE_RISE 15
#define DISTANCE_DEATH 4
#define DISTANCE_START 60

#define Z_MAX -18.2f
#define Z_MIN -20.0f

#define SHARK_SPEED 2.85f

extern int game_danger;
extern bool game_eaten;

void Shark::idle(float deltaTime)
{
	game_danger = 0;

	if (distanceToPlayer > DISTANCE_START) {
		state = SharkState::CHASING;
	}
}

void Shark::chasing(float deltaTime)
{
	float playerAngle = atan2((position.x - player->position.x), (position.z - player->position.z)) * 180 / M_PI;
	rotation.y = playerAngle;

	position.x -= (float)sin((rotation.y) / 180 * M_PI) * (SHARK_SPEED * deltaTime);
	position.z -= (float)cos((rotation.y)/ 180 * M_PI) * (SHARK_SPEED * deltaTime);

	if (distanceToPlayer < DISTANCE_DANGER_3) {
		game_danger = 3;
	}
	else if (distanceToPlayer < DISTANCE_DANGER_2) {
		game_danger = 2;
	}
	else {
		game_danger = 1;
	}
}

Shark::Shark(ObjModel * model, Player* player)
	: GameObject(model)
{
	this->player = player;
	this->state = SharkState::IDLE;
	this->distanceToPlayer = 0;
}

Shark::~Shark()
{
	delete model;
}

void Shark::update(float deltaTime)
{
	distanceToPlayer = (float)sqrt(pow(player->position.x - position.x, 2) + pow(player->position.y - position.y, 2) + pow(player->position.z - position.z, 2));

	if (distanceToPlayer < DISTANCE_DEATH) {
		game_eaten = true;
	}
	
	if (distanceToPlayer < DISTANCE_RISE) {
		position.y = ((distanceToPlayer * (Z_MIN - Z_MAX)) / (DISTANCE_RISE + 2)) + Z_MAX;
	}

	switch (state) {
		case SharkState::IDLE: idle(deltaTime); break;
		case SharkState::CHASING: chasing(deltaTime); break;
	}
}
