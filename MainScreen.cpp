#include "MainScreen.h"

MainScreen::MainScreen(): mHttp(this),
Screen()
{
	createUI();

	start(mUrlBox->getText().c_str());

	for(int ii = 0; ii < SWITCHES; ii++)
		mButtons[ii]->addButtonListener(this);

	refresh->addButtonListener(this);
	mUrlBox->addEditBoxListener(this);
}

MainScreen::~MainScreen()
{
	for(int ii = 0; ii < SWITCHES; ii++)
		mButtons[ii]->removeButtonListener(this);

	refresh->removeButtonListener(this);
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
	mInstructions->setText("Sterownik ETH by Brzeszczot, 2016");
	mMainLayout->addChild(mInstructions);

	mUrlBox = new EditBox();
	mUrlBox->fillSpaceHorizontally();
	mUrlBox->wrapContentVertically();

	if(CREATE_MODE)
		mUrlBox->setText("http://brzeszczot.net/open/_ignore.php");
	else
		mUrlBox->setText("http://192.168.0.150/Pio?");

	//mPasswordBox->setInputFlag(EDIT_BOX_INPUT_FLAG_PASSWORD);
	mMainLayout->addChild(mUrlBox);

	refresh = new Button();
	refresh->fillSpaceHorizontally();
	//refresh->wrapContentHorizontally();
	refresh->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	refresh->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	refresh->setText("Odśwież WE/WY");
	mMainLayout->addChild(refresh);

	mSides = new HorizontalLayout();
	//mSides->fillSpaceHorizontally();
	//mSides->fillSpaceVertically();
	mLeft = new VerticalLayout();
	//mLeft->fillSpaceHorizontally();
	//mLeft->fillSpaceVertically();
	mRight = new HorizontalLayout();
	mRight->setWidth(200);
	//mRight->fillSpaceHorizontally();
	//mRight->fillSpaceVertically();

	mSides->addChild(mLeft);
	mSides->addChild(mRight);
	mMainLayout->addChild(mSides);

	for(int ii = 0; ii < SWITCHES; ii++)
	{
		HorizontalLayout* layout = new HorizontalLayout();
		layout->fillSpaceHorizontally();
		//layout->wrapContentVertically();

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
		//label->fillSpaceHorizontally();
		label->wrapContentVertically();
		label->setText("--");
		layout->addChild(label);

		mLayouts.push_back(layout);
		mLabels.push_back(label);
		mButtons.push_back(button);

		mLeft->addChild(layout);
	}

	VerticalLayout* mResultsLeft = new VerticalLayout();
	VerticalLayout* mResultsRight = new VerticalLayout();
	mRight->addChild(mResultsLeft);
	mRight->addChild(mResultsRight);

	Label* Digit_1 = new Label();
	Digit_1->setText("Cyfr 1:");
	mResultsLeft->addChild(Digit_1);
	Label* Digit_2 = new Label();
	Digit_2->setText("Cyfr 2:");
	mResultsLeft->addChild(Digit_2);
	Label* Digit_3 = new Label();
	Digit_3->setText("Cyfr 3:");
	mResultsLeft->addChild(Digit_3);
	Label* Digit_4 = new Label();
	Digit_4->setText("Cyfr 4:");
	mResultsLeft->addChild(Digit_4);

	Label* Digit_5 = new Label();
	Digit_5->setText("Analog 5:");
	mResultsLeft->addChild(Digit_5);
	Label* Digit_6 = new Label();
	Digit_6->setText("Analog 6:");
	mResultsLeft->addChild(Digit_6);
	Label* Digit_7 = new Label();
	Digit_7->setText("Analog 7:");
	mResultsLeft->addChild(Digit_7);
	Label* Digit_8 = new Label();
	Digit_8->setText("Analog 8:");
	mResultsLeft->addChild(Digit_8);

	for(int jj = 0; jj < 8; jj++)
	{
		Digit_r[jj] = new Label();
		Digit_r[jj]->setText("--");
		mResultsRight->addChild(Digit_r[jj]);
	}
}

