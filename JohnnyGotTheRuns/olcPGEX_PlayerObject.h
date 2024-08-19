#pragma once
#include "pch.h"
#include <exception>

#ifdef OLC_PGEX_PLAYER_OBJECT

#endif

namespace olc
{
	class PlayerObject : public PGEX
	{

	public:

		// Object Actions
		enum ACTION
		{
			IDLE = 0,
			JUMP,
			FALL,
			DUCK,
			HIT,
			CLIMB,
			CHEER,
			BACK,
			SLIDE,
			INTERACT,
			SWITCH,
			KICK,
			SIDE,
			SHOVE,
			SHOVE_BACK,
			TALK,
			ATTACK_KICK,
			HANG,
			HOLD,
			SHOW,
			BEHIND_BACK,
			RUN,
			ATTACK,
			THINK,
			DOWN,
			DRAG,
			HURT,
			WIDE,
			ROPE,
			WALK,
			FALL_DOWN,
			NONE
		};

		/*
		* Default Constructor:
		* The class will create a temp blank sprite 10pxX10px
		* You will need to update the renImage and renSpriteSheet manually from your code
		*/
		PlayerObject();

		/*
		* Recommended Constructor:
		* The class will create and setup renImage or renSpriteSheet depending on bIsSpriteSheet flag
		* You can still update the renImage and renSpriteSheet manually from your code
		*/
		PlayerObject(std::string ImagePath, bool bIsSpriteSheet = false);

		// virtual Default De-Constructor:
		virtual ~PlayerObject();

		// Fires just before the main OnUserCreate
		virtual void OnBeforeUserCreate() override;

		// Fires just After the main OnUserCreate
		virtual void OnAfterUserCreate() override;

		// Fires just before the main OnUserUpdate
		virtual bool OnBeforeUserUpdate(float& fElapsedTime) override;

		// Fires just After the main OnUserUpdate
		virtual void OnAfterUserUpdate(float fElapsedTime) override;

		// Changes the player action
		void UpdateAction(ACTION action);

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created decal
		void UpdatePlayer(float fElapsedTime);


	public:

		/*
		* Stores the image Source and Size for the sprImage
		* or for use in vecPartialImages
		*/
		struct ImageInfo
		{
			olc::vf2d vSource = { 0.0f, 0.0f }; // Source poisition to draw drawing from, default: {0.0f, 0.0f)
			olc::vf2d vSize = { 1.0f, 1.0f };	// The size of the image to be drawn, default: Full passed in image size, edit this to when using SpriteSheets to the location of the sprite
			olc::vf2d vScaleSize = { 1.0f, 1.0f }; // Auto generated, stores the scale size of the image in pixels
			olc::vf2d vScale = { 1.0f, 1.0f };	// Scaling factor (Decal Only), default: {1.0, 1.0}, when AutoScale is set this value will be automactically updated
			olc::Pixel pxTint = olc::WHITE;		// Tint colour for background, set to olc::DARK_GREY for a night time effect
		};

		struct Circle {

			olc::vf2d vfCenterPos = { 0.0f, 0.0f };
			float fRadius = 0.0f;
		};

		/*
		* This Object Properites Struct
		*/
		struct ObjectProperites
		{
			std::string strName = "GameObject";		// Object name. Default "GameObject"
			int8_t nPlayerNumber = 0;				// Object number, Default 0 i.e. Player 1 , Player 2 etc

			int32_t nLives = 3;						// Lives, Default 3
			bool bIsGodMode = false;				// Stores if the player is in God Mode, Default: false
			bool bIsVisiable = true;				// Enable/Disable visiabilty 
			float fGodModeTimeOutSeconds = 10.0f;	// Stores the timeout for God mode, Default: 3000ms
			float fGodModeFlashSeconds = 0.2f;		// Flash timer for when the player is in God Mode

			olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

			olc::vi2d viStartPosition = { 0,0 };	// Start Position {x,y} (Int32_t), Default {0,0}

			olc::vi2d viPosition = { 0,0 };	        // Player current POS {x,y} (Int32_t), Default {0,0}
			olc::vf2d vfPosition = { 0.0f,0.0f };	// Player current POS {x,y} (float), Default {0.0f,0.0f}, recommended for decals

