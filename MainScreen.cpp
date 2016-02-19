#include "MainScreen.h"

MainScreen::MainScreen(): mHttp(this),
Screen()
{
	createUI();

	for(int ii = 0; ii < SWITCHES; ii++)
		mButtons[ii]->addButtonListener(this);

	mOnOff->addButtonListener(this);
	mUrlBox->addEditBoxListener(this);
}

MainScreen::~MainScreen()
{
	for(int ii = 0; ii < SWITCHES; ii++)
		mButtons[ii]->removeButtonListener(this);

	mOnOff->removeButtonListener(this);
	mUrlBox->removeEditBoxListener(this);
}

void MainScreen::createUI()
{
	mMainLayout = new VerticalLayout();
	// Make the layout fill the entire screen.
	mMainLayout->fillSpaceHorizontally();
	mMainLayout->fillSpaceVertically();

	Screen::setMainWidget(mMainLayout);

	mInstructions = new Label();
	// Make the label fill the width of the parent layout and
	// adjust its height to "shrink wrap" the size of the text.
	mInstructions->fillSpaceHorizontally();
	mInstructions->wrapContentVertically();
	mInstructions->setText("Sterownik ETH by Brzeszczot");
	mMainLayout->addChild(mInstructions);

	mUrlBox = new EditBox();
	mUrlBox->fillSpaceHorizontally();
	mUrlBox->wrapContentVertically();
	mUrlBox->setText("http://192.168.0.150/"); // http://brzeszczot.net/open/_ignore.php
	//mPasswordBox->setInputFlag(EDIT_BOX_INPUT_FLAG_PASSWORD);
	mMainLayout->addChild(mUrlBox);

	mOnOff = new Button();
	mOnOff->fillSpaceHorizontally();
	//mOnOff->wrapContentHorizontally();
	mOnOff->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mOnOff->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mOnOff->setText("Włącz");
	mMainLayout->addChild(mOnOff);
	onoff_mode = true;

	for(int ii = 0; ii < SWITCHES; ii++)
	{
		HorizontalLayout* layout = new HorizontalLayout();
		layout->fillSpaceHorizontally();
		layout->wrapContentVertically();

		Button* button = new Button();
		button->wrapContentVertically();
		button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		char buffor[32];
		char buffor_int[10];
		strcpy(buffor,"Przekaźnik nr ");
		itoa((int)ii + 1, buffor_int, 10);
		strcat(buffor,buffor_int);
		button->setText(buffor);
		layout->addChild(button);

		Label* label = new Label();
		label->wrapContentVertically();
		label->setText("Włączony");
		layout->addChild(label);

		mLayouts.push_back(layout);
		mLabels.push_back(label);
		mButtons.push_back(button);

		mMainLayout->addChild(layout);
	}
}

void MainScreen::buttonClicked(Widget* button)
{
	if(button == mOnOff)
	{
		onoff_mode = !onoff_mode;
		onoff_mode ? mOnOff->setText("Włącz") : mOnOff->setText("Wyłącz");
	}

	MAUtil::String url = mUrlBox->getText();

	for(int ii = 0; ii < SWITCHES; ii++)
	{
		if(button == mButtons[ii])
		{
			char buffor[1000];
			char buffor_int[10];
			strcpy(buffor, url.c_str());
			//strcat(buffor, "?get=");
			strcat(buffor, "Pio?sw=pa");
			itoa((int)ii, buffor_int, 10);
			strcat(buffor, buffor_int);
			onoff_mode ? strcat(buffor, "&a=1") : strcat(buffor, "&a=0");

			//mInstructions->setText(buffor);
			clicked = ii;
			start(buffor);
			break;
		}
	}
}

void MainScreen::editBoxReturn(EditBox* editBox)
{
	mUrlBox->hideKeyboard();
}

void MainScreen::start(const char* url)
{
	printf("Connecting to,\n%s...\n", url);

	mHttp.close();
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0)
	{
		printf("http.create fail %i\n", res);
	}
	else
	{
		mHttp.finish();
	}
}

/**
* Listener that is fired when a connection has been established.
* The function tries to read the content-length from the
* resoonse headers. If successfull, the first 1024 bytes
* will be read.
*/
void MainScreen::httpFinished(HttpConnection* http, int result)
{
	printf("HTTP %i\n", result);
	if(result <= 0)
	{
		return;
	}
	String cl;
	int res = mHttp.getResponseHeader("content-length", &cl);

	if(res > 0) {
		printf("content-length: %s bytes\n", cl.c_str());
		int len = atoi(cl.c_str());
		if(len >= BUFSIZE) {
			//printf("Buffer too small, can't read everything.\n");
			len = BUFSIZE-1;
		}
		mBuffer[len] = 0;
		mHttp.read(mBuffer, len);

		mLabels[clicked]->setText(mBuffer);
	}
	else
	{
		printf("No content-length\n");
		mHttp.recv(mBuffer, BUFSIZE);
		//printf("!!!! %s", mBuffer);

		mLabels[clicked]->setText(mBuffer);
	}
}

/**
* Listener that is fired when content with a known length has been read.
*/
void MainScreen::connReadFinished(Connection* conn, int result)
{
	//printf("connReadFinished result: %i\n\n", result);
}

/**
* Listener that is fired when content with unknown length has been read.
*/
void MainScreen::connRecvFinished(Connection* conn, int result)
{
	//printf("connRecvFinished result: %i\n\n", result);
}
