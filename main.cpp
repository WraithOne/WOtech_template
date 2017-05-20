// Include the precompiled headers
#include "pch.h"
#include "App.h"

// Use some common namespaces to simplify the code
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;
using namespace WOtech_template;


// the class definition that creates our core framework
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();    // create the framework view and return it
	}
};

[MTAThread]    // define main() as a multi-threaded-apartment function
int main(Array<String^>^ args) // the starting point of all programs
{
	UNREFERENCED_PARAMETER(args);
	CoreApplication::Run(ref new AppSource());    // run the framework view source
	return 0;
}