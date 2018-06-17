
#include "Player.h"
#define _USE_MATH_DEFINES

#define ROW_SPEED 0.5f
#include <math.h>

#include <iostream>

extern bool game_stuck;

Player::Player(ObjModel *model, Paddle* left, Paddle* right, TerreinGenerator* tg)
	: GameObject(model)
{
	this->leftP = left;
	this->rightP = right;
	this->tg = tg;
}

Player::~Player()
{
	delete model;
	delete leftP;
	delete rightP;
}

void  Player::updatePaddle(Paddle* p, float deltaTime, bool isLeft) {
	p->position.x = position.x;
	p->position.y = position.y;
	p->position.z = position.z;

	p->rotation.x = -rotation.x;
	if(isLeft)
		p->rotation.y = -rotation.y - ((float)sin(0.5f * M_PI) * 26) + ((float)sin(((180 * p->charge)) / 180 * M_PI) * 26);
	else
		p->rotation.y = -rotation.y + ((float)sin(0.5f * M_PI) * 26) - ((float)sin(((180 * p->charge)) / 180 * M_PI) * 26);

	if (p->charge < 1) {
		p->charge += (deltaTime * 1) * ROW_SPEED;
	}
}

void Player::update(float deltaTime)
{
	std::cout << tg->isFree(Vec2f(position.x, position.z)) << std::endl;

	if (!tg->isFree(Vec2f(position.x, position.z))) {
		if (forwardsSpeed > 0)
			forwardsSpeed -= (forwardsSpeed / 0.8f) * (deltaTime * 1);

		move(270, forwardsSpeed * deltaTime);

		if (sideSpeed > 1.5f) {
			rotation.y += (deltaTime * sideSpeed) * 10;
			sideSpeed -= (sideSpeed / 0.5f) * (deltaTime * 1);
		}
		else if (sideSpeed < 1.5f) {
			rotation.y -= (deltaTime * (-1 * sideSpeed)) * 10;
			sideSpeed += ((-1 * sideSpeed) / 0.5f) * (deltaTime * 1);
		}
		else
			sideSpeed = 0;

		updatePaddle(leftP, deltaTime, true);
		updatePaddle(rightP, deltaTime, false);
	}
	else {
		game_stuck = true;
		position = position;
	}

}

void Player::move(float angle, float fac) {

	position.x += (float)cos((rotation.y + angle) / 180 * M_PI) * (fac / 1.5f);
	position.z += (float)sin((rotation.y + angle) / 180 * M_PI) * (fac / 1.5f);
}

void Player::paddleLeft(float speed)
{
	forwardsSpeed += (speed * (leftP->charge - 0.5f));
	sideSpeed += (speed * 2 * (leftP->charge - 0.5f));

	leftP->charge = 0.5f;
}

void Player::paddleRight(float speed)
{
	forwardsSpeed += (speed * (rightP->charge - 0.5f));
	sideSpeed -= (speed * 2 * (rightP->charge - 0.5f));

	rightP->charge = 0.5f;
}

