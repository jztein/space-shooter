#include "pch.h"

#include "Projectile.h"

float PROJECTILE_VEL = 500.0f;

Projectile::Projectile(float2 shipPos)
: m_pos(shipPos)
{}

// return -1 if did not collide
int Projectile::collidedWithEnemy(std::vector<Enemy*> enemies)
{
	return -1;
}

void Projectile::update(float timeDelta)
{
	m_pos.x += (PROJECTILE_VEL * timeDelta);
}