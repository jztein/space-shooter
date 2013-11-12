#include "pch.h"

#include "Projectile.h"

float PROJECTILE_VEL = 500.0f;

Projectile::Projectile(float2 playerPos)
{
	m_pos = playerPos;
}

void Projectile::update(float timeDelta)
{
	m_pos.x += (PROJECTILE_VEL * timeDelta);
}