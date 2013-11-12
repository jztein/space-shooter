#pragma once

#include "GameSprite.h"

class Fruit: public GameSprite
{
public:
	Fruit();
	Fruit(Windows::Foundation::Rect windowBounds);
	virtual ~Fruit(){}

	virtual void update(float deltaTime);
};