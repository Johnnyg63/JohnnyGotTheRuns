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


		enum OBJECT_TYPE
		{
			NONE = 0,		// None object type
			PLAYER,			// Main player 
			GAME_CHAR,		// Game character
			OTHER_PLAYER,	// Network players
			ENEMY			// Standard Enemy

		};

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
			FALL_DOWN
		};

		/*
		* Default Constructor:
		* The class will create a temp blank sprite 10pxX10px
		* You will need to update the renImage and renSpriteSheet manually from your code
		*/
		PlayerObject();

		/*
		* Recommended Constructor:
		* strSpriteSheetPath: Full path to your sprite sheet
		* bIsPlayer: true: Players, false: Game Object
		*/
		PlayerObject(std::string strSpriteSheetPath, OBJECT_TYPE eObjectType = OBJECT_TYPE::NONE);

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

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the updated decal
		void Update(float fElapsedTime);


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
			int8_t nObjectNumber = 0;				// Object number, Default 0 i.e. Player 1 , Player 2 etc
			OBJECT_TYPE eObjectType = OBJECT_TYPE::NONE;	// Object Type: PLAYER, GAME_CHAR, OTHER_PLAYER, ENEMY, Default: OBJECT_TYPE::NONE

			uint16_t nObjectID = 0;					// An Unique Object ID, Default: Auto Genterated (Edit if you dare!)

			int32_t nLives = 3;						// Lives, Default 3
			bool bIsGodMode = false;				// Stores if the player is in God Mode, Default: false
			bool bIsVisiable = true;				// Enable/Disable visiabilty 
			float fGodModeTimeOutSeconds = 10.0f;	// Stores the timeout for God mode, Default: 3000ms
			float fGodModeFlashSeconds = 0.2f;		// Flash timer for when the player is in God Mode

			olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

			olc::vi2d vfStartPosition = { 0,0 };	// Start Position {x,y} (Int32_t), Default {0,0}

			olc::vf2d vfPosition = { 0.0f,0.0f };	// Player current POS {x,y} (float), Default {0.0f,0.0f}, recommended for decals

			olc::vf2d vfPotentialPosition = { 0.0f,0.0f };	// Player potential position used for collision 

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
		std::string strImagePath = "";
		float fGodModeTimer = 0.0f;         // Use to timeout GodMode when the timeout is reached
		float fGodModeFlashTimer = 0.0f;    // Used to flash the Decal while in God mode

		bool bWarpFrame = false;				// used to warp the decal 

		olc::vf2d vfLeft = { -1.0f, 1.0f };		// Left Direction vector 
		olc::vf2d vfRight = { 1.0f, 1.0f };		// Right Direction vector 
		olc::vf2d vfDirection = vfRight;		// Direction vector Default: vfRight
		bool bisFlipped = false;				// Use to manage Decal offset when flipped




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
			float fFramesPerSecound = 10.0f;         // Set the number of times the frame is to change per second, Default: 1.0f

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
		olc::vf2d WarpFrame(ACTION action);

	public:

		/*
		*  Yes our player has LOTS of actions
		*/

		std::vector<ActionFrame> vecActionFrames;

	private:
		std::vector<olc::vf2d> MakeUnitCircle(const size_t verts = 64);
		std::vector<olc::vf2d> vertsUnitCircle = MakeUnitCircle();

		void DrawCircleDecal(const olc::vf2d& vPos, const float fRadius, const olc::Pixel colour = olc::WHITE);

		void FillCircleDecal(const olc::vf2d& vPos, const float fRadius, const olc::Pixel colour = olc::WHITE);

	};

}


#ifdef OLC_PGEX_PLAYER_OBJECT
#undef OLC_PGEX_PLAYER_OBJECT

namespace olc
{


	PlayerObject::PlayerObject() : PGEX(true)
	{
		bIsEmptySprite = true;
		Properties.eObjectType = OBJECT_TYPE::NONE;

	}

	PlayerObject::PlayerObject(std::string strSpriteSheetPath, OBJECT_TYPE eObjectType) : PGEX(true)
	{
		strImagePath = strSpriteSheetPath;
		Properties.eObjectType = eObjectType;
	}


	PlayerObject::~PlayerObject()
	{
		// lets clean up


	}

