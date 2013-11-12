#include "pch.h"
#include "Enemy.h"

float2 ENEMY_START_POS(200.0f, 350.0f);
float2 ENEMY_START_VEL(250.0f, 150.0f);

Enemy::Enemy()
{
	m_pos = ENEMY_START_POS;
	m_vel = ENEMY_START_VEL * RandFloat(0.8f, 1.5f);
}

Enemy::Enemy(Windows::Foundation::Rect windowBounds)
{
	m_pos.x = windowBounds.Width;
	m_pos.y = RandFloat(0.0, windowBounds.Height);
	m_vel = ENEMY_START_VEL * RandFloat(0.8f, 3.0f);
}

// Enemy only moves left
void Enemy::update(float timeDelta)
{
	m_pos.x = m_pos.x - (timeDelta * m_vel.x);
}
