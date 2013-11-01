#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"

class Enemy
{
public:
	Enemy();
	Enemy(Windows::Foundation::Rect windowBounds);
	~Enemy();

	ID3D11Texture2D* Enemy::loadTexture(BasicLoader^ loader);
	ID3D11Texture2D* getTexturePtr();

	float2 getPos() { return m_pos; }

	void update(float timeDelta);

	bool hasCollided(float2 pos);

	// to be implemented
	int getHealth();
	void decreaseHealth(int harm);
	void destroy();

private:
	float RandFloat(float min, float max)
	{
		return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
	}

	float2 m_pos;
	float2 m_vel;
	int m_health;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	ID3D11Texture2D* m_texturePtr;
};