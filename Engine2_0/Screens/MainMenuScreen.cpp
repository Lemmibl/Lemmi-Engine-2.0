#include "MainMenuScreen.h"

#include "ScreenStates.h"

MainMenuScreen::MainMenuScreen()
: ScreenBaseClass()
{
	rootWindow = nullptr;
}

MainMenuScreen::~MainMenuScreen()
{
	rootWindow = nullptr;
}

bool MainMenuScreen::Enter()
{
	SetActivated(true);
	bool result;

	if(!HasBeenInitialized())
	{
		result = Initialize();
		if(!result)
		{
			return false;
		}

		SetInitialized(true);
	}

	//Show mouse cursor
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(true);

	//Set this base window as root.
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);

	rootWindow->activate();
	rootWindow->show();

	return true;
}

void MainMenuScreen::Exit()
{
	SetActivated(false);

	//Hide mouse cursor
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(false);

	//When exiting, hide and deactivate window
	rootWindow->hide();
}

bool MainMenuScreen::Initialize()
{
	//Load the layout for this menu
	rootWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("MainMenu.layout");
	if(!rootWindow)
	{
		return false;
	}
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);

	//Set up start game function and bind it to the options button
	auto& startGameFunction = 
		[&](const CEGUI::EventArgs& args) -> bool
	{
		//Throws an event that will be caught by screenManager, signalling to quit the game
		stateChangeEvent(ScreenStates::InGame);

		return true;
	};

	CEGUI::Window* startGame = rootWindow->getChild("Start Game");
	if(!startGame)
	{
		return false;
	}
	startGame->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::SubscriberSlot::SubscriberSlot(startGameFunction));

	//Set up quit function and bind it to Exit Game button event
	auto& quitFunction = 
		[&](const CEGUI::EventArgs& args) -> bool
	{
		//Throws an event that will be caught by screenManager, signalling to quit the game
		stateChangeEvent(ScreenStates::Quit);

		return true;
	};

	CEGUI::Window* quitGame = rootWindow->getChild("Exit");
	if(!quitGame)
	{
		return false;
	}
	quitGame->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::SubscriberSlot::SubscriberSlot(quitFunction));

	return true;
}


bool MainMenuScreen::Update(double deltaTime )
{
	////If escape was pressed from the main menu, throw quitGame event
	//if(input->WasKeyPressed(DIK_ESCAPE))
	//{
	//	stateChangeEvent(GameStates::QuitGame);
	//}

	return true;
}

bool MainMenuScreen::Render(double deltaTime )
{
	//Not much going on here either; it's all managed by internal CEGUI rendering

	return true;
}