			std::string strSpritePath = "";			// Sprite path, i.e. "images/mysprite.png", Default: ""
			olc::Sprite* sprImage = nullptr;		// Player Sprite image, Default nullptr
			olc::Decal* decImage = nullptr;			// Player Decal Image, Defalut nullptr
			ImageInfo sprImageInfo;					// Stores the Source and Size of the sprImage

			std::string strSpriteSheetPath = "";    // SpriteSheetPath path, i.e. "images/mysprite.png", Default: ""
			olc::Sprite* sprSpriteSheet = nullptr;	// Player Sprite Sheet image, Default nullptr
			olc::Decal* decSpriteSheet = nullptr;	// Plaer Decal Sheet Image, Defalut nullptr

			olc::Renderable renImage;				// Keeps the sprImage and decImage in the one location
			olc::Renderable renSpriteSheet;			// Keeps the sprSpriteSheet and decSpriteSheet in the one location

			olc::vf2d vfMasterScaler;				// Master Scaler is used to scale the overall object, this is applied before AutoScaler

		};


		/*
		* Proprties
		*/
		ObjectProperites Properties;

		// This is the collision circle(s)
		Circle collCircle;

	private:
		bool bIsEmptySprite = false;
		bool bisSpriteSheet = false;
		std::string strImagePath = "";
		float fGodModeTimer = 0.0f;         // Use to timeout GodMode when the timeout is reached
		float fGodModeFlashTimer = 0.0f;    // Used to flash the Decal while in God mode

		

		// Action Map to link the Enum to a String
		std::map<ACTION, std::string> mActionMap;

		// Stuct used for creating the different actions frames, walk, run etc
		struct ActionFrame
		{
			ACTION nActionID = IDLE;                // Action ID, Default: IDLE
			bool bActive = false;					// Is Active, Default: false
			std::string strActionName = "Unknown";  // Action name, i.e. "Walk", "Jump" etc. , Default "Unknown"
			
			uint32_t nMaxFrames = 0;                // Max number of frames, Default 0, this means we are not anitmating this object
			uint32_t nCurrentFrame = 0;             // Stores the number of the last frame displayed
			uint32_t nCurrentFPS = 0;               // Stores the current Engine FPS

			float fFrameChangeRate = 0.0f;          // This value will auto calcuate the rate of change depending on the current FPS
			float fFrameElapsedTime = 0.0f;         // Keeps track of the time pass since the last frame change
			float fFramesPerSecound = 1.0f;         // Set the number of times the frame is to change per second, Default: 1.0f

			/*
			* Stores the location of the partial image from the sprSpriteSheet
			* ImageInfo.vSource {x,y} of the top left of the partial image to draw
			* ImageInfo.vSize {w, h} size of the partial image to be drawn
			*/
			std::vector<ImageInfo> vecPartialImages; // 

		};
		ActionFrame sActionFrame;

		// Loads the actions to the frames,
		// TODO: convert to XML loader
		void LoadActionFrames();

		// Change the frame, depending on frames per second
		void ChangeFrame(ActionFrame* pActionFrame, float fElapsedTime);

	public:

		/*
		*  Yes our player has LOTS of actions
		*/

		std::vector<ActionFrame> vecActionFrames;

	};

}


#ifdef OLC_PGEX_PLAYER_OBJECT
#undef OLC_PGEX_GAME_OBJECT

namespace olc
{


	PlayerObject::PlayerObject() : PGEX(true)
	{
		bIsEmptySprite = true;
	}

	PlayerObject::PlayerObject(std::string ImagePath, bool bIsSpriteSheet) : PGEX(true)
	{
		bisSpriteSheet = bIsSpriteSheet;
		strImagePath = ImagePath;
	}


	PlayerObject::~PlayerObject()
	{

		// lets clean up
		vecActionFrames.clear();

		if (Properties.decImage != nullptr)
		{
			delete Properties.decImage;
		}

		if (Properties.sprImage != nullptr)
		{
			delete Properties.sprImage;
		}

		if (Properties.decSpriteSheet != nullptr)
		{
			delete Properties.decSpriteSheet;
		}

		if (Properties.sprSpriteSheet != nullptr)
		{
			delete Properties.sprSpriteSheet;
		}

		

	}

