#include "pch.h"
//#include <memory>


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
	* We will need some Smart Pointers to get use up and running and to manage memory
	* As we will be using olcPGEX, our memony will be managed and cleaned up automatically see olcPGEX_Template.h for details
	*/
	std::unique_ptr<olc::MainMenu> pMainMenu;					// Main Menu Smart pointer
	std::unique_ptr<olc::BackgroundObject> pBackGround;			// Background smart pointer
	std::unique_ptr<olc::MessageController> pMessageController;	// Message smart pointer, 
	std::unique_ptr<olc::PlayerObject> pPlayer;					// Player smart pointer

	// Font 
	olc::Font font;


	JGotTheRuns()
	{
		// Name your application
		sAppName = "Johnny Got The Runs... OLC CodeJam 2024";

		// Instantiate out smart pointer
		pMainMenu	= std::make_unique<olc::MainMenu>("assets/images/interfacePack_sheet.png");					// Main Menu
		pBackGround = std::make_unique<olc::BackgroundObject>("assets/images/holytoilet.png", false);			// Background
		pMessageController = std::make_unique<olc::MessageController>("assets/images/LettersSpriteSheet.png");	// Message Controller
		
		
		
		
		
		/*
		*  Setup our player
		*/
		pPlayer = std::make_unique<olc::PlayerObject>("assets/images/playerSpriteSheet.png", true);
		pPlayer->Properties.strName = "Johnnyg63";    // Set our player name
		pPlayer->Properties.nPlayerNumber = 0;        // Set our player numbner
		// Load player details
		pPlayer->Properties.nMaxFrames = 3;
		pPlayer->Properties.nLives = 3;
		pPlayer->Properties.vfVelocity = { 10.0f, 10.0f };
		pPlayer->Properties.fFramesPerSecound = 20.0f;

		/*
		* <SubTexture name="run0" x="1152" y="512" width="192" height="256"/>
	<SubTexture name="run1" x="1344" y="512" width="192" height="256"/>
	<SubTexture name="run2" x="1536" y="512" width="192" height="256"/>
		*/

		olc::PlayerObject::ImageInfo sInfo;
		sInfo.vSource = { 0.0f, 292.0f };
		sInfo.vSize = { 88.0f, 73.0f };
		pPlayer->Properties.vecPartialImages.push_back(sInfo);

		sInfo.vSource = { 0.0f, 219.0f };
		sInfo.vSize = { 88.0f, 73.0f };
		pPlayer->Properties.vecPartialImages.push_back(sInfo);

		sInfo.vSource = { 0.0f, 146.0f };
		sInfo.vSize = { 88.0f, 73.0f };
		pPlayer->Properties.vecPartialImages.push_back(sInfo);


		// Move the player to half way down screen, and 5% in from the right
		pPlayer->Properties.viStartPosition.x = (ScreenWidth() / 100) * 10;
		pPlayer->Properties.viStartPosition.y = (ScreenHeight() / 2);
		

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

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		Clear(olc::BLACK);
		pBackGround->DrawDecal();
		pMainMenu->DrawDecal();
		
		



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
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();
	return 0;
}
