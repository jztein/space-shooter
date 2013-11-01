#pragma once

#include "DirectXBase.h"
#include "SimpleSprites.h"
//#include "BasicLoader.h"

class Projectile
{
public:
	Projectile(float2 shipPos);
	~Projectile(){}

	// return -1 if did not collide
	int collidedWithEnemy(std::vector<Enemy*> enemies);

	void update(float timeDelta);
	float2 getPos() { return m_pos; }

private:
	float2 m_pos;
	float2 m_vel;
};