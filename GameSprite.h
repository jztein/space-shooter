#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"
#include "BasicSprites.h"

enum sprite_dir_t
{
	SPRITE_DIR_RIGHT,
	SPRITE_DIR_LEFT,
	SPRITE_DIR_UP,
	SPRITE_DIR_DOWN,
};

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

class GameSprite
{
public:
	GameSprite()
	{
		m_pos = float2(0.0f, 0.0f);
		m_vel = float2(0.0f, 0.0f);
		m_texturePtr = NULL;
	}
	virtual ~GameSprite(){}

	virtual void loadTexture(Platform::String^ filename, BasicSprites::SpriteBatch^ spriteBatch,
							 BasicLoader^ loader)
	{
		loader->LoadTexture(filename, &m_texture, nullptr);
		m_texturePtr = m_texture.Get();

		spriteBatch->AddTexture(m_texturePtr);
	}
	ID3D11Texture2D* getTexturePtr(){ return m_texturePtr; }

	virtual void update(float deltaTime){}
	virtual void update(float deltaTime, Windows::Foundation::Rect windowBounds){}
	virtual void setPos(Windows::Foundation::Rect windowBounds){}

	float2 getPos() { return m_pos; }

	bool hasCollided(float2 pos, float2 collisionBounds)
	{
		if (pos.x > (m_pos.x - collisionBounds.x) &&
			pos.x < (m_pos.x + collisionBounds.x) &&
			pos.y > (m_pos.y - collisionBounds.y) &&
			pos.y < (m_pos.y + collisionBounds.y))
			return true;

		return false;
	}

	// member variables

	float2 m_pos;
	float2 m_vel;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	ID3D11Texture2D* m_texturePtr;
};