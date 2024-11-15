#include "pch.h"


/*
* IMPORTANT : This game is based on scaling, the base scale for this game is HD 1280x720 1X1 pixels
* The engine will auto scale objects to ensure the same user experence across different screen size
* If you change the screen ratio, the engine will still auto scale objects on the base of HD 1280x720 1X1
* Play with the settings to see how it works... this will help you alot when developing games
*/


/*
*  Main Game entry point
*/
class JGotTheRuns : public olc::PixelGameEngine
{
public:

	// IMPORTANT: Include olcPGEX_SplashScreen to the the GPL-3.0 Licence requirements
	//olc::SplashScreen olcSplashScreen;


	/*
	* Game Menu
	*/
	enum GAME_MENU
	{
		MAIN_MENU = 0,
		GAME_LEVEL,
		CREDITS

	};

	GAME_MENU eGameMenu;

	/*
	* QuickGUI Stuff
	*/
public:
	olc::QuickGUI::Manager guiManager;

	olc::QuickGUI::Slider* guiSlider1 = nullptr;
	olc::QuickGUI::Slider* guiSlider2 = nullptr;

	olc::QuickGUI::Button* guiButton1 = nullptr;
	olc::QuickGUI::Button* guiButton2 = nullptr;

	olc::QuickGUI::Label* guiLabelMasterVolume = nullptr;
	olc::QuickGUI::Label* guiLabelSoundVolume = nullptr;

	/*
	* sigonasr2 stuff
	*/
	olc::Renderable renLevel;
	Map map;

	/*
	* We will need some Smart Pointers to get use up and running and to manage memory
	* As we will be using olcPGEX, our memony will be managed and cleaned up automatically see olcPGEX_Template.h for details
	*/
	std::unique_ptr<olc::MainMenu> pMainMenu;					// Main Menu Smart pointer
	std::unique_ptr<olc::BackgroundObject> pBackGround;			// Background smart pointer
	std::unique_ptr<olc::MessageController> pMessageController;	// Message smart pointer
	std::unique_ptr<olc::LevelManager> pLevelManager;			// Level Manager Smart pointer
	std::unique_ptr<olc::Collision> pCollision;					// Collision Smart pointer
	std::unique_ptr<olc::GameObjects> pGameObjects;				// Game Object smart pointer
	std::unique_ptr<olc::ObjectActions> pObjectActions;			// Game Object actions manager

	std::shared_ptr<olc::PlayerObject> pPlayer;					// Player smart pointer
	

	// Font 
	olc::Font font;

	
	olc::TileTransformedView tileTransformedView;						// Transformed view object to make world offsetting simple

	// Conveninet constants to define tile map world
	olc::vi2d m_vWorldSize = { 140, 24 };						// 140X24 Tiles
	olc::vi2d m_vTileSize = { 35, 35 };							// Tile Size (Screen Size 35X35, World Size 1 X 1)

	std::vector<uint8_t> vWorldMapGraphics;	// Our basic grid map!

	// The camera!
	olc::utils::Camera2D camera;

	// The point that represents the player, it is "tracked" by the camera
	olc::vf2d vTrackedPoint;

	// Temp Decal
	olc::Renderable renTemp;

	JGotTheRuns()
	{
		// Name your application
		sAppName = "Johnny Got The Runs... OLC CodeJam 2024";

		// Instantiate out smart pointer
		pMainMenu = std::make_unique<olc::MainMenu>("assets/images/interfacePacksheet.png");					// Main Menu
		pBackGround = std::make_unique<olc::BackgroundObject>("assets/images/holytoilet.png", false);			// Background
		pMessageController = std::make_unique<olc::MessageController>("assets/images/LettersSpriteSheet.png");	// Message Controller
		pLevelManager = std::make_unique<olc::LevelManager>("assets/images/levelSpriteSheet.png", 
															"assets/tiledprojects/Level1Output.tmx", 1);					// Game Level Manager
		pCollision = std::make_unique<olc::Collision>();														// Collision Controller
		pGameObjects = std::make_unique<olc::GameObjects>();													// Game Objects Manager
		pObjectActions = std::make_unique<olc::ObjectActions>();												// Game Objects Actions Manager

		/* -- Order is important -- */

		/*
		* Load our Game Objects
		*/
		pGameObjects->LoadLevelObjects(1);


		/*
		* TODO: Remove just here temp
		*/
		pPlayer = pGameObjects->GetGameObject("Johnny");

		/*
		*  Setup collision
		*/
		pCollision->Properties.vecPlayerObjects = &pGameObjects->Properties.vecPlayerObjects;
		pObjectActions->Properties.vecPlayerObjects = &pGameObjects->Properties.vecPlayerObjects;



		/*
		* Setup our level manager
		*/
		pLevelManager->Properties.tv = &tileTransformedView;
		pLevelManager->Properties.viWorldSize = m_vWorldSize;
		pLevelManager->Properties.viTileSize = m_vTileSize;

		/*
		* Setup our collision  // TODO: Sort out pointers to properites 
		*/
		pCollision->Properties.ptrTileTransFormedView = &tileTransformedView;
		pCollision->Properties.ptrmapLayerInfo = &pLevelManager->Properties.mapLayerInfo;
		pCollision->Properties.viSpriteSheetTiles = { 28, 14 };
		pCollision->Properties.ptrvTrackedPoint = &vTrackedPoint;
		pCollision->Properties.viWorldSize = m_vWorldSize;
		pCollision->Properties.viTileSize = m_vTileSize;

		/*
		* Setup our Object Actions
		*/
		pObjectActions->Properties.ptrTileTransFormedView = &tileTransformedView;
		pObjectActions->Properties.ptrmapLayerInfo = &pLevelManager->Properties.mapLayerInfo;
		pObjectActions->Properties.viSpriteSheetTiles = { 28, 14 };
		pObjectActions->Properties.ptrvTrackedPoint = &vTrackedPoint;
		pObjectActions->Properties.ptrCamera = &camera;
		pObjectActions->Properties.viWorldSize = m_vWorldSize;
		pObjectActions->Properties.viTileSize = m_vTileSize;


	}

