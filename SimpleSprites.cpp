//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "SimpleSprites.h"
#include "BasicLoader.h"

#include "Spaceship.h"
#include "Enemy.h"
#include "Projectile.h"
#include "ScrollingBackground.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

using namespace BasicSprites;

using namespace GameSettings;

SimpleSprites::SimpleSprites() :
    m_numParticlesToDraw(SampleSettings::Performance::InitialParticleCount)
{
	// create player object
	m_spaceship = new Spaceship;// space_ship02.gif");

	m_scroll_background = new ScrollingBackground;
}

SimpleSprites::~SimpleSprites()
{
	delete m_spaceship;
}

void SimpleSprites::CreateDeviceIndependentResources()
{
    DirectXBase::CreateDeviceIndependentResources();

    // Create the performance throttler.

    m_autoThrottle = ref new AutoThrottle(SampleSettings::Performance::TargetFrameTime);
}

void SimpleSprites::CreateDeviceResources()
{
    DirectXBase::CreateDeviceResources();

    // Create the sprite batch.

    m_spriteBatch = ref new SpriteBatch();
	// 1+.. for player
    unsigned int capacity = 1 + SampleSettings::Performance::ParticleCountMax 
							+ SampleSettings::NumAsteroids + EnemyDetails::EnemyCountMax
							+ ProjectileDetails::MaxBullets
							+ 1;
    if (m_featureLevel < D3D_FEATURE_LEVEL_9_3)
    {
        capacity = min(Parameters::MaximumCapacityCompatible, capacity);
    }
    m_spriteBatch->Initialize(
        m_d3dDevice.Get(),
        capacity
        );

    // Load the sprite textures.

    BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());

	// load scrolling background texture
	m_spriteBatch->AddTexture(m_scroll_background->loadTexture(loader));
	/*
    loader->LoadTexture(
        "m31.png",
        &m_background,
        nullptr
        );
    m_spriteBatch->AddTexture(m_background.Get());//*/

    loader->LoadTexture(
        "fishasteroid.png",
        &m_asteroid,
        nullptr
        );
    m_spriteBatch->AddTexture(m_asteroid.Get());

    loader->LoadTexture(
        "particle.dds",
        &m_particle,
        nullptr
        );
    m_spriteBatch->AddTexture(m_particle.Get());

	// load enemy texture
	loader->LoadTexture("seaenemy.png", &m_enemy_texture, nullptr);
	m_spriteBatch->AddTexture(m_enemy_texture.Get());

	// load player texture
	m_spriteBatch->AddTexture(m_spaceship->loadTexture(loader));

    // Create the Sample Overlay.

    m_sampleOverlay = ref new SampleOverlay();

    m_sampleOverlay->Initialize(
        m_d2dDevice.Get(),
        m_d2dContext.Get(),
        m_wicFactory.Get(),
        m_dwriteFactory.Get(),
        OverlayDetails::caption
        );
}

void SimpleSprites::CreateWindowSizeDependentResources()
{
    DirectXBase::CreateWindowSizeDependentResources();

	// Give scroll background object window information
	m_scroll_background->setWindowSize(m_windowBounds);
	m_scroll_background->initSliding();

    // Randomly generate some non-interactive asteroids to fit the screen.

    m_asteroidData.clear();
    for (int i = 0; i < SampleSettings::NumAsteroids; i++)
    {
        AsteroidData data;
		data.pos.x = RandFloat(-0.5*m_windowBounds.Width*i/SampleSettings::NumAsteroids, m_windowBounds.Width);
		data.pos.y = RandFloat(-0.5*m_windowBounds.Height*i/SampleSettings::NumAsteroids, m_windowBounds.Height);
        float tempRot = RandFloat(-PI_F, PI_F);
        float tempMag = RandFloat(0.0f, 17.0f);
        data.vel.x = tempMag * cosf(tempRot);
        data.vel.y = tempMag * sinf(tempRot);
        data.rot = RandFloat(-PI_F, PI_F);
        data.scale = RandFloat(0.1f, 0.4f); // scale
        data.rotVel = RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * data.scale);
        m_asteroidData.push_back(data);
    }


    if (m_particleData.size() == 0)
    {
        // Initialize the interactive particle buffer to fill the window if it is empty.

        for (int i = 0; i < SampleSettings::Performance::ParticleCountMax; i++)
        {
            ParticleData data;
            data.pos.x = RandFloat(0.0f, m_windowBounds.Width);
            data.pos.y = RandFloat(0.0f, m_windowBounds.Height);
            data.vel = float2(0.0f, 0.0f);
            m_particleData.push_back(data);
        }
    }
    else
    {
        // Otherwise, move the interactive particles to fit within the screen.

        for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
        {
            if (particle->pos.x > m_windowBounds.Width)
            {
                particle->pos.x = m_windowBounds.Width;
            }
            if (particle->pos.y > m_windowBounds.Height)
            {
                particle->pos.y = m_windowBounds.Height;
            }
        }
    }

    m_sampleOverlay->UpdateForWindowSizeChange();
}

