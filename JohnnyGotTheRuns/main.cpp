#include "pch.h"
//#include <memory>

// Override base class with your custom functionality
class JGotTheRuns : public olc::PixelGameEngine
{
public:

	/*
	* We will need some Smart Pointers to get use up and running and to manage memory
	* As we will be using olcPGEX, our memony will be managed and cleaned up automatically see olcPGEX_Template.h for details
	*/
	std::unique_ptr<olc::MainMenu> pMainMenu;	// Main Menu Smart pointer



	JGotTheRuns()
	{
		// Name your application
		sAppName = "Johnny Got The Runs... OLC CodeJam 2024";

		// Instantiate out smart pointer
		pMainMenu = std::make_unique<olc::MainMenu>();	// Main Menu


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
		sprTemp = new olc::Sprite("./assets/images/oip.jpg");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		Clear(olc::BLACK);
		DrawString({ 50, 100 }, "Johnny Got The Runs...");
		DrawString({ 50, 150 }, "Can you get Johnny to the Throne in time...");

		pMainMenu->DrawDecal();

		DrawSprite({ 100, 200 }, sprTemp);
		return true;
	}
};

int main()
{
	JGotTheRuns demo;
	if (demo.Construct(640, 480, 1, 1))
		demo.Start();
	return 0;
}
