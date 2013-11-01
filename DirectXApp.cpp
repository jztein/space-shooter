//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "DirectXApp.h"
#include "BasicTimer.h"

#include "Spaceship.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;

DirectXApp::DirectXApp() :
    m_windowClosed(false),
    m_windowVisible(true),
	m_pause(false)
{
}

void DirectXApp::Initialize(
    _In_ CoreApplicationView^ applicationView
    )
{
    applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &DirectXApp::OnActivated);

    CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &DirectXApp::OnSuspending);

    m_renderer = ref new SimpleSprites();
}

void DirectXApp::SetWindow(
    _In_ CoreWindow^ window
    )
{
    window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

    window->SizeChanged +=
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXApp::OnWindowSizeChanged);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXApp::OnVisibilityChanged);

    window->Closed +=
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &DirectXApp::OnWindowClosed);

    DisplayInformation::GetForCurrentView()->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &DirectXApp::OnDpiChanged);

    window->PointerPressed +=
        ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &DirectXApp::OnPointerPressed);

    window->PointerReleased +=
        ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &DirectXApp::OnPointerReleased);

    window->PointerMoved +=
        ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &DirectXApp::OnPointerMoved);

	// Key events handler
	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXApp::OnKeyDown);

    // Disable all pointer visual feedback for better performance when touching.
    auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
    pointerVisualizationSettings->IsContactFeedbackEnabled = false;
    pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;

    m_renderer->Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
}

void DirectXApp::Load(
    _In_ Platform::String^ entryPoint
    )
{
}

void DirectXApp::Run()
{
    m_timer = ref new BasicTimer();

    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
			m_timer->Update();
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			m_renderer->Update(m_timer->Total, m_timer->Delta);
            m_renderer->Render();
            m_renderer->Present(); // This call is synchronized to the display frame rate.
        }
        else
        {
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

void DirectXApp::Uninitialize()
{
}

void DirectXApp::OnWindowSizeChanged(
    _In_ CoreWindow^ sender,
    _In_ WindowSizeChangedEventArgs^ args
    )
{
    m_renderer->UpdateForWindowSizeChange();
	if (sender->Bounds.Width <= 325) // snap mode == 320px width
		m_timer->pauseTimer();
	else
		m_timer->resumeTimer();
}

void DirectXApp::OnSuspending(
    _In_ Platform::Object^ sender,
    _In_ SuspendingEventArgs^ args
    )
{
    // Hint to the driver that the app is entering an idle state and that its memory
    // can be temporarily used for other apps.
    m_renderer->Trim();
}

void DirectXApp::OnWindowClosed(
    _In_ CoreWindow^ sender,
    _In_ CoreWindowEventArgs^ args
    )
{
    m_windowClosed = true;
}

void DirectXApp::OnVisibilityChanged(
    _In_ CoreWindow^ sender,
    _In_ VisibilityChangedEventArgs^ args
    )
{
    m_windowVisible = args->Visible;
}

void DirectXApp::OnDpiChanged(_In_ DisplayInformation^ sender, _In_ Platform::Object^ args)
{
    m_renderer->SetDpi(sender->LogicalDpi);
}

void DirectXApp::OnActivated(
    _In_ CoreApplicationView^ applicationView,
    _In_ IActivatedEventArgs^ args
    )
{
    CoreWindow::GetForCurrentThread()->Activate();
}

void DirectXApp::OnPointerPressed(
    _In_ Windows::UI::Core::CoreWindow^ sender,
    _In_ Windows::UI::Core::PointerEventArgs^ args
    )
{
	// record for swiping direction in OnPointerMoved()
	m_lastPointerPos = float2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);

	// fire projectile (hack)
	m_renderer->getKeyresults(VirtualKey::Space, 0.02);

	/*
    m_renderer->AddRepulsor(
        args->CurrentPoint->PointerId,
        float2(
            args->CurrentPoint->Position.X,
            args->CurrentPoint->Position.Y
            )
        );//*/
}

void DirectXApp::OnPointerReleased(
    _In_ Windows::UI::Core::CoreWindow^ sender,
    _In_ Windows::UI::Core::PointerEventArgs^ args
    )
{
	// stop (since it's harder to control touch screen, make it easier
	m_renderer->getKeyresults(VirtualKey::D, 0.02);

   // m_renderer->RemoveRepulsor(args->CurrentPoint->PointerId);
}

void DirectXApp::OnPointerMoved(
    _In_ Windows::UI::Core::CoreWindow^ sender,
    _In_ Windows::UI::Core::PointerEventArgs^ args
    )
{
	float2 curPos(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);
	float2 delta = curPos- m_lastPointerPos;
	m_lastPointerPos = curPos;

	// moving left/ right is less necessary and should be restricted
	// in order to make touch controls easier.
	if (delta.y == 0)
	{
		return;
	}
	// move up or down
	if (delta.x == 0)
	{
		if (delta.y > 0)
			m_renderer->getKeyresults(VirtualKey::Down, 0.02);
		else
			m_renderer->getKeyresults(VirtualKey::Up, 0.02);
		return;
	}
	float gradient = delta.y / delta.x;
	if (gradient > 0)
		m_renderer->getKeyresults(VirtualKey::Down, 0.02);
	else
		m_renderer->getKeyresults(VirtualKey::Up, 0.02);
    /*if (args->CurrentPoint->IsInContact)
    {
        m_renderer->MoveRepulsor(
            args->CurrentPoint->PointerId,
            float2(
                args->CurrentPoint->Position.X,
                args->CurrentPoint->Position.Y
                )
            );
    }*/
}

void DirectXApp::OnKeyDown(
	_In_ Windows::UI::Core::CoreWindow^ sender,
	_In_ Windows::UI::Core::KeyEventArgs^ args
	)
{
	if (args->VirtualKey == VirtualKey::P)
	{
		m_timer->toggleStop();
	}
	m_renderer->getKeyresults(args->VirtualKey, 0.02);
}

IFrameworkView^ DirectXAppSource::CreateView()
{
    return ref new DirectXApp();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto directXAppSource = ref new DirectXAppSource();
    CoreApplication::Run(directXAppSource);
    return 0;
}
