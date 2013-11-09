#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"

class Fruit
{
public:
	Fruit();
	Fruit(Windows::Foundation::Rect windowBounds);
	~Fruit();

	ID3D11Texture2D* loadTexture(BasicLoader^ loader);
	ID3D11Texture2D* getTexturePtr();

	float2 getPos() { return m_pos; }

	void update(float timeDelta);

	bool hasCollided(float2 pos);

private:

	float2 m_pos;
	float2 m_vel;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	ID3D11Texture2D* m_texturePtr;
};