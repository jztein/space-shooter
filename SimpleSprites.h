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

class Spaceship;
class Enemy;
class Projectile;

namespace GameSettings
{
	namespace EnemyDetails
	{
		static const unsigned int EnemyCountMax = 15;
		static const unsigned int EnemyCountMin = 0;
		static const unsigned int InitialEnemyCount = 1;
		static const float2 EnemySize(32.0f, 32.0f);
		static const unsigned int harm = 1;
		static const unsigned int killPoints = 1;
	}
	namespace ProjectileDetails
	{
		// maximum number of bullets at one time (then reload)
		static const unsigned int MaxBullets = 3;
	}
	namespace SpaceshipDetails
	{
		static const float2 SpaceshipSize(32.0f, 32.0f);
		static bool wasHit = false;
	}
	namespace OverlayDetails
	{
		static Platform::String^ caption = "Score";
	}
	namespace BackgroundDetails
	{
		// Thanks to Fall Fury's vertical scrolling background tutorial
		static const float BACKGROUND_MIDPOINT = 1366.0f / 2.0f;
		static float m_backgroundPos0 = BACKGROUND_MIDPOINT;
		static float m_backgroundPos1 = m_backgroundPos0 * 3;
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
	
	void stopBasicTimer();

private:
	float RandFloat(float min, float max);
    SampleOverlay^ m_sampleOverlay;
    AutoThrottle^ m_autoThrottle;
    BasicSprites::SpriteBatch^ m_spriteBatch;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_background;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_asteroid;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_particle;
    std::vector<AsteroidData> m_asteroidData;
    std::vector<ParticleData> m_particleData;
    std::map<uint32, float2> m_repulsors;

	std::vector<Enemy*> m_enemies;
	std::vector<Projectile*> m_projectiles;

	Spaceship* m_spaceship;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_enemy_texture;

    int m_numParticlesToDraw;
};