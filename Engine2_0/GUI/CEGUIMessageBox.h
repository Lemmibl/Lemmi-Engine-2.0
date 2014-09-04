#pragma once
#include "CEGUI/CEGUI.h"

//Self-destructing and self-contained messagebox using the CEGUI system.
namespace CEGUIMessageBox
{
	//This function creates a message box with the string parameter. You don't have to worry about the lifetime or anything!
	void CreateMessageBox(CEGUI::String message);
};

