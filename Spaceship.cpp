#include "pch.h"
//#include "SimpleSprites.h"


//#include <string>

#include "Spaceship.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

//using namespace BasicSprites;

float SHIP_MAX_X_VEL = 550.0f;
float SHIP_MAX_Y_VEL = 550.0f;
float SHIP_VEL_DELTA = 30.0f;
int SHIP_START_HEALTH = 25;
float2 SHIP_START_POS(200.0f, 350.0f);
float2 SHIP_START_VEL(250.0f, 250.0f);

Spaceship::Spaceship()
{
	m_pos = SHIP_START_POS;
	m_vel = float2(0.0f, 0.0f);
	m_health = SHIP_START_HEALTH;
	m_texturePtr = NULL;
}

Spaceship::~Spaceship()
{

}

ID3D11Texture2D* Spaceship::loadTexture(BasicLoader^ loader)
{
	loader->LoadTexture("oohrooShip.png", &m_texture, nullptr);
	m_texturePtr = m_texture.Get();
	return m_texturePtr;
}

ID3D11Texture2D* Spaceship::getTexturePtr()
{
	return m_texturePtr;
}

void Spaceship::update(float deltaTime)
{
	m_pos = m_pos + (deltaTime * m_vel);
}

void Spaceship::stopShip()
{
	m_vel.x = 0.0f;
	m_vel.y = 0.0f;
}

void Spaceship::moveShip(ship_dir_t dir)
{
	if (dir == SHIP_UP)
	{
		m_vel.y = -SHIP_START_VEL.y;
		m_vel.x = 0;
		return;
	}
	if (dir == SHIP_DOWN)
	{
		m_vel.y = SHIP_START_VEL.y;
		m_vel.x = 0;
		return;
	}
	if (dir == SHIP_LEFT)
	{
		m_vel.y = 0;
		m_vel.x = -SHIP_START_VEL.x;
		return;
	}
	if (dir == SHIP_RIGHT)
	{
		m_vel.y = 0;
		m_vel.x = SHIP_START_VEL.x;
		return;
	}
}

void Spaceship::changeSpeed(bool is_fast)
{
	if (is_fast)
		m_vel = m_vel + float2(SHIP_VEL_DELTA, SHIP_VEL_DELTA);
	
	else
		m_vel = m_vel - float2(SHIP_VEL_DELTA, SHIP_VEL_DELTA);
	//*
	// cap speeds if necessary
	if (m_vel.x > SHIP_MAX_X_VEL)
	{
		m_vel.x = SHIP_MAX_X_VEL;
		return;
	}
	if (m_vel.x < -SHIP_MAX_X_VEL)
	{
		m_vel.x = -SHIP_MAX_X_VEL;
		return;
	}
	if (m_vel.y > SHIP_MAX_Y_VEL)
	{
		m_vel.y = SHIP_MAX_Y_VEL;
		return;
	}
	if (m_vel.y < -SHIP_MAX_Y_VEL)
	{
		m_vel.y = -SHIP_MAX_Y_VEL;
		return;
	}//*/

	/*
	switch (dir)
	{
	case SHIP_UP:
	case SHIP_DOWN:
	if (is_fast)
	m_vel.y += VEL_DELTA;
	else
	m_vel.y -= VEL_DELTA;
	break;
	case SHIP_LEFT:
	case SHIP_RIGHT:
	if (is_fast)
	m_vel.x += VEL_DELTA;
	else
	m_vel.x -= VEL_DELTA;
	break;
	}
	*/

	return;
}