#include "pch.h"
//#include "SimpleSprites.h"


//#include <string>

#include "BasicSprites.h"

#include "Runner.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

int RUN_START_HEALTH = 50;
float2 RUN_START_POS(200.0f, 350.0f);
float2 RUN_START_VEL(250.0f, 0.0f);
float RUN_GRAVITY = 1000.0f;
float RUN_JUMP_VEL = 800.0f;
//int RUN_ANIM_TIMER[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
int RUN_ANIM_TIMER[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 11 };
Runner::Runner()
{
	m_ground = 0.0f;
	m_pos = RUN_START_POS;
	m_vel = float2(0.0f, 0.0f);
	m_health = RUN_START_HEALTH;
	m_texturePtr = NULL;

	m_jumping = false;
	m_animTimer = 0;
}

Runner::~Runner()
{

}

void Runner::resetHealthScorePos()
{
	m_health = RUN_START_HEALTH;
	m_score = 0;
	m_pos = RUN_START_POS;
}

ID3D11Texture2D* Runner::loadTexture(BasicSprites::SpriteBatch^ spriteBatch, BasicLoader^ loader)
{
	loader->LoadTexture("volt1.png", &m_texture1, nullptr);
	m_texturePtr = m_texture1.Get();
	m_texturePtr1 = m_texture1.Get();

	loader->LoadTexture("volt2.png", &m_texture2, nullptr);
	m_texturePtr2 = m_texture2.Get();
	loader->LoadTexture("volt3.png", &m_texture3, nullptr);
	m_texturePtr3 = m_texture3.Get();
	loader->LoadTexture("volt4.png", &m_texture4, nullptr);
	m_texturePtr4 = m_texture4.Get();
	loader->LoadTexture("volt5.png", &m_texture5, nullptr);
	m_texturePtr5 = m_texture5.Get();
	loader->LoadTexture("volt6.png", &m_texture6, nullptr);
	m_texturePtr6 = m_texture6.Get();
	loader->LoadTexture("volt7.png", &m_texture7, nullptr);
	m_texturePtr7 = m_texture7.Get();
	loader->LoadTexture("volt8.png", &m_texture8, nullptr);
	m_texturePtr8 = m_texture8.Get();
	loader->LoadTexture("volt9.png", &m_texture9, nullptr);
	m_texturePtr9 = m_texture9.Get();
	loader->LoadTexture("volt10.png", &m_texture10, nullptr);
	m_texturePtr10 = m_texture10.Get();


	spriteBatch->AddTexture(m_texturePtr1);
	spriteBatch->AddTexture(m_texturePtr2);
	spriteBatch->AddTexture(m_texturePtr3);
	spriteBatch->AddTexture(m_texturePtr4);
	spriteBatch->AddTexture(m_texturePtr5);
	spriteBatch->AddTexture(m_texturePtr6);
	spriteBatch->AddTexture(m_texturePtr7);
	spriteBatch->AddTexture(m_texturePtr8);
	spriteBatch->AddTexture(m_texturePtr9);
	spriteBatch->AddTexture(m_texturePtr10);
	return m_texturePtr;
}

ID3D11Texture2D* Runner::getTexturePtr()
{
	return m_texturePtr;
}

void Runner::update(float deltaTime, Windows::Foundation::Rect windowBounds)
{
	++m_animTimer;
	// if at beginning of window AND trying to move left
	if (m_pos.x < 1e-5 && m_vel.x < 1e-5)
	{
		m_pos.y = m_pos.y + (deltaTime * m_vel.y);
		return;
	}
	if (m_pos.y < 1e-5)
	{
		m_pos.y = 1.0f;
		m_vel.y = 0.0f;
		return;
	}

	if (m_pos.y > m_ground + 1.0f)
	{
		m_jumping = false;
		m_pos.y = m_ground;
		m_vel.y = 0.0f;
		return;
	}
	
	if (m_jumping)
	{
		// v = u + at
		m_vel.y = m_vel.y + RUN_GRAVITY * deltaTime;

		// s = vt
		m_pos = m_pos + (deltaTime * m_vel);
	}
	else
	{
		// when not jumping, want sprite to run
		
		if (m_animTimer < RUN_ANIM_TIMER[0])
			m_texturePtr = m_texturePtr2;//*
		else if (m_animTimer < RUN_ANIM_TIMER[1])
			m_texturePtr = m_texturePtr3;
		else if (m_animTimer < RUN_ANIM_TIMER[2])
			m_texturePtr = m_texturePtr4;
		else if (m_animTimer < RUN_ANIM_TIMER[3])
			m_texturePtr = m_texturePtr5;
		else if (m_animTimer < RUN_ANIM_TIMER[4])
			m_texturePtr = m_texturePtr6;
		else if (m_animTimer < RUN_ANIM_TIMER[5])
			m_texturePtr = m_texturePtr7;//*/
		else if (m_animTimer < RUN_ANIM_TIMER[6])
			m_texturePtr = m_texturePtr8;
		else if (m_animTimer < RUN_ANIM_TIMER[7])
			m_texturePtr = m_texturePtr9;
		else if (m_animTimer < RUN_ANIM_TIMER[8])
			m_texturePtr = m_texturePtr10;
		else
			m_texturePtr = m_texturePtr1;

		if (m_animTimer > RUN_ANIM_TIMER[9])
			m_animTimer = 0;
		//*/
	}
		

	return;
}

void Runner::setPos(Windows::Foundation::Rect windowBounds)
{
	m_pos = float2(windowBounds.Width / 3, windowBounds.Height - 75.0f);
	RUN_START_POS = m_pos;
	m_ground = m_pos.y;
}

void Runner::stop()
{
	m_vel.x = 0.0f;
	m_vel.y = 0.0f;
}

void Runner::move(runner_dir_t dir)
{
	// JUMP
	if (dir == RUN_UP)
	{
		m_jumping = true;
		m_vel.y = -RUN_JUMP_VEL;
		//m_vel.x = 0;
		return;
	}/*
	if (dir == RUN_DOWN)
	{
		m_vel.y = RUN_START_VEL.y;
		m_vel.x = 0;
		return;
	}
	if (dir == RUN_LEFT)
	{
		m_vel.y = 0;
		m_vel.x = -RUN_START_VEL.x;
		return;
	}
	if (dir == RUN_RIGHT)
	{
		m_vel.y = 0;
		m_vel.x = RUN_START_VEL.x;
		return;
	}//*/
}
