#include "pch.h"

#include "ScrollingBackground.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

using namespace BasicSprites;

ScrollingBackground::ScrollingBackground()
{
	m_updateStep = 5.0f;

	m_pos1.x = 0.0f; m_pos1.y = 0.0f;
	m_pos2.x = 0.0f; m_pos2.y = 0.0f;

	m_texturePtr = NULL;
}

void ScrollingBackground::update()
{
	if (m_pos1.x < -m_windowRect.Width / 2)
		m_pos1.x = m_pos2.x + m_windowRect.Width;
	if (m_pos2.x < -m_windowRect.Width / 2)
		m_pos2.x = m_pos1.x + m_windowRect.Width;

	m_pos1.x -= m_updateStep;
	m_pos2.x -= m_updateStep;
}

void ScrollingBackground::initSliding()
{
	m_pos1 = m_windowRect.Width / 2;
	m_pos2 = m_windowRect.Width * 1.5;
}
void ScrollingBackground::draw(BasicSprites::SpriteBatch^ spriteBatch, float4 color)
{

	spriteBatch->Draw(
		m_texturePtr,
		float2(m_pos1.x / m_windowRect.Width, 0.5f),
		//float2(0.5f, 0.5f),
		PositionUnits::Normalized,
		float2(m_windowRect.Width / m_textureSize.Width, m_windowRect.Height / m_textureSize.Height),
		SizeUnits::Normalized,
		color
		);
	spriteBatch->Draw(
		m_texturePtr,
		float2(m_pos2.x / m_windowRect.Width, 0.5f),
		//float2(0.5f, 0.5f),
		PositionUnits::Normalized,
		float2(m_windowRect.Width / m_textureSize.Width, m_windowRect.Height / m_textureSize.Height),
		SizeUnits::Normalized,
		color
		);
}

ID3D11Texture2D* ScrollingBackground::loadTexture(BasicLoader^ loader)
{
	loader->LoadTexture("3d-underwater_bg.png", &m_texture, nullptr);
	
	// Get texture size
	D3D11_TEXTURE2D_DESC* descriptor = new D3D11_TEXTURE2D_DESC;
	m_texture->GetDesc(descriptor);
	m_textureSize.Width = descriptor->Width;
	m_textureSize.Height = descriptor->Height;
	
	m_texturePtr = m_texture.Get();
	return m_texturePtr;
}