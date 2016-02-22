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

#define CREATE_MODE false
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
		void parser(std::string text);
		void setLabels();
		virtual void buttonClicked(Widget* button);
		virtual void editBoxReturn(EditBox* editBox);
	private:
		VerticalLayout* mMainLayout;
		HorizontalLayout* mSides;
		VerticalLayout* mLeft;
		HorizontalLayout* mRight;
		Label* mInstructions;
		EditBox* mUrlBox;
		Button* refresh;
		std::vector<HorizontalLayout*> mLayouts;
		std::vector<Label*> mLabels;
		std::vector<Button*> mButtons;
		int analog_input[4];
		bool digital_input[4];
		bool digital_output[SWITCHES];
		int clicked;
		char mBuffer[BUFSIZE];
		HttpConnection mHttp;
		std::vector<Label*> Digit_r;
};

#endif /* MAINSCREEN_H_ */
