#pragma once
/*
* Setup our engine and tell it to use Geomertry2D Vec2D over PGE 2.0 Vec2D
*/
#define OLC_PGE_APPLICATION
#include "olcUTIL_Geometry2D.h"
#define OLC_IGNORE_VEC2D
#include "olcPixelGameEngine.h"

/*
*  Setup our PGE Extensions, I use PGEX classes to manage the game objects, it makes life easier.... 
*/

#define OLC_PGEX_SPLASHSCREEN		// Manages the GPL-3.0 Licence requirements 
#include "olcPGEX_SplashScreen.h"

#define OLC_PGEX_MAINMENU			// Manages the Main Menu
#include "olcPGEX_MainMenu.h"

#define OLC_PGEX_BACKGROUND_OBJECT	// Manages the Background Image
#include "olcPGEX_Background.h"

#define OLC_PGEX_MESSAGE_CONTROLLER	// Manages the messages displayed to the screen
#include "olcPGEX_MessageController.h"