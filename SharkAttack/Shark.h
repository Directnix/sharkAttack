#pragma once

#include "GameObject.h"
#include "Player.h"

enum SharkState {
	IDLE, CHASING
};

class Shark : public GameObject {
private:
	float distanceToPlayer;

	void idle(float);
	void chasing(float);

	Player* player;
public:
	Shark(ObjModel*, Player* );
	~Shark();

	SharkState state;

	void update(float);
};