	~JGotTheRuns()
	{

	}

	/* Sprites */
	olc::Sprite* sprTemp = nullptr;


public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		olc::Font::init();
		font = olc::Font{ "assets/fonts/kenney_bold.ttf", 16 };
		font.AddFallbackFont("assets/fonts/kenney_thick.ttf");

		
		
		/*
		* Quick GUI Stuff
		*/
		guiSlider1 = new olc::QuickGUI::Slider(guiManager,
			{ 30.0f, 10.0f }, { 246.0f, 10.0f }, 0, 100, 50);


		// Diagonal Slider!
		guiSlider2 = new olc::QuickGUI::Slider(guiManager,
			{ 20.0f, 20.0f }, { 120.0f, 120.0f }, 0, 100, 50);


		// Labels for theme colour sliders
		guiLabelMasterVolume = new olc::QuickGUI::Label(guiManager,
			"Volume:", { 80.0f, 22.0f }, { 50.0f, 16.0f });
		guiLabelSoundVolume = new olc::QuickGUI::Label(guiManager,
			"SFX:", { 80.0f, 42.0f }, { 50.0f, 16.0f });

		// Customize how the labels look
		guiLabelSoundVolume->nAlign = olc::QuickGUI::Label::Alignment::Left;
		guiLabelSoundVolume->bHasBorder = false;
		guiLabelSoundVolume->bHasBackground = false;

		guiLabelMasterVolume->nAlign = olc::QuickGUI::Label::Alignment::Left;
		guiLabelMasterVolume->bHasBorder = false;
		guiLabelMasterVolume->bHasBackground = false;

		// Some Buttons,
		guiButton1 = new olc::QuickGUI::Button(guiManager,
			"Start", { 30.0f, 150.0f }, { 100.0f, 16.0f });
		guiButton2 = new olc::QuickGUI::Button(guiManager,
			"Credits", { 30.0f, 170.0f }, { 100.0f, 16.0f });


		// Transfrom View settings... Move to new location
		// Construct transform view
		tileTransformedView = olc::TileTransformedView(GetScreenSize(), m_vTileSize);

		// Construct Camera
		vTrackedPoint = { 2.0f, 20.0f };
		camera = olc::utils::Camera2D(GetScreenSize() / m_vTileSize, vTrackedPoint);

		// Configure Camera
		camera.SetTarget(vTrackedPoint);
		camera.SetMode(olc::utils::Camera2D::Mode::Simple);
		camera.SetWorldBoundary({ 0.0f, 0.0f }, m_vWorldSize);
		camera.EnableWorldBoundary(true);

		// Create our world grid
		vWorldMapGraphics.resize(m_vWorldSize.x * m_vWorldSize.y);
		
		renTemp.Load("assets/images/toilet.png");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		bool bResult = false;

		// TODO: Remove, just for debug
		//eGameMenu = GAME_MENU::GAME_LEVEL;

		switch (eGameMenu)
		{
		case JGotTheRuns::MAIN_MENU:
		{
			pCollision->Properties.bIsEnabled = false;
			pObjectActions->Properties.bIsEnabled = false;
			bResult = DisplayMainMenu(fElapsedTime);
			//pPlayer->UpdateAction(olc::PlayerObject::ACTION::BEHIND_BACK);
			//pPlayer->Update(fElapsedTime);
			break;
		}
		case JGotTheRuns::GAME_LEVEL:
		{
			pCollision->Properties.bIsEnabled = true;
			pObjectActions->Properties.bIsEnabled = true;
			bResult = DisplayGameLevel(fElapsedTime);
			bResult = DisplayGameObjects(fElapsedTime);

			break;
		}	
		case JGotTheRuns::CREDITS:
		{
			pCollision->Properties.bIsEnabled = false;
			pObjectActions->Properties.bIsEnabled = false;
			bResult = DisplayCredits(fElapsedTime);
			break;
		}	
		default:
			break;
		}

		

