#pragma once
#include "CEGUI/CEGUI.h"

class GameConsoleWindow
{
private:
	#define SystemColour CEGUI::Colour(0xFF00FF00) //Colour used by the system itself. Used for stuff like: /help
	#define ErrorColour CEGUI::Colour(0xFFFF0000) //Generally an angry red to denote that something is WRONG. Used for stuff like: "<" + inString + "> is an invalid command.";

public:
	GameConsoleWindow();                   // Constructor
	~GameConsoleWindow();				   // Destructor

	void Reset();						   // Clear old messages from the log and reset all settings.

	void setVisible(bool visible);         // Hide or show the console
	bool isVisible();                      // return true if console is visible, false if is hidden

	void CreateCEGUIWindow(CEGUI::String filePath, CEGUI::Window* rootWindow = nullptr);	// The function which will load in the CEGUI Window and register event handlers
	void InitializeFromExternalWindow(CEGUI::Window* rootWindow); //This function is an alternative initialize function for when the layout is loaded outside of this class.

	//Print external text. Alternatively with colour. Default is system colours.
	void PrintText(CEGUI::String inMsg, CEGUI::Colour& textColour = SystemColour);

	bool EventCallback_TextSent(const CEGUI::EventArgs &e); //Event callback function triggered when we send text through the EditBox

	std::vector<CEGUI::String>& GetNewMessages() { return messages; }

private:
	void RegisterHandlers();                                   // Register our handler functions
	
	void ParseText(CEGUI::String inMsg);	// Parse the text the user submitted.
	void OutputText(CEGUI::String inMsg, CEGUI::Colour& colour = SystemColour); // Post the message to the ChatHistory listbox with white as default text colo(u)r

	std::vector<CEGUI::String> messages;
	CEGUI::Window* m_ConsoleWindow;								// This will be a pointer to the ConsoleRoot window.
	CEGUI::String sNamePrefix;                                  // This will be the prefix name we give the layout
	static int iInstanceNumber;                                 // This will be the instance number for this class.
	bool m_bConsole;
};