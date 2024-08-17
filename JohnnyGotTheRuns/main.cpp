#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class JGotTheRuns : public olc::PixelGameEngine
{
public:
	JGotTheRuns()
	{
		// Name your application
		sAppName = "Johnny Got The Runs... OLC CodeJam 2024";
	}

	/* Sprite */
	olc::Sprite* sprTemp;

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
