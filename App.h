#pragma once

#include "pch.h"

namespace WOtech_template
{
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		App();

		// IFrameworkView Methods
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		// Application lifecycle event handlers.
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		void OnResuming(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
		void OnWindowActivationChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowActivatedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnStereoEnabledChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

	private:
		bool					WindowClosed;    // change to true when it's time to close the window
		bool					WindowVisible;

		// WOtech devices
		WOtech::DeviceDX11^		m_device;
		WOtech::IRenderer^		m_renderer;
		WOtech::SpriteBatch^	m_spriteBatch;
		WOtech::AudioEngine^	m_audioEngine;
		WOtech::InputManager^	m_inputManager;

		WOtech::GameTimer^		m_gameTimer;

		// Gamestates

		// ClearColor
		Windows::UI::Color		m_clearColor;

		// Assets
		WOtech::Font^			m_font;
	};
};
