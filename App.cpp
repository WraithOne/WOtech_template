#include "pch.h"
#include "App.h"

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace WOtech_template;

App::App()
{
}

void App::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

	CoreApplication::Resuming += ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

	ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::FullScreen;// todo
	WindowClosed = false;    // initialize to false
}

void App::SetWindow(CoreWindow^ window)
{
	window->Activated += ref new TypedEventHandler<CoreWindow^, WindowActivatedEventArgs^>(this, &App::OnWindowActivationChanged);

	window->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

	window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

	window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &App::OnDpiChanged);

	currentDisplayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnOrientationChanged);

	currentDisplayInformation->StereoEnabledChanged += ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &App::OnStereoEnabledChanged);

	DisplayInformation::DisplayContentsInvalidated += ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &App::OnDisplayContentsInvalidated);
}

void App::Load(Platform::String^ /* entryPoint */)
{
	CoreWindow^ Window = CoreWindow::GetForCurrentThread();

	// DeviceDX11
	m_device = ref new WOtech::DeviceDX11();
	m_device->Create();

	// 3D Renderer
	m_renderer = ref new WOtech::ForwardRenderer(m_device);
	
	// SpriteBatch
	m_spriteBatch = ref new WOtech::SpriteBatch(m_device);
	
	// Input
	m_inputManager = ref new WOtech::InputManager();
	// Audio
	m_audioEngine = ref new WOtech::AudioEngine();
	m_audioEngine->Initialize();

}

void App::Run()
{
	CoreWindow^ Window = CoreWindow::GetForCurrentThread();

	// repeat until window closes
	while (!WindowClosed)
	{	
		// Debug exit
		if (m_inputManager->KeyDown(Windows::System::VirtualKey::F9))
			WindowClosed = true;

		if (WindowVisible)
		{
			// Dispatch all Window events
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// Render
			m_renderer->Begin();
			{
				m_spriteBatch->BeginDraw();
				{
				}
				m_spriteBatch->EndDraw();
			}
			m_renderer->Present();
		}
		else
		{
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void App::Uninitialize()
{
}

void App::OnStereoEnabledChanged(DisplayInformation^ /* sender */, Platform::Object^ /* args */)
{
}

void App::OnWindowSizeChanged(CoreWindow^ window, WindowSizeChangedEventArgs^  /* args */)
{
	WOtech::SystemManager::Instance->OnWindowSizeChanged();
}

void App::OnWindowClosed(CoreWindow^ /* sender */, CoreWindowEventArgs^ /* args */)
{
	WindowClosed = true;    // time to end the endless loop
}

void App::OnDpiChanged(DisplayInformation^ sender, Platform::Object^ /* args */)
{
	WOtech::SystemManager::Instance->OnDpiChanged(sender->LogicalDpi);
}

void App::OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ /* args */)
{
	WOtech::SystemManager::Instance->OnOrientationChanged(sender->CurrentOrientation);
}

void App::OnDisplayContentsInvalidated(DisplayInformation^ /* sender */, Platform::Object^ /* args */)
{
	WOtech::SystemManager::Instance->OnDisplayContentsInvalidated();
}

void App::OnActivated(CoreApplicationView^ /* applicationView */, IActivatedEventArgs^ /* args */)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void App::OnVisibilityChanged(CoreWindow^ /* sender */, VisibilityChangedEventArgs^ args)
{
	WindowVisible = args->Visible;
}

void App::OnWindowActivationChanged(Windows::UI::Core::CoreWindow^ /* sender */, Windows::UI::Core::WindowActivatedEventArgs^ /* args */)
{
}

void App::OnSuspending(Platform::Object^ /* sender */, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.

	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		WOtech::ContentManager::Instance->OnSuspending();
		WOtech::SystemManager::Instance->OnSuspending();
		deferral->Complete();
	});
}

void App::OnResuming(Platform::Object^ /* sender */, Platform::Object^ /* args */)
{
	WOtech::SystemManager::Instance->OnResume();
	WOtech::ContentManager::Instance->OnResume(m_spriteBatch);
}