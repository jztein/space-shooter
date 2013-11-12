#pragma once

#include "GameSprite.h"

class Projectile: public GameSprite
{
public:
	Projectile(float2 playerPos);
	virtual ~Projectile(){}

	void update(float timeDelta);
};