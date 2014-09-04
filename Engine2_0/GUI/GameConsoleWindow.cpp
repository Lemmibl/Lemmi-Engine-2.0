#include "GameConsoleWindow.h"

#include "FormattedListboxTextItem.h"
#include "CEGUI/WindowFactoryManager.h"

int GameConsoleWindow::iInstanceNumber;            // Don't forget this declaration

//TODO: Take a rootWnd ptr here?
GameConsoleWindow::GameConsoleWindow()
{
	m_ConsoleWindow = NULL;       // Always good practice to initialize a pointer to a NULL value, helps when switching to Release Builds.
	iInstanceNumber = 0;
	sNamePrefix = CEGUI::String("");
}

GameConsoleWindow::~GameConsoleWindow()
{
}

void GameConsoleWindow::Reset()
{
	static_cast<CEGUI::Listbox*>(m_ConsoleWindow->getChild("History"))->resetList();
	m_ConsoleWindow->getChild("Editbox")->setText("");
}


void GameConsoleWindow::CreateCEGUIWindow(CEGUI::String filePath, CEGUI::Window* rootWindow)
{
	// Get a local pointer to the CEGUI Window Manager, Purely for convenience to reduce typing
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();

	// Now before we load anything, lets increase our instance number to ensure no conflicts.  
	// I like the format #_ConsoleRoot so thats how i'm gonna make the prefix.  This simply
	// Increments the iInstanceNumber then puts it + a _ into the sNamePrefix string. 
	sNamePrefix = ++iInstanceNumber + "_";//"";

	// Now that we can ensure that we have a safe prefix, and won't have any naming conflicts lets create the window
	// and assign it to our member window pointer m_ConsoleWindow
	// inLayoutName is the name of your layout file (for example "console.layout"), don't forget to rename inLayoutName by our layout file
	m_ConsoleWindow = pWindowManager->loadLayoutFromFile(filePath);

	// Being a good programmer, its a good idea to ensure that we got a valid window back. 
	if(m_ConsoleWindow)
	{
		if(rootWindow != nullptr)
		{
			// Lets add our new window to the Root GUI Window
			//TODO: ... root gui window..
			rootWindow->addChild(m_ConsoleWindow);

			m_ConsoleWindow->setClippedByParent(false);
			m_ConsoleWindow->setAlwaysOnTop(true);
		}
		else
		{
			//In case we didn't recieve a real root window ptr, we set this window as root.
			CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_ConsoleWindow);

			//Copy root so that we can show/hide this window from outside of the class etc.
			rootWindow = m_ConsoleWindow;
		}

		// Now register the handlers for the events (Clicking, typing, etc)
		RegisterHandlers();

		setVisible(true);
		m_bConsole = false;
	}
	else
	{
		// Something bad happened and we didn't successfully create the window lets output the information
		CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the ConsoleWindow from .layout");
	}
}

void GameConsoleWindow::InitializeFromExternalWindow( CEGUI::Window* rootWindow )
{
	sNamePrefix = ++iInstanceNumber + "_";

	// Now that we can ensure that we have a safe prefix, and won't have any naming conflicts lets create the window
	// and assign it to our member window pointer m_ConsoleWindow
	// inLayoutName is the name of your layout file (for example "console.layout"), don't forget to rename inLayoutName by our layout file
	m_ConsoleWindow = rootWindow->getChild("Console");

	// Being a good programmer, its a good idea to ensure that we got a valid window back. 
	if(m_ConsoleWindow)
	{
		// Now register the handlers for the events (Clicking, typing, etc)
		RegisterHandlers();

		setVisible(false);
		m_bConsole = false;
	}
	else
	{
		// Something bad happened and we didn't successfully create the window lets output the information
		CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the ConsoleWindow from .layout");
	}
}

