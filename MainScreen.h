#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <string>
#include <vector>
#include <maxtoa.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <NativeUI/Widgets.h>

#include <MAUtil/Moblet.h>		// Moblet class
#include <MAUtil/Connection.h>

using namespace NativeUI;
using namespace MAUtil;

#define BUFSIZE		1024
#define SWITCHES	8

class MainScreen :
	public Screen,
	public ButtonListener,
	public EditBoxListener,
	private HttpConnectionListener
{
	public:
		MainScreen();
		~MainScreen();
		void start(const char* url);
		virtual void httpFinished(HttpConnection* http, int result);
		virtual void connReadFinished(Connection* conn, int result);
		virtual void connRecvFinished(Connection* conn, int result);
	private:
		void createUI();
		virtual void buttonClicked(Widget* button);
		virtual void editBoxReturn(EditBox* editBox);
	private:
		VerticalLayout* mMainLayout;
		Label* mInstructions;
		EditBox* mUrlBox;
		Button* mOnOff;
		bool onoff_mode;
		std::vector<HorizontalLayout*> mLayouts;
		std::vector<Label*> mLabels;
		std::vector<Button*> mButtons;
		int clicked;
		char mBuffer[BUFSIZE];
		HttpConnection mHttp;
};

#endif /* MAINSCREEN_H_ */
