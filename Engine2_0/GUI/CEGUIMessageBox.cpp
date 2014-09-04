#include "CEGUIMessageBox.h"

void CEGUIMessageBox::CreateMessageBox(CEGUI::String message)
{
	//Get external root window
	CEGUI::Window* rootWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	//If rootWindow is invalid we don't want to create a message box.
	if(rootWindow == nullptr)
	{
		return;
	}

	////Set up the transparent background/overlay window
	//CEGUI::Window* backgroundWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "MessageBoxBackground");
	//backgroundWindow->setSize(CEGUI::USize(CEGUI::UDim(1.1f, 0.0f), CEGUI::UDim(1.1f, 0.0f)));
	//backgroundWindow->setAlpha(0.5f);
	//backgroundWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
	//backgroundWindow->setVerticalAlignment(CEGUI::VA_CENTRE);
	//backgroundWindow->setProperty("ClientAreaColour", "tl:FFAAAACC tr:FFAAAACC bl:FFAAAACC br:FFAAAACC");
	//backgroundWindow->setProperty("FrameEnabled", "False");
	//backgroundWindow->setProperty("TitlebarEnabled", "False");
	//backgroundWindow->setProperty("CloseButtonEnabled", "False");
	//backgroundWindow->setProperty("DragMovingEnabled", "False");
	//backgroundWindow->setProperty("SizingEnabled", "False");

	//Set up the actual box window
	CEGUI::Window* boxWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "MessageBoxWindow");
	boxWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
	boxWindow->setVerticalAlignment(CEGUI::VA_CENTRE);
	boxWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.0f, 0.0f)));
	boxWindow->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 400.0f), CEGUI::UDim(0.0f, 300.0f)));
	boxWindow->setProperty("CloseButtonEnabled", "False");
	boxWindow->setProperty("DragMovingEnabled", "False");
	boxWindow->setProperty("RollUpEnabled", "False");
	boxWindow->setInheritsAlpha(false);
	boxWindow->setText("Error");

	//Set up the text we'll be rendering in the box
	CEGUI::Window* errorText = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "MessageBoxText");
	errorText->setHorizontalAlignment(CEGUI::HA_CENTRE);
	errorText->setVerticalAlignment(CEGUI::VA_TOP);
	errorText->setSize(CEGUI::USize(CEGUI::UDim(0.97f, 0.0f), CEGUI::UDim(0.8f, 0.0f)));
	errorText->setProperty("VertFormatting", "TopAligned");
	errorText->setProperty("HorzFormatting", "WordWrapLeftAligned");
	errorText->setText(message);

	//Create a confirmation button and put it horizontally centered and furthest down vertically.
	CEGUI::Window* confirmButton = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "MessageConfirmButton");
	confirmButton->setHorizontalAlignment(CEGUI::HA_CENTRE);
	confirmButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.82f, 0.0f)));
	confirmButton->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 90.0f), CEGUI::UDim(0.0f, 40.0f)));
	confirmButton->setText("OK");

	//Setup a lambda function that ensures that we destroy everything when we trigger the EventClicked event.
	auto& selfDestructFunction = 
		[](const CEGUI::EventArgs& args) -> bool
	{
		CEGUI::Window* rootWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
		CEGUI::Window* backgroundWindow = rootWindow->getChild("MessageBoxWindow");

		//Destroy this window and remove it
		rootWindow->destroyChild(backgroundWindow);

		return true;
	};

	//Add the lambda to be triggered when this event is thrown
	confirmButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber::SubscriberSlot(selfDestructFunction));

	//Append children to the box
	boxWindow->addChild(errorText);
	boxWindow->addChild(confirmButton);

	////Append box to the background
	//backgroundWindow->addChild(boxWindow);

	////Make sure it's active and shit
	//backgroundWindow->activate();
	//backgroundWindow->show();
	//backgroundWindow->setAlwaysOnTop(true);
	
	////Append background to real root
	//rootWindow->addChild(backgroundWindow);

	boxWindow->activate();
	boxWindow->show();
	boxWindow->setAlwaysOnTop(true);

	rootWindow->addChild(boxWindow);
}