void MainScreen::buttonClicked(Widget* button)
{
	MAUtil::String url = mUrlBox->getText();

	if(button == refresh)
	{
		start(url.c_str());
		return;
	}

	for(int ii = 0; ii < SWITCHES; ii++)
	{
		if(button == mButtons[ii])
		{
			char buffor[1000];
			char buffor_int[10];
			strcpy(buffor, url.c_str());
			strcat(buffor, "sw=pa");
			itoa((int)ii, buffor_int, 10);
			strcat(buffor, buffor_int);
			digital_output[ii] ? strcat(buffor, "&a=0") : strcat(buffor, "&a=1");

			//mInstructions->setText(buffor);

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

		this->parser(std::string(mBuffer));
	}
	else
	{
		printf("No content-length\n");
		mHttp.recv(mBuffer, BUFSIZE);
		//printf("!!!! %s", mBuffer);

		this->parser(std::string(mBuffer));
	}
}

void MainScreen::parser(std::string text)
{
	std::string pa_patern[SWITCHES];

	pa_patern[0].append("PA0 ");
	pa_patern[1].append("PA1 ");
	pa_patern[2].append("PA2 ");
	pa_patern[3].append("PA3 ");
	pa_patern[4].append("PA4 ");
	pa_patern[5].append("PA5 ");
	pa_patern[6].append("PA6 ");
	pa_patern[7].append("PA7 ");

	std::string tag_digital_input("<h2>Digital Input:</h2>");
	std::string tag_analog_input_0("ADC0: ");
	std::string tag_analog_input_1("ADC1: ");
	std::string tag_analog_input_2("ADC2: ");
	std::string tag_analog_input_3("ADC3: ");

	std::size_t found;

	// parse 8 outups
	for(int ii = 0; ii < SWITCHES; ii++)
	{
		found = text.find(pa_patern[ii]);

		if(found != std::string::npos)
		{
			pa_patern[ii] = text.substr(found + 4, 2);
			pa_patern[ii] == "on" ? digital_output[ii] = true : digital_output[ii] = false;
			digital_output[ii] ? mLabels[ii]->setText("WŁ") : mLabels[ii]->setText("WYŁ");
		}
	}

	// parse digital input
	found = text.find(tag_digital_input);
	if(found != std::string::npos)
	{
		tag_digital_input = text.substr(found + 23, 4);

		for(int jj = 0; jj < 4; jj++)
		{
			tag_digital_input[jj] == '1' ? digital_input[jj] = true : digital_input[jj] = false;
		}
	}

	// parse analog input
	found = text.find(tag_analog_input_0);
	if(found != std::string::npos)
	{
		tag_analog_input_0 = text.substr(found + 6, 4);
		analog_input[0] = atoi (tag_analog_input_0.c_str());
	}
	found = text.find(tag_analog_input_1);
	if(found != std::string::npos)
	{
		tag_analog_input_1 = text.substr(found + 6, 4);
		analog_input[1] = atoi (tag_analog_input_1.c_str());
	}
	found = text.find(tag_analog_input_2);
	if(found != std::string::npos)
	{
		tag_analog_input_2 = text.substr(found + 6, 4);
		analog_input[2] = atoi (tag_analog_input_2.c_str());
	}
	found = text.find(tag_analog_input_3);
	if(found != std::string::npos)
	{
		tag_analog_input_3 = text.substr(found + 6, 4);
		analog_input[3] = atoi (tag_analog_input_3.c_str());
	}

	setLabels();
}

void MainScreen::setLabels()
{
	for(int jj = 0; jj < 4; jj++)
	{
		digital_input[jj] ? Digit_r[jj]->setText("TAK") : Digit_r[jj]->setText("NIE");
	}

	for(int jj = 0; jj < 4; jj++)
	{
		char buffor[10];
		itoa((int)analog_input[jj], buffor, 10);
		Digit_r[jj + 4]->setText(buffor);
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