	void PlayerObject::OnBeforeUserCreate()
	{
		// Ok let create our player object

		if (bIsEmptySprite)
		{
			Properties.renImage.Create(10, 10);
			Properties.sprImage = Properties.renImage.Sprite();
			Properties.decImage = Properties.renImage.Decal();
			Properties.sprImageInfo.vSource = { 0.0f, 0.0f };
			Properties.sprImageInfo.vSize = { 10.0f, 10.0f };
			return;
		}


		// There is a chance that our developer decided to update the properities after instantiated this class using the default construtor
		// and now has updated the properites before this execution
		// Therefore the below code checks and ensures these "new" properties are applied

		if (bisSpriteSheet)
		{
			// lets create Sprite Sheet
			Properties.strSpriteSheetPath = strImagePath;
			Properties.renSpriteSheet.Load(Properties.strSpriteSheetPath);
			Properties.sprSpriteSheet = Properties.renSpriteSheet.Sprite();
			Properties.decSpriteSheet = Properties.renSpriteSheet.Decal();
			Properties.sprImageInfo.vSize = Properties.renSpriteSheet.Sprite()->Size();

		}
		else
		{
			// Lets create a sprite
			Properties.strSpritePath = strImagePath;
			Properties.renImage.Load(Properties.strSpritePath);
			Properties.sprImage = Properties.renImage.Sprite();
			Properties.decImage = Properties.renImage.Decal();
			Properties.sprImageInfo.vSize = Properties.renImage.Sprite()->Size();

		}


		Properties.vfPosition = Properties.viStartPosition;
		Properties.viPosition = Properties.vfPosition;

		LoadActionFrames();

	}

	void PlayerObject::OnAfterUserCreate()
	{

	}

	bool PlayerObject::OnBeforeUserUpdate(float& fElapsedTime)
	{

		/* Update our postion  */

		// Ok we are falling....
		//Properties.vfPosition.y += Properties.vfVelocity.y * fElapsedTime;

		// Have we hit the bottom?
		if (Properties.vfPosition.y > (pge->ScreenHeight() - Properties.sprImageInfo.vSize.y))
			Properties.vfPosition.y = pge->ScreenHeight() - Properties.sprImageInfo.vSize.y;

		//if (pge->GetTouch().bReleased)
		//{
			// We are tap...tap..tapping
	   //     Properties.vfPosition.y -= Properties.vfVelocity.y * 200.0f * fElapsedTime;
		//}

		// Have we hit the top
		if (Properties.vfPosition.y < 1.0f)
			Properties.vfPosition.y = 1.0f;

		// Decals uses vf2f (floats) while Sprites use vi2d (int32_t), let case vfPos to vnPos
		Properties.viPosition = Properties.vfPosition;

		/* END Update our postion  */


		// Ok now for the fun part, 
		// We need to update the output decal so the main engine knows what to draw
		// To do this we use the public method to drawObject
		/// DrawDecal and DrawSprite methods

		/* End Update our frame  */

		return false;
	}

	void PlayerObject::OnAfterUserUpdate(float fElapsedTime)
	{
		if (Properties.bIsGodMode)
		{
			fGodModeFlashTimer += fElapsedTime;
			if (fGodModeFlashTimer > Properties.fGodModeFlashSeconds)
			{
				Properties.bIsVisiable = !Properties.bIsVisiable;
				fGodModeFlashTimer = 0.0f;
			}

			fGodModeTimer += fElapsedTime;
			if (fGodModeTimer > Properties.fGodModeTimeOutSeconds)
			{
				Properties.bIsVisiable = true;
				Properties.bIsGodMode = false;
			}

		}
		else
		{
			fGodModeFlashTimer = 0.0f;
			fGodModeTimer = 0.0f;
		}
	}

	void PlayerObject::UpdateAction(ACTION action)
	{
		for (auto& actionFrame : vecActionFrames)
		{
			actionFrame.bActive = false;
			if (actionFrame.nActionID == action)
			{
				actionFrame.bActive = true;
			}
		}
	}