		return bResult;
	}

	/*
	* Loads and Displays the Main Menu
	*/
	bool DisplayMainMenu(float fElapsedTime)
	{
		bool bResult = false;
		SetDrawTarget(nullptr);
		Clear(olc::BLACK);
		pBackGround->DrawDecal();
		pMainMenu->DrawDecal();

		bResult = DisplayQuickGUI(fElapsedTime);
		
		return bResult;
	}

	/*
	* Displays the Main Menu GUI Options and handles events
	*/
	bool DisplayQuickGUI(float fElapsedTime)
	{
		guiManager.Update(this);

		guiLabelMasterVolume->vPos.x = pMainMenu->Properties.vfSettingRect.x;
		guiLabelMasterVolume->vPos.y = pMainMenu->Properties.vfSettingRect.y + 10;
		guiLabelMasterVolume->vSize = { 240.0f, 20.0f };
		guiSlider1->vPosMin.x = pMainMenu->Properties.vfSettingRect.x;
		guiSlider1->vPosMin.y = pMainMenu->Properties.vfSettingRect.y + 40.0f;
		guiSlider1->vPosMax.x = pMainMenu->Properties.vfSettingRect.x + 240.f;
		guiSlider1->vPosMax.y = pMainMenu->Properties.vfSettingRect.y + 40.0f;

		guiLabelSoundVolume->vPos.x = pMainMenu->Properties.vfSettingRect.x;
		guiLabelSoundVolume->vPos.y = pMainMenu->Properties.vfSettingRect.y + 50;
		guiLabelSoundVolume->vSize = { 240.0f, 20.0f };
		guiSlider2->vPosMin.x = pMainMenu->Properties.vfSettingRect.x;
		guiSlider2->vPosMin.y = pMainMenu->Properties.vfSettingRect.y + 80.0f;
		guiSlider2->vPosMax.x = pMainMenu->Properties.vfSettingRect.x + 240.0f;
		guiSlider2->vPosMax.y = pMainMenu->Properties.vfSettingRect.y + 80.0f;

		guiButton1->vPos.x = pMainMenu->Properties.vfButtonRect.x;
		guiButton1->vPos.y = pMainMenu->Properties.vfButtonRect.y + 10.0f;
		guiButton1->vSize = { 240.0f, 40.0f };

		guiButton2->vPos.x = pMainMenu->Properties.vfButtonRect.x;
		guiButton2->vPos.y = pMainMenu->Properties.vfButtonRect.y + 60.0f;
		guiButton2->vSize = { 240.0f, 40.0f };


		if (guiSlider1->bHeld)
		{
			guiLabelMasterVolume->sText = "Volume: " + std::to_string(guiSlider1->fValue);

		}

		if (guiSlider2->bHeld)
		{
			guiLabelSoundVolume->sText = "SFX: " + std::to_string(guiSlider2->fValue);

		}

		if (guiButton1->bPressed)
		{
			pBackGround->LoadBackGround("assets/images/background.png");
			eGameMenu = GAME_MENU::GAME_LEVEL;
			pPlayer->Properties.vfStartPosition.x = (GetScreenSize().x / 100.0f) * 10.0f;
			pPlayer->Properties.vfStartPosition.y = (GetScreenSize().y / 100.0f) * 55.0f;
		}

		if (guiButton2->bPressed)
		{
			eGameMenu = GAME_MENU::CREDITS;
		}

		guiManager.DrawDecal(this);

		return true;
	}


	/*
	* Displays the current level that is loaded
	*/
	bool DisplayGameLevel(float fElapsedTime)
	{
		// TODO: Add code to manage mulitple levels... for the jam one will do!
		pBackGround->DrawDecal();
		olc::vi2d vTileTL = tileTransformedView.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = tileTransformedView.GetBottomRightTile().min(m_vWorldSize);

		// Display the Level
		pLevelManager->DisplayLevel(fElapsedTime, vTileTL, vTileBR);

		return true;
	}

	bool DisplayGameObjects(float fElapsedTime)
	{
		for (auto& pPlayerObject : pGameObjects->Properties.vecPlayerObjects)
		{
			pPlayerObject->Update(fElapsedTime);
		}

		return true;
	}

	/*
	* Displays all the folks that help with this project
	*/
	bool DisplayCredits(float fElapsedTime)
	{
		// TODO: Get folks to help with this project ;)
		return true;
	}



};

int main()
{

	JGotTheRuns demo;

	/*
	* IMPORTANT : This game is based on scaling, the base scale for this game is HD 1280x720 1X1 pixels
	* The engine will auto scale objects to ensure the same user experence across different screen size
	* If you change the screen ratio, the engine will still auto scale objects on the base of HD 1280x720 1X1
	* Play with the settings to see how it works... this will help you alot when developing games
	*
	* In short there should be no hard coded values for sprites, except in the case of where the sprite is located on spritesheet
	*/

	// Lets use HD!
	if (demo.Construct(1280, 840, 1, 1))
		demo.Start();
	return 0;
}
