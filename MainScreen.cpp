#include "MainScreen.h"

MainScreen::MainScreen(): mHttp(this),
Screen()
{
	createUI();

	mP1Button->addButtonListener(this);
	mP2Button->addButtonListener(this);
	mUrlBox->addEditBoxListener(this);
}

MainScreen::~MainScreen()
{
	mP1Button->removeButtonListener(this);
	mP2Button->removeButtonListener(this);
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
	mInstructions->setText("Sterownik ETH");
	mMainLayout->addChild(mInstructions);

	mUrlBox = new EditBox();
	mUrlBox->fillSpaceHorizontally();
	mUrlBox->wrapContentVertically();
	mUrlBox->setText("http://10.52.106.69/ps/Transcription/cronjobs/BMSOpdivoReg_AS3.php?env=dev");//http://192.168.0.150/
	//mPasswordBox->setInputFlag(EDIT_BOX_INPUT_FLAG_PASSWORD);
	mMainLayout->addChild(mUrlBox);



	mLayoutP1 = new HorizontalLayout();
	mLayoutP1->fillSpaceHorizontally();
	mLayoutP1->wrapContentVertically();
	//mLayoutP1->fillSpaceVertically();

	mP1Button = new Button();
	mP1Button->wrapContentVertically();
	mP1Button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mP1Button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mP1Button->setText("Przekaźnik 1");
	mLayoutP1->addChild(mP1Button);

	mLabelP1 = new Label();
	//mLabelP1->fillSpaceHorizontally();
	mLabelP1->wrapContentVertically();
	mLabelP1->setText("Włączony");
	mLayoutP1->addChild(mLabelP1);

	mMainLayout->addChild(mLayoutP1);



	mLayoutP2 = new HorizontalLayout();
	mLayoutP2->fillSpaceHorizontally();
	mLayoutP2->wrapContentVertically();
	//mLayoutP1->fillSpaceVertically();

	mP2Button = new Button();
	mP2Button->wrapContentVertically();
	mP2Button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mP2Button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mP2Button->setText("Przekaźnik 2");
	mLayoutP2->addChild(mP2Button);

	mLabelP2 = new Label();
	//mLabelP2->fillSpaceHorizontally();
	mLabelP2->wrapContentVertically();
	mLabelP2->setText("Włączony");
	mLayoutP2->addChild(mLabelP2);

	mMainLayout->addChild(mLayoutP2);
}

void MainScreen::buttonClicked(Widget* button)
{
	if ( mP1Button == button )
	{
		MAUtil::String password = mUrlBox->getText();
		start(password.c_str());
		//start("http://brzeszczot.net/");

		//clearButtonClicked();
	}
	else if ( mP2Button == button )
	{
		//mUrlBox->hideKeyboard();
		//submitEditBoxContent();
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

		mInstructions->setText(mBuffer);
	}
	else
	{
		printf("No content-length\n");
		mHttp.recv(mBuffer, BUFSIZE);

		//printf("!!!! %s", mBuffer);


		mInstructions->setText(mBuffer);
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