	void PlayerObject::UpdatePlayer(float fElapsedTime)
	{

		for (auto& actionFrame : vecActionFrames)
		{
			if (actionFrame.bActive)
			{
				// 1: Is the vector empty
				if (actionFrame.vecPartialImages.empty()) { break; }

				// 2:
				actionFrame.fFrameChangeRate = 1.0f / actionFrame.fFramesPerSecound;
				actionFrame.fFrameElapsedTime += fElapsedTime;

				if (actionFrame.fFrameElapsedTime > actionFrame.fFrameChangeRate)
				{
					actionFrame.fFrameElapsedTime = 0.0f;
					actionFrame.nCurrentFrame++;
				}
				if (actionFrame.nCurrentFrame >= actionFrame.vecPartialImages.size()) { actionFrame.nCurrentFPS = 0; }

				// 3: Update our Properties to tell dev what is happening
				Properties.sprImageInfo.vSource = actionFrame.vecPartialImages[actionFrame.nCurrentFrame].vSource;
				Properties.sprImageInfo.vSize = actionFrame.vecPartialImages[actionFrame.nCurrentFrame].vSize;

				// 4: Draw the current decal frame
				if (Properties.bIsVisiable)
				{
					pge->DrawPartialDecal(
						Properties.vfPosition,
						Properties.decSpriteSheet,
						Properties.sprImageInfo.vSource,
						Properties.sprImageInfo.vSize,
						Properties.sprImageInfo.vScale,
						Properties.sprImageInfo.pxTint
					);
				}
					
			}
		}

		// Now that we have our Player position we need to setup our collision circle
		// NOTE: The Decal World and Sprite World do not aline, you made need to play with the values to get it perfect
		// Our collision circle will have an approx center of the decal with an approx radius

		// 1: Lets get our center point
		collCircle.vfCenterPos = Properties.vfPosition + (Properties.sprImageInfo.vSize / 2);
		collCircle.vfCenterPos *= Properties.sprImageInfo.vScale;

		// 2: Lets get the radius, we want the small circle that can fit within the decal
		collCircle.fRadius = (std::min(Properties.sprImageInfo.vSize.x, Properties.sprImageInfo.vSize.y) / 2) 
								* std::min(Properties.sprImageInfo.vScale.x, Properties.sprImageInfo.vScale.y);

		pge->FillCircle(collCircle.vfCenterPos, collCircle.fRadius, olc::GREEN);

	}

	void PlayerObject::ChangeFrame(ActionFrame* pActionFrame, float fElapsedTime)
	{
		// Calculate the rate of change from frames
		

	}


