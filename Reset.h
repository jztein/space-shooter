#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"

class Reset
{
public:
	Reset();
	Reset(Windows::Foundation::Rect windowBounds);
	~Reset();

	void setPos(Windows::Foundation::Rect windowBounds);

	float2 getPos() { return m_pos; }

	ID3D11Texture2D* loadTexture(BasicLoader^ loader);
	ID3D11Texture2D* getTexturePtr();

	bool hasCollided(float2 pos);

private:

	float2 m_pos;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	ID3D11Texture2D* m_texturePtr;
};