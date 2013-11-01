//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "BasicTimer.h"

BasicTimer::BasicTimer()
{
	m_stopTimer = false;
    if (!QueryPerformanceFrequency(&m_frequency))
    {
        throw ref new Platform::FailureException();
    }
    Reset();
}

void BasicTimer::toggleStop()
{
	if (m_stopTimer)
	{
		LARGE_INTEGER curTime;
		if (!QueryPerformanceCounter(&curTime))
		{
			throw ref new Platform::FailureException();
		}
		m_lastTime.QuadPart = curTime.QuadPart - static_cast<LONGLONG>(m_delta);
	}
	m_stopTimer = !m_stopTimer;
}

void BasicTimer::pauseTimer()
{
	m_stopTimer = false;
	toggleStop();
}

void BasicTimer::resumeTimer()
{
	m_stopTimer = true;
	toggleStop();
}

void BasicTimer::Reset()
{
    Update();
    m_startTime = m_currentTime;
    m_total = 0.0f;
    m_delta = 1.0f / 60.0f;
}

void BasicTimer::Update()
{
	if (m_stopTimer)
		return;
    if (!QueryPerformanceCounter(&m_currentTime))
    {
        throw ref new Platform::FailureException();
    }

    m_total = static_cast<float>(
        static_cast<double>(m_currentTime.QuadPart-m_startTime.QuadPart) /
        static_cast<double>(m_frequency.QuadPart)
        );

    if (m_lastTime.QuadPart == m_startTime.QuadPart)
    {
        // If the timer was just reset, report a time delta equivalent to 60Hz frame time.
        m_delta = 1.0f / 60.0f;
    }
    else
    {
        m_delta = static_cast<float>(
            static_cast<double>(m_currentTime.QuadPart-m_lastTime.QuadPart) /
            static_cast<double>(m_frequency.QuadPart)
            );
    }

    m_lastTime = m_currentTime;
}

float BasicTimer::Total::get()
{
	if (m_stopTimer)
		return 0.0f;
    return m_total;
}

float BasicTimer::Delta::get()
{
	if (m_stopTimer)
		return 0.0f;
    return m_delta;
}
