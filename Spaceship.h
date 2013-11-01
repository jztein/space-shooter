#pragma once

#include "DirectXBase.h"
#include "BasicLoader.h"

enum ship_dir_t
{
	SHIP_UP,
	SHIP_DOWN,
	SHIP_LEFT,
	SHIP_RIGHT,
};

class Spaceship
{
public:
	Spaceship();
	~Spaceship();

	ID3D11Texture2D* Spaceship::loadTexture(BasicLoader^ loader);
	ID3D11Texture2D* getTexturePtr();

	void moveShip(ship_dir_t dir);
	void update(float deltaTime);
	void stopShip();

	// if fast, increase vel. else, decrease vel.
	void changeSpeed(bool fast);

	float2 getPos() { return m_pos;  }
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
	float2 m_pos;
	float2 m_vel;
	int m_health;

	int m_score;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	ID3D11Texture2D* m_texturePtr;
};