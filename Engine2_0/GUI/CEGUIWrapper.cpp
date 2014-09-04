#include "CEGUIWrapper.h"

#include "CEGUI/CEGUI.h"
#include "CEGUI/CommonDialogs/ColourPicker/ColourPicker.h"
#include "CEGUI/CommonDialogs/ColourPicker/Controls.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"

CEGUIWrapper::CEGUIWrapper()
{
}


CEGUIWrapper::~CEGUIWrapper()
{
	Shutdown();
}

bool CEGUIWrapper::Initialize()
{
	const CEGUI::String filepath = "../Engine2_0/Data/CEGUI/";

	//// http://static.cegui.org.uk/docs/current/rendering_tutorial.html

	// Bootstrap CEGUI::System with an OpenGL3Renderer object that uses the
	// current GL viewport, the DefaultResourceProvider, and the default
	// ImageCodec.
	//
	// NB: Your OpenGL context must already be initialised when you call this; 
	// CEGUI will not create the OpenGL context itself.
	CEGUI::OpenGL3Renderer& myRenderer = CEGUI::OpenGL3Renderer::create();
	CEGUI::System::create(myRenderer);

	// http://static.cegui.org.uk/docs/current/resprov_tutorial.html

	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes",			filepath+"schemes/");
	rp->setResourceGroupDirectory("imagesets",			filepath+"imagesets/");
	rp->setResourceGroupDirectory("fonts",				filepath+"fonts/");
	rp->setResourceGroupDirectory("layouts",			filepath+"layouts/");
	rp->setResourceGroupDirectory("looknfeels",			filepath+"looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts",		filepath+"lua_scripts/");

	// set the default resource groups to be used
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	// http://static.cegui.org.uk/docs/current/datafile_tutorial.html

	// create (load) the scheme files
	// (this auto-loads the looknfeel and imageset files)
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");

	//Set up some semi-supported windowtypes
	CEGUI::WindowFactoryManager::addWindowType<CEGUI::ColourPicker>();
	CEGUI::WindowFactoryManager::addWindowType<CEGUI::ColourPickerControls>();


	// create (load) a font.
	// The first font loaded automatically becomes the default font, but note
	// that the scheme might have already loaded a font, so there may already
	// be a default set - if we want the "DejaVuSans-10" font to definitely
	// be the default, we should set the default explicitly afterwards.
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10-NoScale.font");

	//The FontManager automatically sets the first loaded font as the system default. If this is not the default font you require, 
	// or if you can not guarantee the order fonts are loaded, you should set the default explicitly, as in this code:
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10-NoScale");

	//This uses the TaharezLook imageset which was loaded as part of the TaharezLook scheme we loaded above.
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	//Finally, if you intend to use tool tips, you should specify the type of the ToolTip based widget that you want used for that purpose. 
	// It is actually possible to set this on a per-window basis, though this is not normally required, and is beyond the scope of this introductory tutorial. 
	// The code to set the default tool tip window type for the initial, default GUIContext looks like this:
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");

	return true;
}

void CEGUIWrapper::Update(double deltaTime)
{
	//Get time since last update
	CEGUI::System::getSingleton().injectTimePulse(static_cast<float>(deltaTime));
}

void CEGUIWrapper::Render()
{
	// make sure that before calling renderAllGUIContexts, that any bound textures
	// and shaders used to render the scene above are disabled using
	// glBindTexture(0) and glUseProgram(0) respectively also set
	// glActiveTexture(GL_TEXTURE_0) 
	// draw GUI
	// NB: When not using >=3.2 core profile, this call should not occur
	// between glBegin/glEnd calls.
	CEGUI::System::getSingleton().renderAllGUIContexts();
}

void CEGUIWrapper::Shutdown()
{
	CEGUI::System::destroy();
}

//bool CEGUIWrapper::HandleInputs()
//{
//unsigned int mouseClickCount, keyPressCount;
//bool result = false;

////Get singleton
//CEGUI::System::getSingleton().injectTimePulse(timer.GetFrameTimeSeconds());

//CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(input->GetMouseDelta().x, input->GetMouseDelta().y);

//keyPressCount = input->ActiveKeyboardStateCount();
//mouseClickCount = input->ActiveMouseStateCount();

//if(keyPressCount > 0 && keypressUpdateTimer >= KeyboardInputUpdateRate)
//{
//	keypressUpdateTimer = 0.0f;

//	auto keyArray = input->GetActiveKeyboardStates();

//	for(int i = 0; i < keyPressCount; i++)
//	{
//		result = false;

//		switch(keyArray[i].first)
//		{
//		case InputClass::KeyDown:
//			result = CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(keyArray[i].second));
//			break;

//		case InputClass::KeyUp:
//			result = CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(keyArray[i].second));
//			break;

//		};
//	}
//}

//if(mouseClickCount > 0 && clickUpdateTimer >= MouseInputUpdateRate)
//{
//	clickUpdateTimer = 0.0f;

//	auto mouseArray = input->GetActiveMouseStates();

//	for(unsigned int i = 0; i < mouseClickCount; i++)
//	{
//		switch(mouseArray[i].first)
//		{
//		case InputClass::KeyDown:
//			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(mouseArray[i].second);

//		case InputClass::KeyUp:
//			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(mouseArray[i].second);
//		}

//		if(mouseArray[i].second == CEGUI::RightButton)
//		{
//			////If right click was pressed then toggle the drawing of the mouse cursor.
//			//if(mouseArray[i].first == InputClass::KeyUp)
//			//{
//			showCursor = !showCursor;

//			//Retarded hack to toggle hiding and showing of mouse cursor.
//			//Apparently the showCursor uses some sort of internal reference  counter, where each false decrements it with 1 and every true increments it with one.
//			//If the program started with cursor showing, it'll be set to +1. You need to be -1 or lower for it to hide........ So Every toggle needs to change it with 2.
//			//http://us.generation-nt.com/answer/showcursor-false-should-hide-cursor-help-10591742.html
//			if(showCursor)
//			{
//				//ShowCursor(TRUE);
//				//ShowCursor(TRUE);
//				input->UnlockMouseCursor();
//			}
//			else
//			{
//				//ShowCursor(FALSE);
//				//ShowCursor(FALSE);
//				input->LockMouseCursor();
//			}
//		}
//		//}
//	}
//}
//
//		return true;
//}
