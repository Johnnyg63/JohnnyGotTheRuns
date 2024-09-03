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
	* Manages the Main Menu Screen
	*/
	class MainMenu : public PGEX
	{

	public:
		// Standard Constructor
		MainMenu();

		/*
		* Constructor
		* strSpriteSheetPath: Sprite Sheet Path i.e. "assets/images/SprintSheet.png"
		*/
		MainMenu(std::string strSpriteSheetPath);
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

		// Font 
		olc::Font font;

	private:

		void DrawBanners();

		void DrawOptionsMenu();

		void DrawToilet();

		


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

			ImageInfo sprImageInfo;					// Stores the Source and Size of the sprImage

			std::string strSpriteSheetPath = "";    // SpriteSheetPath path, i.e. "images/mysprite.png", Default: ""
			std::string strFontPath = "";			// Font path, i.e. "fonts/kenney_bold.ttf", Default: ""
			std::string strFontBackupPath = "";		// Font path, i.e. "fonts/kenney_thick.ttf", Default: ""
			int8_t nFontSize = 12;					// Font size, Default: 16

			olc::Renderable renSpriteSheet;			// Keeps the sprSpriteSheet and decSpriteSheet in the one location
			/*
			*
			* Stores the location of the partial image from the sprSpriteSheet
			* ImageInfo.vSource {x,y} of the top left of the partial image to draw
			* ImageInfo.vSize {w, h} size of the partial image to be drawn
			*
			*/
			std::vector<ImageInfo> vecPartialImages;

			olc::vf2d vfSettingRect = { 0.0f, 0.0f };
			olc::vf2d vfButtonRect = { 0.0f, 0.0f };




		};

		ObjectProperites Properties;

	private:
		olc::Renderable renBGImage;

		olc::Renderable renToilet;

		std::string strToiletPath;

		// std::u32string
		olc::Decal* decHeaderMessage = nullptr;
		olc::Decal* decFooterMessage = nullptr;
		olc::Decal* decSettingMessage = nullptr;
		olc::Decal* decButtonsMessage = nullptr;


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

		// TODO: Create default empty sprite for when this constructor is useds
	}

	MainMenu::MainMenu(std::string sprSpriteSheetPath) : PGEX(true)
	{
		// Set up default Properties
		Properties.bIsActive = 0;

		Properties.strSpriteSheetPath = sprSpriteSheetPath;

#if defined (_MSC_VER)
		// Windows stuff

		if (sprSpriteSheetPath.rfind("./", 0) != 0) {
			Properties.strSpriteSheetPath = "./" + sprSpriteSheetPath;
		}
		strToiletPath = "./assets/images/toilet.png";
#else
		/*if (sprSpriteSheetPath.rfind("./", 0) == 0) {
			Properties.strSpriteSheetPath = sprSpriteSheetPath.substr(2);
		}
		strToiletPath = "assets/images/toilet.png";*/
#endif

	}

	// See Step 3: Rename to your Class name
	MainMenu::~MainMenu()
	{

		if (Properties.renSpriteSheet.Decal() != nullptr)
		{
			Properties.renSpriteSheet.Decal()->~Decal();

		}

		if (Properties.renSpriteSheet.Sprite() != nullptr)
		{
			Properties.renSpriteSheet.Sprite()->~Sprite();

		}

		Properties.vecPartialImages.clear();

	

	}

	// See Step 3: Rename to your Class name
	void MainMenu::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

		// Gets load our renender
		Properties.renSpriteSheet.Load(Properties.strSpriteSheetPath);
		Properties.sprImageInfo.vSize.x = Properties.renSpriteSheet.Sprite()->width;
		Properties.sprImageInfo.vSize.y = Properties.renSpriteSheet.Sprite()->height;

		renToilet.Load(strToiletPath);

		// Load our font
#if defined (_MSC_VER)
// Windows stuff

		Properties.strFontPath = "./assets/fonts/kenney_bold.ttf";
		Properties.strFontBackupPath = "./assets/fonts/kenney_thick.ttf";
#else
		Properties.strFontPath = "assets/fonts/kenney_bold.ttf";
		Properties.strFontBackupPath = "assets/fonts/kenney_thick.ttf";
