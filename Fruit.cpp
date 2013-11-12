#include "pch.h"

#include "Fruit.h"

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

void Fruit::update(float timeDelta)
{
	m_pos.x = m_pos.x - (timeDelta * m_vel.x);
}