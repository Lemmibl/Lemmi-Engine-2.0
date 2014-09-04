#pragma once
#include "CEGUI/CEGUI.h"

struct TextMessage
{
	TextMessage(CEGUI::String ext_text, CEGUI::Colour ext_textColor)
	: text(ext_text), textColor(ext_textColor)
	{	
	}

	CEGUI::String text;
	CEGUI::Colour textColor;
};