#endif

		olc::Font::init();
		font = olc::Font{ Properties.strFontPath, Properties.nFontSize };
		font.AddFallbackFont(Properties.strFontBackupPath);

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
		//if (!Properties.bIsActive) return false;

		// Fires just before the main OnUserUpdate
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void MainMenu::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
		// If we are not acivated then we just return, we have nothing to do
		//if (!Properties.bIsActive) return;
	}

	// See Step 3: Rename to your Class name
	void MainMenu::DrawDecal()
	{
		DrawBanners();
		DrawOptionsMenu();
		DrawToilet();
	}

	void MainMenu::DrawBanners()
	{
		// 1: Our banner is store here in the sprite sheet (interfacePack_sheet.xml)
	   // <SubTexture name="bannerModern.png" x="0" y="59" width="264" height="50"/> TODO: Create XML Reader for images

	   // 2: We now need to scale it to our screen, we know our base sceen size is 1280/720, 1, 1, so will work to this

	   // Get the current vScale of our screen
		olc::vf2d vfBaseScale = { 1280, 720 };
		olc::vf2d vfScaler = { 1.0f, 1.0f };
		olc::vf2d vfScreenSize = pge->GetScreenSize();
		vfScaler.x = vfScreenSize.x / vfBaseScale.x;
		vfScaler.y = vfScreenSize.y / vfBaseScale.y;

		//3: Get the current center pos and 20% from top
		olc::vf2d vfCenTopPos;
		vfCenTopPos.x = vfScreenSize.x / 2.0f;
		vfCenTopPos.y = (vfScreenSize.y / 100) * 10;

		//4: Ok now we need our start pos for our banner
		olc::vf2d vfStartPos;
		vfStartPos.x = vfCenTopPos.x - ((264 * vfScaler.x) / 2);  // we know our base height is width="264" from the xml
		vfStartPos.y = vfCenTopPos.y;

		//5: done
		pge->DrawPartialDecal(
			vfStartPos,
			Properties.renSpriteSheet.Decal(),
			{ 0.0f, 59.0f },
			{ 264.0f, 50.0f },
			vfScaler);

		vfStartPos.x += 2.0f;
		vfStartPos.y += 12.0f;

		// Memory Leak.... will correct later
		if (decHeaderMessage == nullptr)
		{
			decHeaderMessage = font.RenderStringToDecal(U" Johnny Got The Runs...", olc::BLACK);
		}
		pge->DrawDecal({ vfStartPos.x + ((vfStartPos.x / 100.0f) * 2.5f),  vfStartPos.y + 6.0f }, decHeaderMessage, vfScaler);

		vfStartPos.x = (pge->GetScreenSize().x / 100.0f) * 12.5f;
		vfStartPos.y = (pge->GetScreenSize().y / 100.0f) * 90.0f;

		// We need to work out how much we want to scale in the x so the banner fills 80% of the screen
		// We know the width of the image therefore
		float newWidth = (pge->GetScreenSize().x / 100) * 80;	// New Width Size
		float percentDiff = (264.0f / newWidth) * 100;			// Gives us the %differance between the widths	
		float newScaleX = (100 / percentDiff);						// Tells us by what amount to scale x


		pge->DrawPartialDecal(
			vfStartPos,
			Properties.renSpriteSheet.Decal(),
			{ 0.0f, 59.0f },
			{ 264.0f, 50.0f },
			{ newScaleX, vfScaler.y });


		vfStartPos.x = (pge->GetScreenSize().x / 100.0f) * 33.0f;
		vfStartPos.y = (pge->GetScreenSize().y / 100.0f) * 92.5f;

		if (decFooterMessage == nullptr)
		{
			decFooterMessage = font.RenderStringToDecal(U" Can you get Johnny to the Throne in time... ", olc::BLACK);
		}

		pge->DrawDecal(vfStartPos, decFooterMessage, vfScaler);
		
	}

	void MainMenu::DrawOptionsMenu()
	{
		// Right the options will be on the top right of the screen
		// Option 1: Start Game
		// Option 2: Settings (Shound etc)
		// Option 3: Credits

		// Ok first step lets redraw our banner to work
		// <SubTexture name="bannerHanging.png" x="0" y="0" width="268" height="59"/>

		// Get our scaler values
		olc::vf2d vfBaseScale = { 1280, 720 };
		olc::vf2d vfScaler = { 1.0f, 1.0f };
		olc::vf2d vfScreenSize = pge->GetScreenSize();
		vfScaler.x = vfScreenSize.x / vfBaseScale.x;
		vfScaler.y = vfScreenSize.y / vfBaseScale.y;

		//3: Get the current center pos and 20% from top and 75% from the left
		olc::vf2d vfCenTopRight;
		vfCenTopRight.x = (vfScreenSize.x / 100.0f) * 80.0f;
		vfCenTopRight.y = (vfScreenSize.y / 100.0f) * 15.0f;

		//4: Ok now we need our start pos for our banner
		olc::vf2d vfStartPos;
		vfStartPos.x = vfCenTopRight.x - ((264 * vfScaler.x) / 2);  // we know our base height is width="264" from the xml
		vfStartPos.y = vfCenTopRight.y;

		float newHeight = (pge->GetScreenSize().y / 100) * 33;	// New Height Size
		float percentDiff = (59.0f / newHeight) * 100;			// Gives us the %differance between the widths	
		float newScaleY = (100 / percentDiff);					// Tells us by what amount to scale x

		//5: done
		pge->DrawPartialDecal(
			vfStartPos,
			Properties.renSpriteSheet.Decal(),
			{ 0.0f, 59.0f },
			{ 264.0f, 50.0f },
			{ vfScaler.x, newScaleY });

		if (decSettingMessage == nullptr)
		{
			decSettingMessage = font.RenderStringToDecal(U" Settings ", olc::BLACK);
		}

		pge->DrawDecal({ vfStartPos.x + 10, vfStartPos.y + 40 }, decSettingMessage, vfScaler);
		
		// Set our Top Right position for QuickGUI
		Properties.vfSettingRect = { vfStartPos.x + 10, vfStartPos.y + 60 };

		vfCenTopRight.x = (vfScreenSize.x / 100.0f) * 20.0f;
		vfCenTopRight.y = (vfScreenSize.y / 100.0f) * 15.0f;

		vfStartPos.x = vfCenTopRight.x - ((264 * vfScaler.x) / 2);  // we know our base height is width="264" from the xml
		vfStartPos.y = vfCenTopRight.y;

		newHeight = (pge->GetScreenSize().y / 100) * 33;	// New Height Size
		percentDiff = (59.0f / newHeight) * 100;			// Gives us the %differance between the widths	
		newScaleY = (100 / percentDiff);						// Tells us by what amount to scale x

		//5: done
		pge->DrawPartialDecal(
			vfStartPos,
			Properties.renSpriteSheet.Decal(),
			{ 0.0f, 59.0f },
			{ 264.0f, 50.0f },
			{ vfScaler.x, newScaleY });

		if (decButtonsMessage == nullptr)
		{
			decButtonsMessage = font.RenderStringToDecal(U" Lets Go! ", olc::BLACK);
		}

		pge->DrawDecal({ vfStartPos.x + 10, vfStartPos.y + 40 }, decButtonsMessage, vfScaler);

		Properties.vfButtonRect = { vfStartPos.x + 10, vfStartPos.y + 60 };
	}

	void MainMenu::DrawToilet()
	{
		// Get the current vScale of our screen
		olc::vf2d vfBaseScale = { 1280, 720 };
		olc::vf2d vfScaler = { 1.0f, 1.0f };
		olc::vf2d vfScreenSize = pge->GetScreenSize();
		vfScaler.x = vfScreenSize.x / vfBaseScale.x;
		vfScaler.y = vfScreenSize.y / vfBaseScale.y;

		//3: Get the current center pos and 20% from top
		olc::vf2d vfCenTopPos;
		vfCenTopPos.x = (vfScreenSize.x / 100) * 80;
		vfCenTopPos.y = (vfScreenSize.y / 100) * 80;

		//4: Ok now we need our start pos for our banner
		olc::vf2d vfStartPos;
		vfStartPos.x = vfCenTopPos.x;  // we know our base height is width="264" from the xml
		vfStartPos.y = vfCenTopPos.y;

		//5: done
		pge->DrawDecal(
			vfStartPos,
			renToilet.Decal(),
			{ vfScaler.x / 2, vfScaler.y / 2 });

	}


} // olc

#endif