void GameConsoleWindow::RegisterHandlers()
{
	// Alright now we need to register the handlers.  We mentioned above we want to acknowledge when the user presses Enter, 
	// as well as when they click the 'Send' button.  So we need to register each of those events

	auto* sendButton = m_ConsoleWindow->getChild("Submit");
	sendButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameConsoleWindow::EventCallback_TextSent, this));

	auto* editBox = m_ConsoleWindow->getChild("Editbox");
	editBox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&GameConsoleWindow::EventCallback_TextSent, this));

	//Get ptr to the vertical scrollbar on the history window
	CEGUI::Scrollbar* scrollBar = static_cast<CEGUI::Listbox*>(m_ConsoleWindow->getChild(sNamePrefix + "History"))->getVertScrollbar();

	//Tweak some settings
	scrollBar->setEndLockEnabled(true);

	//These settings control how far you scroll every time you push the up/down buttons
	//scrollBar->setOverlapSize(100.0f);
	scrollBar->setStepSize(5.0f);

	const float scrollStepSize = 0.01f;

	scrollBar->setConfig(0, 0, &scrollStepSize, 0, 0);
}

void GameConsoleWindow::PrintText(CEGUI::String inMsg, CEGUI::Colour& textColour /*= SystemColour*/ )
{
	//Pass through string to internal function, and we use the "internal system" colour!
	OutputText(inMsg, textColour);
}


void GameConsoleWindow::ParseText(CEGUI::String inMsg)
{
	//See if this string is a "command"
	if(inMsg[0] == '/')
	{
		if (inMsg.length() >= 1) // Be sure we've got a string longer than 0
		{
			size_t stringSize = inMsg.size();
			CEGUI::String command = inMsg.substr(1, stringSize-1);

			//convert command message to lower case. (Significantly simplifies string comparison further down).
			for(unsigned int i = 0; i < command.length(); ++i)
			{
				command[i] = tolower(command[i]);
			}

			// Begin processing

			//if (command == "say")
			//{
			//	////"Optimal" way to replace the /say command with a "You:"
			//	//inString[0] = 'Y';
			//	//inString[1] = 'o';
			//	//inString[2] = 'u';
			//	//inString[3] = ':';

			//	CEGUI::String outString = userName + ": " + inString.substr(4); // Create a new string with your "name" and only add the part of the string that isn't a command
			//	OutputText(outString, textColour);
			//}
			//else 
			if (command == "help")
			{
				OutputText("Do you want <help>? HA! There is none! Despair!",  SystemColour);
			}
			else if (command == "yell")
			{
				OutputText("You: AIEEEEEE!", ErrorColour);
			}
			else
			{
				CEGUI::String outString = "<" + inMsg + "> is an invalid command.";
				OutputText(outString, ErrorColour); // With red ANGRY colors!
			}
		}
	}
	else
	{
		OutputText(inMsg); // no commands, just output what was written
	}
}

void GameConsoleWindow::OutputText(CEGUI::String inMsg, CEGUI::Colour& colour)
{

	// Get a pointer to the ChatBox so we don't have to use this ugly getChild function everytime.
	CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(m_ConsoleWindow->getChild("History"));

	CEGUI::FormattedListboxTextItem* newItem=0; // This will hold the actual text and will be the listbox segment / item

	//Don't worry, FormattedListboxTextItem is self-deleting... I hope
	newItem = new CEGUI::FormattedListboxTextItem(inMsg, colour); // instance new item with colour
	outputWindow->addItem(newItem); // Add the new FormattedListBoxTextItem to the ListBox
}

void GameConsoleWindow::setVisible(bool visible)
{
	m_ConsoleWindow->setVisible(visible);
	m_bConsole = visible;

	CEGUI::Editbox* Editbox = static_cast<CEGUI::Editbox*>(m_ConsoleWindow->getChild("Editbox"));

	if(visible)
	{
		Editbox->activate();
	}
	else
	{
		Editbox->deactivate();
	}
}

bool GameConsoleWindow::isVisible()
{
	return m_ConsoleWindow->isVisible();
}

bool GameConsoleWindow::EventCallback_TextSent(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);

	auto* editbox = m_ConsoleWindow->getChild("Editbox");

	//For some fucking reason CEGUI strings dont perform deep copies, so we do it manually.
	auto& text = editbox->getText();

	CEGUI::String message(text.size(), 0);

	for(unsigned int i = 0; i < text.size(); ++i)
	{
		message[i] = text[i];
	}

	if(message.size() > 0)
	{
		messages.push_back(std::move(message));
	}

	editbox->setText("");

	return true;
}