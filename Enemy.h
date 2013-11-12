#pragma once

#include"GameSprite.h"

class Enemy: public GameSprite
{
public:
	Enemy();
	Enemy(Windows::Foundation::Rect windowBounds);
	virtual ~Enemy(){}

	void update(float timeDelta);


private:
	float RandFloat(float min, float max)
	{
		return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
	}
};