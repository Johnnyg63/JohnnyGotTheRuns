#pragma once
#include "pch.h"


/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_PGEX_MAINMENU
// Add extra defines for sub header files you might be using :)
#endif

namespace olc
{
	/*
	* See Step 3: Rename to your Class name
	*/
	class MainMenu : public PGEX
	{

	public:
		MainMenu(); 
		virtual ~MainMenu();

		// Fires just before the main OnUserCreate
		virtual void OnBeforeUserCreate() override;

		// Fires just After the main OnUserCreate
		virtual void OnAfterUserCreate() override;

		// Fires just before the main OnUserUpdate
		virtual bool OnBeforeUserUpdate(float& fElapsedTime) override;

		// Fires just After the main OnUserUpdate
		virtual void OnAfterUserUpdate(float fElapsedTime) override;

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created decal
		void DrawDecal();

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created sprite
		void DrawSprite();

		// Add your own public methods here


	public:

		/*
		* Example struct that can be used within the Properties struct for a vector, vecPartialImages, in this case
		*/
		struct ImageInfo
		{
			olc::vf2d vSource = { 0.0f, 0.0f };
			olc::vf2d vSize = { 1.0f, 1.0f };
		};

		/*
		* This is your Class/Object properities example
		* Add and edit as you need, it will allow you to make changes to the object on the fly
		* As this Class is connected to the engine it will be automatically called when needed
		* no complex config required, "we do the engine, you do the fun stuff"
		*/
		struct ObjectProperites
		{
			bool bIsActive = false;				// Sets if the object is active or not, Default: false (Not Active)

			std::string strName = "GameObject"; // Object name. Default "GameObject"
			int8_t nObjectNumber = 0;	        // Object number, Default 0 i.e. Player 1 , Player 2 etc

			uint32_t nLives = 3;				// Lives, Default 3
			uint32_t nMaxFrames = 3;			// Max number of frames, Default 0, this means we are not anitmating this object
			uint32_t nCurrentFrame = 0;			// Stores the number of the last frame displayed
			uint32_t nCurrentFPS = 0;			// Stores the current Engine FPS

			float fFrameChangeRate = 0.0f;		// This value will auto calcuate the rate of change depending on the current FPS
			float fFrameElapsedTime = 0.0f;		// Keeps track of the time pass since the last frame change
			float fFramesPerSecound = 1.0f;		// Set the number of times the frame is to change per second 

			olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

			olc::vi2d viStartPosition = { 0,0 };				// Start Position {x,y} (Int32_t), Default {0,0}
			//olc::vf2d vfStartPosition = { 0.0f, 0.0f };	    // Start Positon POS {x,y} (float), Default {0.0f,0.0f}, recommended for decals

			olc::vi2d viPosition = { 0,0 };	        // Player current POS {x,y} (Int32_t), Default {0,0}
			olc::vf2d vfPosition = { 0.0f,0.0f };	// Player current POS {x,y} (float), Default {0.0f,0.0f}, recommended for decals

			std::string strSpritePath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""
			olc::Sprite* sprImage = nullptr;	// Player Sprite image, Default nullptr
			olc::Decal* decImage = nullptr;	    // Player Decal Image, Defalut nullptr
			ImageInfo sprImageInfo;             // Stores the Source and Size of the sprImage


			std::string strSpriteSheetPath = "";     // SpriteSheetPath path, i.e. "images/mysprite.png", Default: ""
			olc::Sprite* sprSpriteSheet = nullptr;	// Player Sprite Sheet image, Default nullptr
			olc::Decal* decSpriteSheet = nullptr;	// Plaer Decal Sheet Image, Defalut nullptr

			olc::Renderable renImage;      // Keeps the sprImage and decImage in the one location
			olc::Renderable renSpriteSheet;  // Keeps the sprSpriteSheet and decSpriteSheet in the one location
			/*
			*
			* Stores the location of the partial image from the sprSpriteSheet
			* ImageInfo.vSource {x,y} of the top left of the partial image to draw
			* ImageInfo.vSize {w, h} size of the partial image to be drawn
			*
			*/
			std::vector<ImageInfo> vecPartialImages;



		};

		ObjectProperites Properties;


	};

}

/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_PGEX_MAINMENU
#undef OLC_PGEX_MAINMENU

namespace olc
{

	// See Step 3: Rename to your Class name
	MainMenu::MainMenu() : PGEX(true)
	{
		// Set up default Properties
		Properties.bIsActive = 0;
	}

	// See Step 3: Rename to your Class name
	MainMenu::~MainMenu()
	{

		if (Properties.decImage != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.decImage;
		}

		if (Properties.sprImage != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.sprImage;
		}

		if (Properties.decSpriteSheet != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.decSpriteSheet;
		}

		if (Properties.sprSpriteSheet != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.sprSpriteSheet;
		}

		Properties.vecPartialImages.clear();



	}

	// See Step 3: Rename to your Class name
	void MainMenu::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

	}

	// See Step 3: Rename to your Class name
	void MainMenu::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	// See Step 3: Rename to your Class name
	bool MainMenu::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// If we are not acivated then we just return, we have nothing to do
		if (!Properties.bIsActive) return false;
		
		// Fires just before the main OnUserUpdate
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void MainMenu::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
		// If we are not acivated then we just return, we have nothing to do
		if (!Properties.bIsActive) return;
	}

	// See Step 3: Rename to your Class name
	void MainMenu::DrawDecal()
	{

	}

	// See Step 3: Rename to your Class name
	void MainMenu::DrawSprite()
	{

	}



} // olc

#endif
