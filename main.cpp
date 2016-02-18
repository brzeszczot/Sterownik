#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class
#include <conprint.h>			// lprintfln for logging
#include <NativeUI/Widgets.h>	// Include all widgets
#include "MainScreen.h"			// Main UI screen

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet for the  application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * Constructor that creates the UI.
	 */
	NativeUIMoblet()
	{
		// Create the main user interface screen.
		mMainScreen = new MainScreen();

		// Display the NativeUI screen.
		// Note: This would hide any previously visible screen.
		mMainScreen->show();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete mMainScreen;
	}

	/**
	 * This method is called when the application is closed.
	 */
	void NativeUIMoblet::closeEvent()
	GCCATTRIB(noreturn)
	{
		// Deallocate the main screen.
		delete mMainScreen;
		mMainScreen = NULL;

		// Exit the app.
		close();
	}

	/**
	 * Method called when a key is pressed.
	 */
	void NativeUIMoblet::keyPressEvent(int keyCode, int nativeCode)
	{
	    // Close the application if the back key is pressed.
	    if(MAK_BACK == keyCode)
	    {
	        closeEvent();
	    }
	}

private:
	MainScreen* mMainScreen;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	NativeUIMoblet* moblet = new NativeUIMoblet();

	// Run the moblet event loop.
	Moblet::run(moblet);

	// Deallocate objects.
	delete moblet;

	return 0;
}
