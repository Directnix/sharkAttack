#pragma once

#include "GameObject.h"
#include "TerreinGenerator.h"
#include "Paddle.h"

class Player : public GameObject {
private:
	float forwardsSpeed = 0;
	float sideSpeed = 0;
	Paddle* leftP;
	Paddle* rightP;
	TerreinGenerator* tg;

	void updatePaddle(Paddle*, float, bool);

public:
	Player(ObjModel*, Paddle*, Paddle*, TerreinGenerator* );
	~Player();

	void move(float, float);
	void paddleLeft(float);
	void paddleRight(float);
	void update(float);
};
