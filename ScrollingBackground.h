#pragma once

#include "GameSprite.h"

class ScrollingBackground: public GameSprite
{
public:
	ScrollingBackground();
	virtual ~ScrollingBackground(){}

	virtual void loadTexture(Platform::String^ filename, BasicSprites::SpriteBatch^ spriteBatch, BasicLoader^ loader);

	virtual void update(float timeDelta);
	void setWindowSize(Windows::Foundation::Rect windowRect)
	{
		m_windowRect = windowRect;
	}

	void initSliding();

	// color: when game over can make background dark
	void draw(BasicSprites::SpriteBatch^ spriteBatch, float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f));
private:
	float2 m_pos1;
	float2 m_pos2;

	float m_updateStep;

	Windows::Foundation::Rect m_textureSize;
	Windows::Foundation::Rect m_windowRect;
};