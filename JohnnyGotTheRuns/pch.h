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
*  IMPORTANT: Order is important.
*/

#define OLC_PGEX_SPLASHSCREEN		// Manages the GPL-3.0 Licence requirements 
#include "olcPGEX_SplashScreen.h"

#define OLC_PGEX_TRANSFORMEDVIEW	// Manages the Trnasform View
#include "olcPGEX_TransformedView.h"

#include "olcUTIL_Camera2D.h"		// Manages player camera

#include "TMXParser.h"				// TMXParser Stuff for TiledMap graphics
#include "TSXParser.h"				// TSXParser stuff for collisions

#define OLC_PGEX_LEVEL_MANAGER		// Manages the loading of the levels graphices etc
#include "olcPGEX_LevelManager.h"

#define OLC_PGEX_QUICKGUI			// Manages the Menu iteams buttons and sliders
#include "olcPGEX_QuickGUI.h"

#define OLC_PGEX_TTF				// Manages fonts
#include "olcPGEX_TTF.h"

#define OLC_PGEX_MESSAGE_CONTROLLER	// Manages the messages displayed to the screen
#include "olcPGEX_MessageController.h"

#define OLC_PGEX_MAINMENU			// Manages the Main Menu
#include "olcPGEX_MainMenu.h"

#define OLC_PGEX_BACKGROUND_OBJECT	// Manages the Background Image
#include "olcPGEX_Background.h"

#define OLC_PGEX_PLAYER_OBJECT		// Manages the Player object
#include "olcPGEX_PlayerObject.h"

#define OLC_GAME_OBJECT				// Central Location for managing the Game Objects
#include "GameObjects.h"			

#define OLC_PGEX_COLLISION			// Manages the Collisions of objects
#include "olcPGEX_Collision.h"

#define OLC_PGEX_OBJECT_ACTIONS		// Manages the actions of the game objects (movemenmt etc)
#include "olcPGEX_Object_Actions.h"