#include "pch.h"
#include "Fruit.h"
#include "SimpleSprites.h"

using namespace GameSettings::FruitDetails;

float FRUIT_MAX_X_VEL = 200.0f;
float FRUIT_MAX_Y_VEL = 800.0f;
float FRUIT_VEL_DELTA = 30.0f;
int FRUIT_START_HEALTH = 5;
float2 FRUIT_START_POS(200.0f, 350.0f);
float2 FRUIT_START_VEL(150.0f, 150.0f);

Fruit::Fruit()
{
	m_pos = FRUIT_START_POS;
	m_vel = FRUIT_START_VEL;
	m_texturePtr = NULL;
}

Fruit::Fruit(Windows::Foundation::Rect windowBounds)
{
	m_pos.x = windowBounds.Width;
	m_pos.y = 100.0f + static_cast<float>(rand() % 200);
	m_vel = FRUIT_START_VEL;
	m_texturePtr = NULL;
}

// Enemy only moves left
void Fruit::update(float timeDelta)
{
	m_pos.x = m_pos.x - (timeDelta * m_vel.x);
}

Fruit::~Fruit(){}

ID3D11Texture2D* Fruit::loadTexture(BasicLoader^ loader)
{/*
	loader->LoadTexture("apple.png", &m_texture, nullptr);
	m_texturePtr = m_texture.Get();
	return m_texturePtr;//*/
	return m_texturePtr;
}

ID3D11Texture2D* Fruit::getTexturePtr()
{
	return m_texturePtr;
}

bool Fruit::hasCollided(float2 pos)
{
	if (pos.x > (m_pos.x - FruitCollisionSize.x) &&
		pos.x < (m_pos.x + FruitCollisionSize.x) &&
		pos.y > (m_pos.y - FruitCollisionSize.y) &&
		pos.y < (m_pos.y + FruitCollisionSize.y))
		return true;

	return false;
}