void SimpleSprites::Update(float timeTotal, float timeDelta)
{
    // Update the performance throttler.

    auto control = m_autoThrottle->Update(timeDelta);

    if (control == FrameWorkload::Increase)
    {
        m_numParticlesToDraw += SampleSettings::Performance::ParticleCountDelta;
    }
    if (control == FrameWorkload::Decrease)
    {
        m_numParticlesToDraw -= SampleSettings::Performance::ParticleCountDelta;
    }
    if (control != FrameWorkload::Maintain)
    {
        m_numParticlesToDraw = max(SampleSettings::Performance::ParticleCountMin, min(SampleSettings::Performance::ParticleCountMax, m_numParticlesToDraw));
        if (m_featureLevel < D3D_FEATURE_LEVEL_9_3)
        {
            m_numParticlesToDraw = min(static_cast<int>(Parameters::MaximumCapacityCompatible - SampleSettings::NumAsteroids - 1), m_numParticlesToDraw);
        }
    }

    // Update the non-interactive asteroids.
    // Their behavior is to drift across the window with a fixed translational and rotational
    // velocity.  Upon crossing a boundary outside the window, their position wraps.

    for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
    {
        static const float border = 100.0f;
        asteroid->pos = asteroid->pos + asteroid->vel * timeDelta;
        if (asteroid->vel.x < 0)
        {
            if (asteroid->pos.x < -border)
            {
                asteroid->pos.x = m_windowBounds.Width + border;
            }
        }
        else
        {
            if (asteroid->pos.x > m_windowBounds.Width + border)
            {
                asteroid->pos.x = -border;
            }
        }
        if (asteroid->vel.y < 0)
        {
            if (asteroid->pos.y < -border)
            {
                asteroid->pos.y = m_windowBounds.Height + border;
            }
        }
        else
        {
            if (asteroid->pos.y > m_windowBounds.Height + border)
            {
                asteroid->pos.y = -border;
            }
        }

        asteroid->rot += asteroid->rotVel * timeDelta;
        if (asteroid->rot > PI_F)
        {
            asteroid->rot -= 2.0f * PI_F;
        }
        if (asteroid->rot < -PI_F)
        {
            asteroid->rot += 2.0f * PI_F;
        }
    }

    // Update the interactive particles.
    // Their behavior is to be gravitationally attracted to two oscillating gravity
    // wells and repelled by any pressed pointer points.  Upon reaching the edge of
    // the window, the particles bounce.

    // Add two gravity wells that move throughout the window.
    float2 wellPositions[] =
    {
        float2(
            (1.0f + 0.8f * cosf(timeTotal / (2.0f * PI_F) + 3.0f)) * m_windowBounds.Width / 2.0f,
            (1.0f + 0.8f * sinf(timeTotal / 5.0f)) * m_windowBounds.Height / 2.0f
            ),
        float2(
            (1.0f + 0.8f * cosf(timeTotal / (PI_F * PI_F) + 1.0f)) * m_windowBounds.Width / 2.0f,
            (1.0f + 0.8f * sinf(timeTotal / PI_F)) * m_windowBounds.Height / 2.0f
            )
    };

    for (auto particle = m_particleData.begin(); particle != m_particleData.begin() + m_numParticlesToDraw; particle++)
    {
        if (particle->pos.x < 0)
        {
            particle->vel.x = abs(particle->vel.x);
        }
        if (particle->pos.x > m_windowBounds.Width)
        {
            particle->vel.x = -abs(particle->vel.x);
        }
        if (particle->pos.y < 0)
        {
            particle->vel.y = abs(particle->vel.y);
        }
        if (particle->pos.y > m_windowBounds.Height)
        {
            particle->vel.y = -abs(particle->vel.y);
        }

        for (auto repulsor = m_repulsors.begin(); repulsor != m_repulsors.end(); repulsor++)
        {
            float2 delta = particle->pos - repulsor->second;
            float deltaLength = length(delta) + 24.0f; // Offset length to avoid division by zero.
            float deltaLengthCubed = deltaLength * deltaLength * deltaLength;
            particle->vel = particle->vel + SampleSettings::Physics::Gravity * timeDelta * delta / deltaLengthCubed;
        }

        for (int i = 0; i < ARRAYSIZE(wellPositions); i++)
        {
            float gravitySign = 1.0f;
            if ((static_cast<int>(timeTotal / 2.0f) + 1) % 10 == 0)
            {
                // Every 20 seconds, "explode" the gravity wells for 2 seconds.
                gravitySign = -1.0f;
            }
            float2 delta = wellPositions[i] - particle->pos;
            float deltaLength = length(delta) + 24.0f;
            float deltaLengthCubed = deltaLength * deltaLength * deltaLength;
            particle->vel = particle->vel + gravitySign * 0.2f * SampleSettings::Physics::Gravity * timeDelta * delta / deltaLengthCubed;
        }

        particle->vel = particle->vel * (1.0f - SampleSettings::Physics::Damping);

        // Add random noise to the velocity to prevent particles from locking together.

        particle->vel.x += RandFloat(-0.5f, 0.5f);
        particle->vel.y += RandFloat(-0.5f, 0.5f);

        particle->pos = particle->pos + particle->vel * timeDelta;
    }

	// destroy lost or dead enemies
	std::vector<Enemy*> new_m_enemies;
	for (auto enemy : m_enemies)
	{
		// check if projectile is off screen
		if (enemy->getPos().x < -m_windowBounds.X)
		{
			delete enemy;
		}
		else
		{
			// check if was hit (less likely than being out of bounds, so check second)
			bool hit = false;
			for (auto p : m_projectiles)
			{
				if (enemy->hasCollided(p->getPos()))
				{
					m_spaceship->plusScore(EnemyDetails::killPoints);
					hit = true;
					break;
				}
			}
			if (hit)
				delete enemy;
			else
				new_m_enemies.push_back(enemy);
		}
	}
	m_enemies = new_m_enemies;

	// update currently existing enemies
	for (auto enemy : m_enemies)
	{
		if (enemy->hasCollided(m_spaceship->getPos()))
		{
			m_spaceship->decreaseHealth(EnemyDetails::harm);
			SpaceshipDetails::wasHit = true;
		}
		enemy->update(timeDelta);
	}

	// pace enemy creation
	if (static_cast<int>(timeTotal) % 2 == 0)
	{
		// create enemies if not more than max
		if (m_enemies.size() < EnemyDetails::EnemyCountMax)
		{
			int numEnemiesToCreate = rand() %
							((EnemyDetails::EnemyCountMax - m_enemies.size())/2);

			Enemy* new_enemy = NULL;

			for (int i = 0; i < numEnemiesToCreate; ++i)
			{
				new_enemy = new Enemy(m_windowBounds);
				printf("POS: %f, %f\n", new_enemy->getPos().x, new_enemy->getPos().y);
				m_enemies.push_back(new_enemy);
			}
		}
	}

	// update player spaceship
	m_spaceship->update(timeDelta);
	if (m_spaceship->getHealth() < 0)
	{
		// PLAYER IS DEAD! print score (m_spaceship->getScore())
		;
	}

	// if there are projectiles outside the screen, destroy them
	std::vector<Projectile*> new_m_projectiles;
	for (auto p : m_projectiles)
	{
		if (p->getPos().x > m_windowBounds.Width)
			delete p;
		else
			new_m_projectiles.push_back(p);
	}
	
	m_projectiles = new_m_projectiles;

	// update projectiles
	for (auto projectile : m_projectiles)
	{
		projectile->update(timeDelta);
	}

	// update score for overlay caption
	std::string score = (static_cast<std::ostringstream*>(
			&( std::ostringstream() << m_spaceship->getScore() )
		)->str());
	std::string health = (static_cast<std::ostringstream*>(
		&(std::ostringstream() << m_spaceship->getHealth())
		)->str());
	std::string bullets = (static_cast<std::ostringstream*>(
		&(std::ostringstream() << ProjectileDetails::MaxBullets - m_projectiles.size())
		)->str());

	std::string newCaption = "Score: "		+ score +
							"\nHealth: "	+ health +
							"\nBullets: "	+ bullets;

	if (m_spaceship->getHealth() == 0)
		newCaption += "\n\n\n\n\n\t\tG A M E   O V E R";

	std::wstring wCaption(newCaption.begin(), newCaption.end());

	OverlayDetails::caption = ref new Platform::String(wCaption.c_str());

	m_sampleOverlay->change_m_sampleName(OverlayDetails::caption);

	// update scrolling background
	m_scroll_background->update();
}

