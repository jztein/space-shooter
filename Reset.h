#pragma once

#include "GameSprite.h"

class Reset: public GameSprite
{
public:
	Reset(){}
	Reset(Windows::Foundation::Rect windowBounds);
	virtual ~Reset(){}

	void setPos(Windows::Foundation::Rect windowBounds);
};