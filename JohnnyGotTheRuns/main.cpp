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
	* GAME Menu
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
	std::unique_ptr<olc::MessageController> pMessageController;	// Message smart pointer, 
	std::unique_ptr<olc::PlayerObject> pPlayer;					// Player smart pointer

	// Font 
	olc::Font font;

	int8_t tempSwitch = 0;

	JGotTheRuns()
	{
		// Name your application
		sAppName = "Johnny Got The Runs... OLC CodeJam 2024";

		// Instantiate out smart pointer
		pMainMenu = std::make_unique<olc::MainMenu>("assets/images/interfacePack_sheet.png");					// Main Menu
		pBackGround = std::make_unique<olc::BackgroundObject>("assets/images/holytoilet.png", false);			// Background
		pMessageController = std::make_unique<olc::MessageController>("assets/images/LettersSpriteSheet.png");	// Message Controller


		/*
		*  Setup our player
		*/
		pPlayer = std::make_unique<olc::PlayerObject>("assets/images/playerSpriteSheet.png", true);
		pPlayer->Properties.strName = "Johnnyg63";    // Set our player name
		pPlayer->Properties.nPlayerNumber = 0;        // Set our player numbner
		// Load player details
		pPlayer->Properties.nLives = 3;
		pPlayer->Properties.vfVelocity = { 100.0f, 100.0f };


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
		font = olc::Font{ "./assets/fonts/kenney_bold.ttf", 16 };
		font.AddFallbackFont("./assest/fonts/kenney_thick.ttf");

		pPlayer->Properties.vfStartPosition.x = (GetScreenSize().x / 100.0f) * 10.0f;
		pPlayer->Properties.vfStartPosition.y = (GetScreenSize().y / 100.0f) * 72.0f;
		pPlayer->Properties.vfPosition = pPlayer->Properties.vfStartPosition;
		pPlayer->Properties.vfMasterScaler = { 0.50f, 0.50f }; // Out player is HD and Big, bring him down a little

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

		// TODO Remove
		renLevel.Load("./assets/images/platformerPack_industrial_tilesheet.png");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		bool bResult = false;

		switch (eGameMenu)
		{
		case JGotTheRuns::MAIN_MENU:
			bResult = DisplayMainMenu(fElapsedTime);
			break;
		case JGotTheRuns::GAME_LEVEL:
			bResult = DisplayGameLevel(fElapsedTime);
			break;
		case JGotTheRuns::CREDITS:
			bResult = DisplayCredits(fElapsedTime);
			break;
		default:
			break;
		}

		return bResult;
	}


	bool DisplayMainMenu(float fElapsedTime)
	{
		bool bResult = false;
		SetDrawTarget(nullptr);
		Clear(olc::BLACK);
		pBackGround->DrawDecal();
		pMainMenu->DrawDecal();

		bResult = DisplayQuickGUI(fElapsedTime);

		pPlayer->UpdateAction(olc::PlayerObject::ACTION::BEHIND_BACK);
		if (GetKey(olc::Key::RIGHT).bHeld)
		{
			pPlayer->UpdateAction(olc::PlayerObject::ACTION::WALK);
			pPlayer->Properties.vfPosition.x += pPlayer->Properties.vfVelocity.x * fElapsedTime;
		}
		if (GetKey(olc::Key::LEFT).bHeld)
		{
			pPlayer->UpdateAction(olc::PlayerObject::ACTION::WALK);
			pPlayer->Properties.vfPosition.x -= pPlayer->Properties.vfVelocity.x * fElapsedTime;
		}

		pPlayer->UpdatePlayer(fElapsedTime);
		return bResult;
	}

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
			TMXParser tmxParser = TMXParser("./assets/maps/Level1Output.tmx");
			map = tmxParser.GetData();
			
			eGameMenu = GAME_MENU::GAME_LEVEL;
		}

		if (guiButton2->bPressed)
		{
			eGameMenu = GAME_MENU::CREDITS;
		}

		guiManager.DrawDecal(this);

		return true;
	}


	bool DisplayGameLevel(float fElapsedTime)
	{
		size_t nCount = map.LayerData.size();
		nCount = map.MapData.data.size();
		nCount = map.TilesetData.data.size();

		int x = 0;
		int y = 0;
		bool bTest = false;

		for (auto& layer : map.LayerData)
		{
			auto rowYtiles = layer.tiles;  // 11 Rows
			for (auto& tiles : rowYtiles)
			{
				x = 0;
				for (auto& tile : tiles)
				{
					
					int tileId = tile;
					if (tileId > 0.)
					{
						// Draw something
						int tileX = (tileId - 1) % 980;
						int tileY = (tileId - 1) / 980;

						float spriteX = x * 70;
						float spriteY = y * 70;

						float sourceX = tileX * 70;
						float sourceY = tileY * 70;

						DrawRectDecal({ spriteX, spriteY }, { 70.0f, 70.0f });
						DrawPartialDecal({ spriteX, spriteY }, { 70.0f, 70.0f }, renLevel.Decal(), { sourceX, sourceY }, { 70.0f, 70.0f });

					}

					x++;
				}

				y++;
			}
			
		}
		return true;
	}

	bool DisplayCredits(float fElapsedTime)
	{

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
	if (demo.Construct(1260, 770, 1, 1))
		demo.Start();
	return 0;
}
