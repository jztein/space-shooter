#include "pch.h"
#include "Reset.h"
#include "SimpleSprites.h"

using namespace GameSettings::FruitDetails;

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

