#include "pch.h"
#include "Reset.h"
#include "SimpleSprites.h"

using namespace GameSettings::FruitDetails;

Reset::Reset()
{
	m_pos = float2(0.0f, 0.0f);
	m_texturePtr = NULL;
}

Reset::Reset(Windows::Foundation::Rect windowBounds)
{
	m_pos.x = 50.0f;
	m_pos.y = windowBounds.Height - 50.0f;
	m_texturePtr = NULL;
}

void Reset::setPos(Windows::Foundation::Rect windowBounds)
{
	m_pos.x = 50.0f;
	m_pos.y = windowBounds.Height - 75.0f;
}


Reset::~Reset(){}

ID3D11Texture2D* Reset::loadTexture(BasicLoader^ loader)
{/*
	loader->LoadTexture("restart_arrow.png", &m_texture, nullptr);
	m_texturePtr = m_texture.Get();
	return m_texturePtr;//*/
	return m_texturePtr;
}

ID3D11Texture2D* Reset::getTexturePtr()
{
	return m_texturePtr;
}

bool Reset::hasCollided(float2 pos)
{
	if (pos.x > (m_pos.x - FruitCollisionSize.x) &&
		pos.x < (m_pos.x + FruitCollisionSize.x) &&
		pos.y >(m_pos.y - FruitCollisionSize.y) &&
		pos.y < (m_pos.y + FruitCollisionSize.y))
		return true;

	return false;
}
