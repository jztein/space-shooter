#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"

enum runner_dir_t
{
	RUN_UP,
	RUN_DOWN,
	RUN_LEFT,
	RUN_RIGHT,
};

class Runner
{
public:
	Runner();
	~Runner();

	void resetHealthScorePos();

	ID3D11Texture2D* loadTexture(BasicSprites::SpriteBatch^ spriteBatch, BasicLoader^ loader);
	ID3D11Texture2D* getTexturePtr();

	void move(runner_dir_t dir);
	void update(float deltaTime, Windows::Foundation::Rect windowBounds);
	void setPos(Windows::Foundation::Rect windowBounds);
	void stop();

	bool isJumping() { return m_jumping; }

	unsigned int getNumTextures() { return 10; }

	float2 getPos() { return m_pos; }
	int getHealth() { return m_health; }
	void decreaseHealth(int harm)
	{
		if (m_health <= 0)
			return;
		m_health -= harm;
	}
	int getScore() { return m_score; }
	void plusScore(int points) { m_score += points; }

private:
	float m_ground;
	float2 m_pos;
	float2 m_vel;
	int m_health;

	int m_score;

	bool m_jumping;

	int m_animTimer;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture1;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture3;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture4;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture5;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture6;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture7;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture8;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture9;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture10;
	ID3D11Texture2D* m_texturePtr;
	ID3D11Texture2D* m_texturePtr1;
	ID3D11Texture2D* m_texturePtr2;
	ID3D11Texture2D* m_texturePtr3;
	ID3D11Texture2D* m_texturePtr4;
	ID3D11Texture2D* m_texturePtr5;
	ID3D11Texture2D* m_texturePtr6;
	ID3D11Texture2D* m_texturePtr7;
	ID3D11Texture2D* m_texturePtr8;
	ID3D11Texture2D* m_texturePtr9;
	ID3D11Texture2D* m_texturePtr10;
};