void SimpleSprites::Render()
{
    m_d3dContext->OMSetRenderTargets(
        1,
        m_d3dRenderTargetView.GetAddressOf(),
        nullptr
        );

    m_d3dContext->ClearRenderTargetView(
        m_d3dRenderTargetView.Get(),
        reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::MidnightBlue))
        );

    m_spriteBatch->Begin();

    // Draw the background.
	// dark out if player dead (then game over)
	if (m_spaceship->getHealth() == 0)
	{
		m_scroll_background->draw(m_spriteBatch, float4(0.2f, 0.2f, 0.2f, 1.0f));
	}
	else
	{
		// Game is alive and well.
		m_scroll_background->draw(m_spriteBatch);
	}

    // Draw the non-interactive asteroids.

    for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
    {
        m_spriteBatch->Draw(
            m_asteroid.Get(),
            asteroid->pos,
            PositionUnits::DIPs,
            float2(1.0f, 1.0f) * asteroid->scale,
            SizeUnits::Normalized,
            float4(0.8f, 0.8f, 1.0f, 0.6f),
            asteroid->rot
            );
    }

    // Draw the interactive particles.

    for (auto particle = m_particleData.begin(); particle != m_particleData.begin() + m_numParticlesToDraw; particle++)
    {
        float alpha = length(particle->vel) / 200.0f;
        m_spriteBatch->Draw(
            m_particle.Get(),
            particle->pos,
            PositionUnits::DIPs,
            float2(32.0f, 32.0f),
            SizeUnits::DIPs,
            float4(0.1f, 0.02f, 0.0f, alpha),
            0.0f,
            BlendMode::Additive
            );
    }

	// Render the player
	if (SpaceshipDetails::wasHit)
	{
		if (m_spaceship->getHealth() > 0)
			SpaceshipDetails::wasHit = false;
		m_spriteBatch->Draw(m_spaceship->getTexturePtr(), m_spaceship->getPos(), PositionUnits::DIPs,
			SpaceshipDetails::SpaceshipSize, SizeUnits::DIPs, float4(1.0f, 0.0f, 0.0f, 1.0f), 0.0f);
	}
	else
	{
		m_spriteBatch->Draw(m_spaceship->getTexturePtr(), m_spaceship->getPos(), PositionUnits::DIPs,
			SpaceshipDetails::SpaceshipSize, SizeUnits::DIPs, float4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	}
	

	// Render the enemies
	for (auto enemy : m_enemies)
	{
		m_spriteBatch->Draw(m_enemy_texture.Get(), enemy->getPos(), PositionUnits::DIPs,
			EnemyDetails::EnemySize, SizeUnits::DIPs, float4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	}

	// Render the projectiles
	for (auto projectile : m_projectiles)
	{
		m_spriteBatch->Draw(m_particle.Get(), projectile->getPos(), PositionUnits::DIPs,
			float2(32.0f, 32.0f), SizeUnits::DIPs, float4(1.0f, 0.3f, 1.0f, 0.7f), 0.0f);
	}

    m_spriteBatch->End();

    // Render the Sample Overlay.

    m_sampleOverlay->Render();

	/*
	if (m_spaceship->getHealth() == 0)
	{
		for (;;);
	}//*/
}

float SimpleSprites::RandFloat(float min, float max)
{
    return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}

void SimpleSprites::AddRepulsor(_In_ uint32 id, _In_ float2 position)
{
    m_repulsors[id] = position;
}

void SimpleSprites::MoveRepulsor(_In_ uint32 id, _In_ float2 position)
{
    m_repulsors[id] = position;
}

void SimpleSprites::RemoveRepulsor(_In_ uint32 id)
{
    m_repulsors.erase(id);
}

void SimpleSprites::getKeyresults(Windows::System::VirtualKey key, float timeDelta)
{
	switch (key)
	{
		// change direction of ship (up, down, left, right)
	case Windows::System::VirtualKey::Up:
		m_spaceship->moveShip(SHIP_UP);
		return;
	case Windows::System::VirtualKey::Down:
		m_spaceship->moveShip(SHIP_DOWN);
		return;
	case Windows::System::VirtualKey::Left:
		m_spaceship->moveShip(SHIP_LEFT);
		return;
	case Windows::System::VirtualKey::Right:
		m_spaceship->moveShip(SHIP_RIGHT);
		return;

		// change speed of ship: faster, slower, stop
	case Windows::System::VirtualKey::F:
		m_spaceship->changeSpeed(true);
		return;
	case Windows::System::VirtualKey::S:
		m_spaceship->changeSpeed(false);
		return;
	case Windows::System::VirtualKey::D:
		m_spaceship->stopShip();
		return;

		// fire bullet
	case Windows::System::VirtualKey::Space:
		if (m_spaceship->getHealth())
		{
			// don't fire more bullets if already max
			if (m_projectiles.size() >= ProjectileDetails::MaxBullets)
				return;
			Projectile* new_bullet = new Projectile(m_spaceship->getPos());
			m_projectiles.push_back(new_bullet);
		}
	}
	
	return;
}