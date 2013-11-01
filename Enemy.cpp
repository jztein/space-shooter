#include "pch.h"
#include "Enemy.h"
#include "SimpleSprites.h"

using namespace GameSettings::EnemyDetails;

float ENEMY_MAX_X_VEL = 550.0f;
float ENEMY_MAX_Y_VEL = 550.0f;
float ENEMY_VEL_DELTA = 30.0f;
int ENEMY_START_HEALTH = 1;
float2 ENEMY_START_POS(200.0f, 350.0f);
float2 ENEMY_START_VEL(200.0f, 150.0f);

Enemy::Enemy()
{
	m_pos = ENEMY_START_POS;
	m_vel = ENEMY_START_VEL;
	m_health = ENEMY_START_HEALTH;
	m_texturePtr = NULL;
}

Enemy::Enemy(Windows::Foundation::Rect windowBounds)
{
	m_pos.x = windowBounds.Width;
	m_pos.y = RandFloat(0.0, windowBounds.Height);
	m_vel = ENEMY_START_VEL;
	m_health = ENEMY_START_HEALTH;
	m_texturePtr = NULL;
}

// Enemy only moves left
void Enemy::update(float timeDelta)
{
	m_pos.x = m_pos.x - (timeDelta * m_vel.x);
}

Enemy::~Enemy(){}

ID3D11Texture2D* Enemy::loadTexture(BasicLoader^ loader)
{
	loader->LoadTexture("enemyShip.png", &m_texture, nullptr);
	m_texturePtr = m_texture.Get();
	return m_texturePtr;
}

ID3D11Texture2D* Enemy::getTexturePtr()
{
	return m_texturePtr;
}

bool Enemy::hasCollided(float2 pos)
{
	if (pos.x > (m_pos.x - EnemySize.x) && 
		pos.x < (m_pos.x + EnemySize.x) &&
		pos.y >(m_pos.y - EnemySize.y) &&
		pos.y < (m_pos.y + EnemySize.y))
		return true;

	return false;
}

// to be implemented
int Enemy::getHealth() { return m_health;  }
void Enemy::decreaseHealth(int harm) {}
void Enemy::destroy() {}