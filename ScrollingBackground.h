#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"
#include "BasicSprites.h"

#include "pch.h"
//#include "SimpleSprites.h"


//#include <string>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

class ScrollingBackground
{
public:
	ScrollingBackground();
	~ScrollingBackground(){}

	ID3D11Texture2D* loadTexture(BasicLoader^ loader);
	ID3D11Texture2D* getTexturePtr() { return m_texturePtr; }

	void update();
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
	float m_vel;

	float m_updateStep;

	Windows::Foundation::Rect m_textureSize;
	Windows::Foundation::Rect m_windowRect;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	ID3D11Texture2D* m_texturePtr;
};