	void PlayerObject::OnBeforeUserCreate()
	{
		// Ok let create our player object

		if (bIsEmptySprite)
		{
			Properties.renImage.Create(10, 10);
			Properties.sprImageInfo.vSource = { 0.0f, 0.0f };
			Properties.sprImageInfo.vSize = { 10.0f, 10.0f };
			return;
		}


		// There is a chance that our developer decided to update the properities after instantiated this class using the default construtor
		// and now has updated the properites before this execution
		// Therefore the below code checks and ensures these "new" properties are applied

		// lets create Sprite Sheet
		Properties.strSpriteSheetPath = strImagePath;
		Properties.renSpriteSheet.Load(Properties.strSpriteSheetPath);
		Properties.sprSpriteSheet = Properties.renSpriteSheet.Sprite();
		Properties.decSpriteSheet = Properties.renSpriteSheet.Decal();
		Properties.sprImageInfo.vSize = Properties.renSpriteSheet.Sprite()->Size();

		LoadActionFrames();

	}

	void PlayerObject::OnAfterUserCreate()
	{

	}

	bool PlayerObject::OnBeforeUserUpdate(float& fElapsedTime)
	{
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

	void PlayerObject::Update(float fElapsedTime)
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
				if (actionFrame.nCurrentFrame >= actionFrame.vecPartialImages.size())
				{
					actionFrame.nCurrentFrame = 0;
				}

				Properties.sprImageInfo.vSource = actionFrame.vecPartialImages[actionFrame.nCurrentFrame].vSource;
				Properties.sprImageInfo.vSize = actionFrame.vecPartialImages[actionFrame.nCurrentFrame].vSize;
				Properties.sprImageInfo.vScale = WarpFrame(actionFrame.nActionID);
				Properties.sprImageInfo.vScaleSize = Properties.sprImageInfo.vSize * Properties.sprImageInfo.vScale;
				Properties.sprImageInfo.vScale *= vfDirection;

				olc::vf2d vfDecolPos = Properties.vfPosition;

				/*
				* Player only options
				*/

				switch (Properties.eObjectType)
				{
					case PlayerObject::OBJECT_TYPE::ENEMY:
					{
						// TODO: 
						break;
					}
					case PlayerObject::OBJECT_TYPE::GAME_CHAR:
					{
						// TODO:
						break;
					}
					case PlayerObject::OBJECT_TYPE::NONE:
					{
						// TODO:
						break;
					}
					case PlayerObject::OBJECT_TYPE::OTHER_PLAYER:
					{
						// For the player we update the vTrackpoint as this controlls the player position
						// TODO
						break;
					}
					case PlayerObject::OBJECT_TYPE::PLAYER:
					{
						if (pge->GetKey(olc::Key::RIGHT).bPressed)
						{

							bisFlipped = false;
							vfDirection = vfRight;
						}

						if (pge->GetKey(olc::Key::LEFT).bPressed)
						{
							bisFlipped = true;

							vfDirection = vfLeft;
						}

						if (bisFlipped)
						{
							vfDecolPos.x += Properties.sprImageInfo.vScaleSize.x;
						}

						break;
					}

				default:
					break;
				}

				// 5: Draw the current decal frame
				if (Properties.bIsVisiable)
				{
					pge->DrawPartialDecal(
						vfDecolPos,
						Properties.decSpriteSheet,
						Properties.sprImageInfo.vSource,
						Properties.sprImageInfo.vSize,
						Properties.sprImageInfo.vScale,
						Properties.sprImageInfo.pxTint
					);
				}


				// TODO: Add Support for when the player is in DUCK mode
				//6: Lets get our collision circle
				collCircle.vfCenterPos = Properties.vfPosition + (Properties.sprImageInfo.vScaleSize / 2);
				collCircle.fRadius = (std::min(Properties.sprImageInfo.vScaleSize.x, Properties.sprImageInfo.vScaleSize.y) / 2);
				// Move the circle to the buttom of he decal, we care about the players feet position
				collCircle.vfCenterPos.y += (Properties.sprImageInfo.vScaleSize.y / 2) - collCircle.fRadius;

				// Show the circle for debugging, thank Javid
				//pge->DrawRectDecal(Properties.vfPosition, Properties.sprImageInfo.vScaleSize, olc::BLUE);

			}
		}

		// Now that we have our Player position we need to setup our collision circle
		// NOTE: The Decal World and Sprite World do not aline, you made need to play with the values to get it perfect
		// Our collision circle will have an approx center of the decal with an approx radius

		//DrawCircleDecal(collCircle.vfCenterPos, collCircle.fRadius, olc::GREEN);

	}

	// Javidx9 Stuff

	// PGE doesnt have a DrawCircleDEcal routine by default for a number
	// of reasons, so I've made one here that exploits a unit circle
	// being pre-made...

	std::vector<olc::vf2d> PlayerObject::MakeUnitCircle(const size_t verts)
	{
		std::vector<olc::vf2d> vOut(verts, { 0,0 });
		float anglestep = 2.0f * 3.14159f / float(verts - 1);
		for (size_t i = 0; i < verts; i++)
			vOut[i] = olc::vf2d(1.0f, anglestep * float(i)).cart();
		return vOut;
	}

	void PlayerObject::DrawCircleDecal(const olc::vf2d& vPos, const float fRadius, const olc::Pixel colour)
	{
		std::vector<olc::vf2d> vDraw(vertsUnitCircle.size(), { 0,0 });
		std::transform(vertsUnitCircle.begin(), vertsUnitCircle.end(), vDraw.begin(),
			[&](const olc::vf2d& vIn) { return vIn * fRadius + vPos; });
		pge->SetDecalMode(olc::DecalMode::WIREFRAME);
		pge->DrawPolygonDecal(nullptr, vDraw, vDraw, colour);
		pge->SetDecalMode(olc::DecalMode::NORMAL);
	}

	void PlayerObject::FillCircleDecal(const olc::vf2d& vPos, const float fRadius, const olc::Pixel colour)
	{
		std::vector<olc::vf2d> vDraw(vertsUnitCircle.size(), { 0,0 });
		std::transform(vertsUnitCircle.begin(), vertsUnitCircle.end(), vDraw.begin(),
			[&](const olc::vf2d& vIn) { return vIn * fRadius + vPos; });
		pge->SetDecalMode(olc::DecalMode::NORMAL);
		pge->DrawPolygonDecal(nullptr, vDraw, vDraw, colour);
	}



	void PlayerObject::LoadActionFrames()
	{
		// Ok, this is LONG method as I have not implemented the XML Parser yet: TODO: Implement XML
		// For the moment we will manually add the info... just want to get it up and running folks

		// IDLE
		ActionFrame actFrame_IDLE;
		ImageInfo sImageInfo_IDLE;
		actFrame_IDLE.nActionID = ACTION::IDLE;
		actFrame_IDLE.strActionName = "idle";
		actFrame_IDLE.nMaxFrames = 1;

		sImageInfo_IDLE.vSource = { 0.0f, 0.0f };
		sImageInfo_IDLE.vSize = { 192.0f, 256.0f };
		actFrame_IDLE.vecPartialImages.push_back(sImageInfo_IDLE);
		vecActionFrames.push_back(actFrame_IDLE);

		// JUMP
		ActionFrame actFrame_JUMP;
		ImageInfo sImageInfo_JUMP;
		actFrame_JUMP.nActionID = ACTION::JUMP;
		actFrame_JUMP.strActionName = "jump";
		actFrame_JUMP.nMaxFrames = 1;

		sImageInfo_JUMP.vSource = { 192.0f, 0.0f };
		sImageInfo_JUMP.vSize = { 192.0f, 256.0f };
		actFrame_JUMP.vecPartialImages.push_back(sImageInfo_JUMP);
		vecActionFrames.push_back(actFrame_JUMP);

		// FALL
		ActionFrame actFrame_FALL;
		ImageInfo sImageInfo_FALL;
		actFrame_FALL.nActionID = ACTION::FALL;
		actFrame_FALL.strActionName = "fall";
		actFrame_FALL.nMaxFrames = 1;

		sImageInfo_FALL.vSource = { 384.0f, 0.0f };
		sImageInfo_FALL.vSize = { 192.0f, 256.0f };
		actFrame_FALL.vecPartialImages.push_back(sImageInfo_FALL);
		vecActionFrames.push_back(actFrame_FALL);

		// DUCK
		ActionFrame actFrame_DUCK;
		ImageInfo sImageInfo_DUCK;
		actFrame_DUCK.nActionID = ACTION::DUCK;
		actFrame_DUCK.strActionName = "duck";
		actFrame_DUCK.nMaxFrames = 1;

		sImageInfo_DUCK.vSource = { 576.0f, 0.0f };
		sImageInfo_DUCK.vSize = { 192.0f, 256.0f };
		actFrame_DUCK.vecPartialImages.push_back(sImageInfo_DUCK);
		vecActionFrames.push_back(actFrame_DUCK);

		// HIT
		ActionFrame actFrame_HIT;
		ImageInfo sImageInfo_HIT;
		actFrame_HIT.nActionID = ACTION::HIT;
		actFrame_HIT.strActionName = "hit";
		actFrame_HIT.nMaxFrames = 1;

		sImageInfo_HIT.vSource = { 768.0f, 0.0f };
		sImageInfo_HIT.vSize = { 192.0f, 256.0f };
		actFrame_HIT.vecPartialImages.push_back(sImageInfo_HIT);
		vecActionFrames.push_back(actFrame_HIT);

		// CLIMB
		ActionFrame actFrame_CLIMB;
		ImageInfo sImageInfo_CLIMB;
		actFrame_CLIMB.nActionID = ACTION::CLIMB;
		actFrame_CLIMB.strActionName = "climb";
		actFrame_CLIMB.nMaxFrames = 2;

		sImageInfo_CLIMB.vSource = { 960.0f, 0.0f };
		sImageInfo_CLIMB.vSize = { 192.0f, 256.0f };
		actFrame_CLIMB.vecPartialImages.push_back(sImageInfo_CLIMB);

		sImageInfo_CLIMB.vSource = { 1152.0f, 0.0f };
		sImageInfo_CLIMB.vSize = { 192.0f, 256.0f };
		actFrame_CLIMB.vecPartialImages.push_back(sImageInfo_CLIMB);
		vecActionFrames.push_back(actFrame_CLIMB);

		// CHEER
		ActionFrame actFrame_CHEER;
		ImageInfo sImageInfo_CHEER;
		actFrame_CHEER.nActionID = ACTION::CHEER;
		actFrame_CHEER.strActionName = "cheer";
		actFrame_CHEER.nMaxFrames = 2;

		sImageInfo_CHEER.vSource = { 1344.0f, 0.0f };
		sImageInfo_CHEER.vSize = { 192.0f, 256.0f };
		actFrame_CHEER.vecPartialImages.push_back(sImageInfo_CHEER);

		sImageInfo_CHEER.vSource = { 1536.0f, 0.0f };
		sImageInfo_CHEER.vSize = { 192.0f, 256.0f };
		actFrame_CHEER.vecPartialImages.push_back(sImageInfo_CHEER);
		vecActionFrames.push_back(actFrame_CHEER);

		// BACK
		ActionFrame actFrame_BACK;
		ImageInfo sImageInfo_BACK;
		actFrame_BACK.nActionID = ACTION::BACK;
		actFrame_BACK.strActionName = "back";
		actFrame_BACK.nMaxFrames = 1;

		sImageInfo_BACK.vSource = { 0.0f, 256.0f };
		sImageInfo_BACK.vSize = { 192.0f, 256.0f };
		actFrame_BACK.vecPartialImages.push_back(sImageInfo_BACK);
		vecActionFrames.push_back(actFrame_BACK);

		// SLIDE
		ActionFrame actFrame_SLIDE;
		ImageInfo sImageInfo_SLIDE;
		actFrame_SLIDE.nActionID = ACTION::SLIDE;
		actFrame_SLIDE.strActionName = "slide";
		actFrame_SLIDE.nMaxFrames = 1;

		sImageInfo_SLIDE.vSource = { 192.0f, 256.0f };
		sImageInfo_SLIDE.vSize = { 192.0f, 256.0f };
		actFrame_SLIDE.vecPartialImages.push_back(sImageInfo_SLIDE);
		vecActionFrames.push_back(actFrame_SLIDE);

		// INTERACT
		ActionFrame actFrame_INTERACT;
		ImageInfo sImageInfo_INTERACT;
		actFrame_INTERACT.nActionID = ACTION::INTERACT;
		actFrame_INTERACT.strActionName = "interact";
		actFrame_INTERACT.nMaxFrames = 1;

		sImageInfo_INTERACT.vSource = { 384.0f, 256.0f };
		sImageInfo_INTERACT.vSize = { 192.0f, 256.0f };
		actFrame_INTERACT.vecPartialImages.push_back(sImageInfo_INTERACT);
		vecActionFrames.push_back(actFrame_INTERACT);

		// SWITCH
		ActionFrame actFrame_SWITCH;
		ImageInfo sImageInfo_SWITCH;
		actFrame_SWITCH.nActionID = ACTION::SWITCH;
		actFrame_SWITCH.strActionName = "switch";
		actFrame_SWITCH.nMaxFrames = 2;

		sImageInfo_SWITCH.vSource = { 576.0f, 256.0f };
		sImageInfo_SWITCH.vSize = { 192.0f, 256.0f };
		actFrame_SWITCH.vecPartialImages.push_back(sImageInfo_SWITCH);

		sImageInfo_SWITCH.vSource = { 768.0f, 256.0f };
		sImageInfo_SWITCH.vSize = { 192.0f, 256.0f };
		actFrame_SWITCH.vecPartialImages.push_back(sImageInfo_SWITCH);
		vecActionFrames.push_back(actFrame_SWITCH);

		// KICK
		ActionFrame actFrame_KICK;
		ImageInfo sImageInfo_KICK;
		actFrame_KICK.nActionID = ACTION::KICK;
		actFrame_KICK.strActionName = "kick";
		actFrame_KICK.nMaxFrames = 1;

		sImageInfo_KICK.vSource = { 960.0f, 256.0f };
		sImageInfo_KICK.vSize = { 192.0f, 256.0f };
		actFrame_KICK.vecPartialImages.push_back(sImageInfo_KICK);
		vecActionFrames.push_back(actFrame_KICK);

		// SIDE
		ActionFrame actFrame_SIDE;
		ImageInfo sImageInfo_SIDE;
		actFrame_SIDE.nActionID = ACTION::SIDE;
		actFrame_SIDE.strActionName = "side";
		actFrame_SIDE.nMaxFrames = 1;

		sImageInfo_SIDE.vSource = { 1152.0f, 256.0f };
		sImageInfo_SIDE.vSize = { 192.0f, 256.0f };
		actFrame_SIDE.vecPartialImages.push_back(sImageInfo_SIDE);
		vecActionFrames.push_back(actFrame_SIDE);

		// SHOVE
		ActionFrame actFrame_SHOVE;
		ImageInfo sImageInfo_SHOVE;
		actFrame_SHOVE.nActionID = ACTION::SHOVE;
		actFrame_SHOVE.strActionName = "shove";
		actFrame_SHOVE.nMaxFrames = 1;

		sImageInfo_SHOVE.vSource = { 1344.0f, 256.0f };
		sImageInfo_SHOVE.vSize = { 192.0f, 256.0f };
		actFrame_SHOVE.vecPartialImages.push_back(sImageInfo_SHOVE);
		vecActionFrames.push_back(actFrame_SHOVE);

		// SHOVE_BACK
		ActionFrame actFrame_SHOVE_BACK;
		ImageInfo sImageInfo_SHOVE_BACK;
		actFrame_SHOVE_BACK.nActionID = ACTION::SHOVE_BACK;
		actFrame_SHOVE_BACK.strActionName = "shoveBack";
		actFrame_SHOVE_BACK.nMaxFrames = 1;

		sImageInfo_SHOVE_BACK.vSource = { 1536.0f, 256.0f };
		sImageInfo_SHOVE_BACK.vSize = { 192.0f, 256.0f };
		actFrame_SHOVE_BACK.vecPartialImages.push_back(sImageInfo_SHOVE_BACK);
		vecActionFrames.push_back(actFrame_SHOVE_BACK);

		// TALK
		ActionFrame actFrame_TALK;
		ImageInfo sImageInfo_TALK;
		actFrame_TALK.nActionID = ACTION::TALK;
		actFrame_TALK.strActionName = "talk";
		actFrame_TALK.nMaxFrames = 1;

		sImageInfo_TALK.vSource = { 0.0f, 512.0f };
		sImageInfo_TALK.vSize = { 192.0f, 256.0f };
		actFrame_TALK.vecPartialImages.push_back(sImageInfo_TALK);
		vecActionFrames.push_back(actFrame_TALK);

		// ATTACK_KICK
		ActionFrame actFrame_ATTACK_KICK;
		ImageInfo sImageInfo_ATTACK_KICK;
		actFrame_ATTACK_KICK.nActionID = ACTION::ATTACK_KICK;
		actFrame_ATTACK_KICK.strActionName = "attackKick";
		actFrame_ATTACK_KICK.nMaxFrames = 1;

		sImageInfo_ATTACK_KICK.vSource = { 192.0f, 512.0f };
		sImageInfo_ATTACK_KICK.vSize = { 192.0f, 256.0f };
		actFrame_ATTACK_KICK.vecPartialImages.push_back(sImageInfo_ATTACK_KICK);
		vecActionFrames.push_back(actFrame_ATTACK_KICK);

		// HANG
		ActionFrame actFrame_HANG;
		ImageInfo sImageInfo_HANG;
		actFrame_HANG.nActionID = ACTION::HANG;
		actFrame_HANG.strActionName = "hang";
		actFrame_HANG.nMaxFrames = 1;

		sImageInfo_HANG.vSource = { 384.0f, 512.0f };
		sImageInfo_HANG.vSize = { 192.0f, 256.0f };
		actFrame_HANG.vecPartialImages.push_back(sImageInfo_HANG);
		vecActionFrames.push_back(actFrame_HANG);

		// HOLD
		ActionFrame actFrame_HOLD;
		ImageInfo sImageInfo_HOLD;
		actFrame_HOLD.nActionID = ACTION::HOLD;
		actFrame_HOLD.strActionName = "hold";
		actFrame_HOLD.nMaxFrames = 1;

		sImageInfo_HOLD.vSource = { 576.0f, 512.0f };
		sImageInfo_HOLD.vSize = { 192.0f, 256.0f };
		actFrame_HOLD.vecPartialImages.push_back(sImageInfo_HOLD);
		vecActionFrames.push_back(actFrame_HOLD);

		// SHOW
		ActionFrame actFrame_SHOW;
		ImageInfo sImageInfo_SHOW;
		actFrame_SHOW.nActionID = ACTION::SHOW;
		actFrame_SHOW.strActionName = "show";
		actFrame_SHOW.nMaxFrames = 1;

		sImageInfo_SHOW.vSource = { 768.0f, 512.0f };
		sImageInfo_SHOW.vSize = { 192.0f, 256.0f };
		actFrame_SHOW.vecPartialImages.push_back(sImageInfo_SHOW);
		vecActionFrames.push_back(actFrame_SHOW);

		// BEHIND_BACK
		ActionFrame actFrame_BEHIND_BACK;
		ImageInfo sImageInfo_BEHIND_BACK;
		actFrame_BEHIND_BACK.nActionID = ACTION::BEHIND_BACK;
		actFrame_BEHIND_BACK.strActionName = "behindBack";
		actFrame_BEHIND_BACK.nMaxFrames = 1;

		sImageInfo_BEHIND_BACK.vSource = { 960.0f, 512.0f };
		sImageInfo_BEHIND_BACK.vSize = { 192.0f, 256.0f };
		actFrame_BEHIND_BACK.vecPartialImages.push_back(sImageInfo_BEHIND_BACK);
		vecActionFrames.push_back(actFrame_BEHIND_BACK);

		// RUN
		ActionFrame actFrame_RUN;
		ImageInfo sImageInfo_RUN;
		actFrame_RUN.nActionID = ACTION::RUN;
		actFrame_RUN.strActionName = "run";
		actFrame_RUN.nMaxFrames = 3;

		sImageInfo_RUN.vSource = { 1152.0f, 512.0f };
		sImageInfo_RUN.vSize = { 192.0f, 256.0f };
		actFrame_RUN.vecPartialImages.push_back(sImageInfo_RUN);

		sImageInfo_RUN.vSource = { 1344.0f, 512.0f };
		sImageInfo_RUN.vSize = { 192.0f, 256.0f };
		actFrame_RUN.vecPartialImages.push_back(sImageInfo_RUN);

		sImageInfo_RUN.vSource = { 1536.0f, 512.0f };
		sImageInfo_RUN.vSize = { 192.0f, 256.0f };
		actFrame_RUN.vecPartialImages.push_back(sImageInfo_RUN);
		vecActionFrames.push_back(actFrame_RUN);

		// ATTACK
		ActionFrame actFrame_ATTACK;
		ImageInfo sImageInfo_ATTACK;
		actFrame_ATTACK.nActionID = ACTION::ATTACK;
		actFrame_ATTACK.strActionName = "attack";
		actFrame_ATTACK.nMaxFrames = 3;

		sImageInfo_ATTACK.vSource = { 0.0f, 768.0f };
		sImageInfo_ATTACK.vSize = { 192.0f, 256.0f };
		actFrame_ATTACK.vecPartialImages.push_back(sImageInfo_ATTACK);

		sImageInfo_ATTACK.vSource = { 192.0f, 768.0f };
		sImageInfo_ATTACK.vSize = { 192.0f, 256.0f };
		actFrame_ATTACK.vecPartialImages.push_back(sImageInfo_ATTACK);

		sImageInfo_ATTACK.vSource = { 384.0f, 768.0f };
		sImageInfo_ATTACK.vSize = { 192.0f, 256.0f };
		actFrame_ATTACK.vecPartialImages.push_back(sImageInfo_ATTACK);
		vecActionFrames.push_back(actFrame_ATTACK);

		// THINK
		ActionFrame actFrame_THINK;
		ImageInfo sImageInfo_THINK;
		actFrame_THINK.nActionID = ACTION::THINK;
		actFrame_THINK.strActionName = "think";
		actFrame_THINK.nMaxFrames = 1;

		sImageInfo_THINK.vSource = { 576.0f, 768.0f };
		sImageInfo_THINK.vSize = { 192.0f, 256.0f };
		actFrame_THINK.vecPartialImages.push_back(sImageInfo_THINK);
		vecActionFrames.push_back(actFrame_THINK);

		// DOWN
		ActionFrame actFrame_DOWN;
		ImageInfo sImageInfo_DOWN;
		actFrame_DOWN.nActionID = ACTION::DOWN;
		actFrame_DOWN.strActionName = "down";
		actFrame_DOWN.nMaxFrames = 1;

		sImageInfo_DOWN.vSource = { 768.0f, 768.0f };
		sImageInfo_DOWN.vSize = { 192.0f, 256.0f };
		actFrame_DOWN.vecPartialImages.push_back(sImageInfo_DOWN);
		vecActionFrames.push_back(actFrame_DOWN);

		// DRAG
		ActionFrame actFrame_DRAG;
		ImageInfo sImageInfo_DRAG;
		actFrame_DRAG.nActionID = ACTION::DRAG;
		actFrame_DRAG.strActionName = "drag";
		actFrame_DRAG.nMaxFrames = 1;

		sImageInfo_DRAG.vSource = { 960.0f, 768.0f };
		sImageInfo_DRAG.vSize = { 192.0f, 256.0f };
		actFrame_DRAG.vecPartialImages.push_back(sImageInfo_DRAG);
		vecActionFrames.push_back(actFrame_DRAG);

		// HURT
		ActionFrame actFrame_HURT;
		ImageInfo sImageInfo_HURT;
		actFrame_HURT.nActionID = ACTION::HURT;
		actFrame_HURT.strActionName = "hurt";
		actFrame_HURT.nMaxFrames = 1;

		sImageInfo_HURT.vSource = { 1152.0f, 768.0f };
		sImageInfo_HURT.vSize = { 192.0f, 256.0f };
		actFrame_HURT.vecPartialImages.push_back(sImageInfo_HURT);
		vecActionFrames.push_back(actFrame_HURT);

		// WIDE
		ActionFrame actFrame_WIDE;
		ImageInfo sImageInfo_WIDE;
		actFrame_WIDE.nActionID = ACTION::WIDE;
		actFrame_WIDE.strActionName = "wide";
		actFrame_WIDE.nMaxFrames = 1;

		sImageInfo_WIDE.vSource = { 1344.0f, 768.0f };
		sImageInfo_WIDE.vSize = { 192.0f, 256.0f };
		actFrame_WIDE.vecPartialImages.push_back(sImageInfo_WIDE);
		vecActionFrames.push_back(actFrame_WIDE);

		// ROPE
		ActionFrame actFrame_ROPE;
		ImageInfo sImageInfo_ROPE;
		actFrame_ROPE.nActionID = ACTION::ROPE;
		actFrame_ROPE.strActionName = "rope";
		actFrame_ROPE.nMaxFrames = 1;

		sImageInfo_ROPE.vSource = { 1536.0f, 768.0f };
		sImageInfo_ROPE.vSize = { 192.0f, 256.0f };
		actFrame_ROPE.vecPartialImages.push_back(sImageInfo_ROPE);
		vecActionFrames.push_back(actFrame_ROPE);

		// WALK
		ActionFrame actFrame_WALK;
		ImageInfo sImageInfo_WALK;
		actFrame_WALK.nActionID = ACTION::WALK;
		actFrame_WALK.strActionName = "walk";
		actFrame_WALK.nMaxFrames = 8;

		// frame 0
		sImageInfo_WALK.vSource = { 0.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 1
		sImageInfo_WALK.vSource = { 192.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 2
		sImageInfo_WALK.vSource = { 384.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 3
		sImageInfo_WALK.vSource = { 576.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 4
		sImageInfo_WALK.vSource = { 768.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 5
		sImageInfo_WALK.vSource = { 960.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 6
		sImageInfo_WALK.vSource = { 1152.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);

		// frame 7
		sImageInfo_WALK.vSource = { 1344.0f, 1024.0f };
		sImageInfo_WALK.vSize = { 192.0f, 256.0f };
		actFrame_WALK.vecPartialImages.push_back(sImageInfo_WALK);
		vecActionFrames.push_back(actFrame_WALK);

		// FALL_DOWN
		ActionFrame actFrame_FALL_DOWN;
		ImageInfo sImageInfo_FALL_DOWN;
		actFrame_FALL_DOWN.nActionID = ACTION::FALL_DOWN;
		actFrame_FALL_DOWN.strActionName = "fallDOWN";
		actFrame_FALL_DOWN.nMaxFrames = 1;

		sImageInfo_FALL_DOWN.vSource = { 1536.0f, 1024.0f };
		sImageInfo_FALL_DOWN.vSize = { 192.0f, 256.0f };
		actFrame_FALL_DOWN.vecPartialImages.push_back(sImageInfo_FALL_DOWN);
		vecActionFrames.push_back(actFrame_FALL_DOWN);


	}

	olc::vf2d PlayerObject::WarpFrame(ACTION action)
	{
		bWarpFrame = !bWarpFrame;
		if (!bWarpFrame) return olc::vf2d(Properties.vfMasterScaler);

		olc::vf2d vfWarp = { 0.0f, 0.0f };

		switch (action)
		{
		case olc::PlayerObject::IDLE:
			break;
		case olc::PlayerObject::JUMP:
			break;
		case olc::PlayerObject::FALL:
			break;
		case olc::PlayerObject::DUCK:
			break;
		case olc::PlayerObject::HIT:
			break;
		case olc::PlayerObject::CLIMB:
			break;
		case olc::PlayerObject::CHEER:
			break;
		case olc::PlayerObject::BACK:
			break;
		case olc::PlayerObject::SLIDE:
			break;
		case olc::PlayerObject::INTERACT:
			break;
		case olc::PlayerObject::SWITCH:
			break;
		case olc::PlayerObject::KICK:
			break;
		case olc::PlayerObject::SIDE:
			break;
		case olc::PlayerObject::SHOVE:
			break;
		case olc::PlayerObject::SHOVE_BACK:
			break;
		case olc::PlayerObject::TALK:
			break;
		case olc::PlayerObject::ATTACK_KICK:
			break;
		case olc::PlayerObject::HANG:
			break;
		case olc::PlayerObject::HOLD:
			break;
		case olc::PlayerObject::SHOW:
			break;
		case olc::PlayerObject::BEHIND_BACK:
			vfWarp = { 0.003f, 0.003f };
			break;
		case olc::PlayerObject::RUN:
			//vfWarp = { 0.005f, 0.009f };
			break;
		case olc::PlayerObject::ATTACK:
			break;
		case olc::PlayerObject::THINK:
			break;
		case olc::PlayerObject::DOWN:
			break;
		case olc::PlayerObject::DRAG:
			break;
		case olc::PlayerObject::HURT:
			break;
		case olc::PlayerObject::WIDE:
			break;
		case olc::PlayerObject::ROPE:
			break;
		case olc::PlayerObject::WALK:
			//vfWarp = { 0.005f, 0.005f };
			break;
		case olc::PlayerObject::FALL_DOWN:
			break;
		default:
			break;
		}

		return olc::vf2d(Properties.vfMasterScaler + vfWarp);
	}





} // olc

#endif