	void PlayerObject::LoadActionFrames()
	{
		// Ok, this is LONG method as I have not implemented the XML Parser yet: TODO: Implement XML
		// For the moment we will manually add the info... just want to get it up and running folks

		// IDLE
		ActionFrame actFrame;
		actFrame.nActionID = ACTION::IDLE;
		actFrame.strActionName = "idle";
		actFrame.nMaxFrames = 1;
		ImageInfo sImageInfo;
		sImageInfo.vSource = { 0.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// JUMP
		actFrame.nActionID = ACTION::JUMP;
		actFrame.strActionName = "jump";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 192.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// FALL
		actFrame.nActionID = ACTION::FALL;
		actFrame.strActionName = "fall";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 384.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// DUCK
		actFrame.nActionID = ACTION::DUCK;
		actFrame.strActionName = "duck";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 576.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// HIT
		actFrame.nActionID = ACTION::HIT;
		actFrame.strActionName = "hit";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 768.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// CLIMB
		actFrame.nActionID = ACTION::CLIMB;
		actFrame.strActionName = "climb";
		actFrame.nMaxFrames = 2;

		sImageInfo.vSource = { 960.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		sImageInfo.vSource = { 1152.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// CHEER
		actFrame.nActionID = ACTION::CHEER;
		actFrame.strActionName = "cheer";
		actFrame.nMaxFrames = 2;

		sImageInfo.vSource = { 1344.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		sImageInfo.vSource = { 1536.0f, 0.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// BACK
		actFrame.nActionID = ACTION::BACK;
		actFrame.strActionName = "back";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 0.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// SLIDE
		actFrame.nActionID = ACTION::SLIDE;
		actFrame.strActionName = "slide";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 192.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// INTERACT
		actFrame.nActionID = ACTION::INTERACT;
		actFrame.strActionName = "interact";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 384.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// SWiTCH
		actFrame.nActionID = ACTION::SWITCH;
		actFrame.strActionName = "switch";
		actFrame.nMaxFrames = 2;

		sImageInfo.vSource = { 576.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		sImageInfo.vSource = { 768.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// KICK
		actFrame.nActionID = ACTION::KICK;
		actFrame.strActionName = "kick";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 960.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// SIDE
		actFrame.nActionID = ACTION::SIDE;
		actFrame.strActionName = "side";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1152.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// SHOVE
		actFrame.nActionID = ACTION::SHOVE;
		actFrame.strActionName = "shove";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1344.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// SHOVE_BACK
		actFrame.nActionID = ACTION::SHOVE_BACK;
		actFrame.strActionName = "shoveBack";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1536.0f, 256.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// TALK
		actFrame.nActionID = ACTION::TALK;
		actFrame.strActionName = "talk";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 0.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// ATTACK_KICK
		actFrame.nActionID = ACTION::ATTACK_KICK;
		actFrame.strActionName = "attackKick";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 192.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// HANG
		actFrame.nActionID = ACTION::HANG;
		actFrame.strActionName = "hang";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 384.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// HOLD
		actFrame.nActionID = ACTION::HOLD;
		actFrame.strActionName = "hold";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 576.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// SHOW
		actFrame.nActionID = ACTION::SHOW;
		actFrame.strActionName = "show";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 768.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// BEHIND_BACK
		actFrame.nActionID = ACTION::BEHIND_BACK;
		actFrame.strActionName = "behindBack";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 960.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// RUN
		actFrame.nActionID = ACTION::RUN;
		actFrame.strActionName = "run";
		actFrame.nMaxFrames = 3;

		sImageInfo.vSource = { 1152.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		sImageInfo.vSource = { 1344.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		sImageInfo.vSource = { 1536.0f, 512.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// ATTACK
		actFrame.nActionID = ACTION::ATTACK;
		actFrame.strActionName = "attack";
		actFrame.nMaxFrames = 3;

		sImageInfo.vSource = { 0.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		sImageInfo.vSource = { 192.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		sImageInfo.vSource = { 384.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// THINK
		actFrame.nActionID = ACTION::THINK;
		actFrame.strActionName = "think";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 576.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// DOWN
		actFrame.nActionID = ACTION::DOWN;
		actFrame.strActionName = "down";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 768.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// DRAG
		actFrame.nActionID = ACTION::DRAG;
		actFrame.strActionName = "drag";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 960.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// HURT
		actFrame.nActionID = ACTION::HURT;
		actFrame.strActionName = "hurt";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1152.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// WIDE
		actFrame.nActionID = ACTION::WIDE;
		actFrame.strActionName = "wide";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1344.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// ROPE
		actFrame.nActionID = ACTION::ROPE;
		actFrame.strActionName = "rope";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1536.0f, 768.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// WALK
		actFrame.nActionID = ACTION::WALK;
		actFrame.strActionName = "walk";
		actFrame.nMaxFrames = 8;

		// frame 0
		sImageInfo.vSource = { 0.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 1
		sImageInfo.vSource = { 192.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 2
		sImageInfo.vSource = { 384.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 3
		sImageInfo.vSource = { 576.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 4
		sImageInfo.vSource = { 768.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 5
		sImageInfo.vSource = { 960.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 6
		sImageInfo.vSource = { 1152.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);

		// frame 7
		sImageInfo.vSource = { 1344.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);

		// FALL_DOWN
		actFrame.nActionID = ACTION::FALL_DOWN;
		actFrame.strActionName = "fallDOWN";
		actFrame.nMaxFrames = 1;

		sImageInfo.vSource = { 1536.0f, 1024.0f };
		sImageInfo.vSize = { 192.0f, 256.0f };
		actFrame.vecPartialImages.push_back(sImageInfo);
		vecActionFrames.push_back(actFrame);


	}

	

} // olc

#endif
