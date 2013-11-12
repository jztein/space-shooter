//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "DirectXBase.h"
#include "SampleOverlay.h"
#include "AutoThrottle.h"
#include "BasicSprites.h"

class Runner;
class Enemy;
class Projectile;
class ScrollingBackground;
class Fruit;
class Reset;

namespace GameSettings
{
	namespace EnemyDetails
	{
		static const unsigned int EnemyCountMax = 15;
		static const unsigned int EnemyCountMin = 0;
		static const unsigned int InitialEnemyCount = 1;
		static const float2 EnemySize(64.0f, 48.0f);
		static const float2 EnemyCollisionSize(EnemySize.x*1.3f, EnemySize.y*1.3f);
		static const unsigned int harm = 1;
		static const unsigned int killPoints = 1;
		static const unsigned int bonusKillPoints = 5;
	}
	namespace FruitDetails
	{
		static const float2 FruitSize(48.0f, 48.0f);
		static const float2 FruitCollisionSize(FruitSize.x*1.3f, FruitSize.y*1.3f);
		static const unsigned int healthPoints = -5; // since passed to decreaseHealth()
	}
	namespace ProjectileDetails
	{
		// maximum number of bullets at one time (then reload)
		static const unsigned int MaxBullets = 3;
	}
	namespace RunnerDetails
	{
		static const float2 RunnerSize(0.7f, 0.7f);
		static bool wasHit = false;
	}
	namespace OverlayDetails
	{
		static Platform::String^ caption = "Score";
	}
}

namespace SampleSettings
{
    static const unsigned int NumAsteroids = 20;
    namespace Performance
    {
        static const float TargetFrameTime = 1.0f / 20.0f;
        static const unsigned int InitialParticleCount = 100;
        static const unsigned int ParticleCountDelta = 32;
        static const unsigned int ParticleCountMin = 150;
		static const unsigned int ParticleCountMax = 200;
    }
    namespace Physics
    {
        static const float Gravity = 40000000.0f;
        static const float Damping = 0.015f;
    }
	
}

struct AsteroidData
{
    float2 pos;
    float2 vel;
    float rot;
    float rotVel;
    float scale;
};

struct ParticleData
{
    float2 pos;
    float2 vel;
};

ref class SimpleSprites : public DirectXBase
{
public:
	
	virtual ~SimpleSprites();
internal:
    SimpleSprites();

	void getKeyresults(Windows::System::VirtualKey key, float timeDelta);
	
    virtual void CreateDeviceIndependentResources() override;
    virtual void CreateDeviceResources() override;
    virtual void CreateWindowSizeDependentResources() override;
    virtual void Render() override;
    void Update(float timeTotal, float timeDelta);
    void AddRepulsor(_In_ uint32 id, _In_ float2 position);
    void MoveRepulsor(_In_ uint32 id, _In_ float2 position);
    void RemoveRepulsor(_In_ uint32 id);

	void RestartIfNeeded(float2 mousePos);
	
	void stopBasicTimer();

private:
	float RandFloat(float min, float max);
    SampleOverlay^ m_sampleOverlay;
    AutoThrottle^ m_autoThrottle;
    BasicSprites::SpriteBatch^ m_spriteBatch;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_ground;
    std::vector<AsteroidData> m_asteroidData;
    std::vector<ParticleData> m_particleData;
    std::map<uint32, float2> m_repulsors;

	Fruit* m_fruit;
	std::vector<Enemy*> m_enemies;
	std::vector<Projectile*> m_projectiles;

	Reset* m_resetButton;

	Runner* m_runner;
	ScrollingBackground* m_scroll_background;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_enemy_texture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_fruit_texture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_resetButton_texture;
    int m_numParticlesToDraw;
};
