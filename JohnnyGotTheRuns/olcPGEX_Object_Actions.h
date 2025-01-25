#pragma once
#pragma once
#include "pch.h"



#ifdef OLC_PGEX_OBJECT_ACTIONS

#endif

namespace olc
{
	/*
	* Manages Object Actions, such as player movement
	*/
	class ObjectActions : public PGEX
	{

	public:
		ObjectActions();
		virtual ~ObjectActions();

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

		/*
		* Manages the Keyboard and mouse inputs
		* TODO: Add support for touch screen
		*/
		void ManageKeyInputs(std::shared_ptr<olc::PlayerObject> pPlayerObject, float fElapsedTime);

		/*
		*  Updates the player position,
		*  Note: here we just move the player freely, the Collision Controller will correct the position on the next loop
		*/
		void UpdateObjectPosition(std::shared_ptr<olc::PlayerObject> pPlayerObject, olc::vf2d vfDirection, float fElapsedTime);

	public:

		struct ObjectProperites
		{
			bool bIsEnabled = false;			// Important, we only set this to true when we need to use this class
			std::string strName = "ObjectActions Manager";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , ObjectActions 2 etc

			olc::TileTransformedView* ptrTileTransFormedView;		// Pointer to Transformed view object to make world offsetting simple
			olc::vf2d* ptrvTrackedPoint;			// Pointer to the point that represents the player, it is "tracked" by the camera

			olc::utils::Camera2D* ptrCamera;

			std::map<int, std::vector<olc::LevelManager::DecalInfo>>* ptrmapLayerInfo; // Pointer layers of DecalInfo vectors

			// Conveninet constants to define tile map world
			olc::vi2d viWorldSize = { 140, 24 }; // 2048 64 cells
			olc::vi2d viTileSize = { 35, 35 };

			olc::vi2d viSpriteSheetTiles = { 28, 14 };	// Stores the total number of tiles x,y in the sprite sheet (Important!)

			std::vector<std::shared_ptr<olc::PlayerObject>>* vecPlayerObjects;

		};

		ObjectProperites Properties;

	private:
		bool bisSpriteSheet = false; // Flag for sprite sheet or 1 image

		bool bChangeGround = false; // Flag used to change the background

	};

}


#ifdef OLC_PGEX_OBJECT_ACTIONS
#undef OLC_PGEX_OBJECT_ACTIONS

namespace olc
{


	ObjectActions::ObjectActions() : PGEX(true)
	{

	}


	ObjectActions::~ObjectActions()
	{


	}


	void ObjectActions::OnBeforeUserCreate()
	{

	}


	void ObjectActions::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	bool ObjectActions::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void ObjectActions::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
		if (Properties.bIsEnabled)
		{
			// We manage ObjectActionss before we draw
			for (auto& playerObject : *Properties.vecPlayerObjects)
			{
				switch (playerObject->Properties.eObjectType)
				{
				case PlayerObject::OBJECT_TYPE::ENEMY:
				{
					// TODO: 
					break;
				}
				case PlayerObject::OBJECT_TYPE::GAME_CHAR:
				{
					// TODO: Add Gravity
					playerObject->UpdateAction(PlayerObject::ACTION::CHEER);

					break;
				}
				case PlayerObject::OBJECT_TYPE::NONE:
				{

					break;
				}
				case PlayerObject::OBJECT_TYPE::OTHER_PLAYER:
				{

					break;
				}
				case PlayerObject::OBJECT_TYPE::PLAYER:
				{
					// For the player we update the vTrackpoint as this controlls the player position
					ManageKeyInputs(playerObject, fElapsedTime);
					break;
				}

				default:
					break;
				}
			}


		}
	}

	// See Step 3: Rename to your Class name
	void ObjectActions::DrawDecal()
	{

	}

	void ObjectActions::ManageKeyInputs(std::shared_ptr<olc::PlayerObject> pPlayerObject, float fElapsedTime)
	{
		olc::vf2d vfDirection = { 0.0f, 0.0f };
		if (pPlayerObject->Properties.bIsGravityEnabled == true)
		{
			vfDirection = pPlayerObject->Properties.vfGravityDirection;
		}
		else
		{
			vfDirection = { 0.0f, 0.0f };
		}
		
		pPlayerObject->Properties.vfVelocity = vfDirection;
		if (pPlayerObject->Properties.bIsOnLadder == true)
		{
			pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::CLIMB_PAUSE);
		}
		else
		{
			pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::BEHIND_BACK);
		}
		

		if (pge->GetKey(olc::Key::UP).bHeld && pPlayerObject->Properties.bIsOnLadder == true)
		{
			pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::CLIMB);
			vfDirection = { 0, -1 }; //up

		}

		if (pge->GetKey(olc::Key::DOWN).bHeld)
		{

			
			if (pPlayerObject->Properties.bIsOnLadder == false)
			{
				pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::DUCK);
			}
			else
			{
				pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::CLIMB);
			}
			vfDirection = { 0, +1. }; // down


		}

		if (pge->GetKey(olc::Key::LEFT).bHeld)
		{
			pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::WALK);//pPlayer->Properties.vfPosition.x -= pPlayer->Properties.vfVelocity.x * fElapsedTime;
			vfDirection = { -1, 0 }; // left

		}

		if (pge->GetKey(olc::Key::RIGHT).bHeld)
		{
			pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::WALK);//pPlayer->Properties.vfPosition.x += pPlayer->Properties.vfVelocity.x * fElapsedTime;
			vfDirection = { +1, 0 }; // right

		}

		if (pge->GetKey(olc::Key::SPACE).bHeld)
		{
			pPlayerObject->UpdateAction(olc::PlayerObject::ACTION::JUMP);
			//pPlayer->Properties.vfPosition.y -= pPlayer->Properties.vfVelocity.y * fElapsedTime * 5.0f;
			// When we are jumping we only care about the up direction therefore we only subtract to the Y 
			// more than the down force, 
			vfDirection.y += -2.0f; // Jump

		}

		UpdateObjectPosition(pPlayerObject, vfDirection, fElapsedTime);

	}

	void ObjectActions::UpdateObjectPosition(std::shared_ptr<olc::PlayerObject> pPlayerObject, olc::vf2d vfDirection, float fElapsedTime)
	{

		// TODO: Move to new location
		pPlayerObject->Properties.vfVelocity += vfDirection; // Update player direction

		// Now we update our trackpoint in 
		*Properties.ptrvTrackedPoint += pPlayerObject->Properties.vfVelocity * 4.0f * fElapsedTime;

		// Edge case when the player gets stuck
		if (std::isnan(Properties.ptrvTrackedPoint->x) || std::isnan(Properties.ptrvTrackedPoint->y))
		{
			//Player is stuck lets reset
			// TODO: Add code for when the player is stuck
		}

		Properties.ptrCamera->Update(fElapsedTime);

		// Set the transformed view to that required by the camera
		Properties.ptrTileTransFormedView->SetWorldOffset(Properties.ptrCamera->GetViewPosition());

		// Where will object be worst case ?
		pPlayerObject->Properties.vfPotentialPosition = pPlayerObject->Properties.vfPosition + pPlayerObject->Properties.vfVelocity * 4.0f * fElapsedTime;


		// Some borders TODO: Change to using olcUTIL_Geometry2D.h
		if (Properties.ptrvTrackedPoint->x < 0.00f)
		{
			Properties.ptrvTrackedPoint->x = 0.00f;
			pPlayerObject->Properties.vfPosition.x = 0.0f;
		}
		if (Properties.ptrvTrackedPoint->x > Properties.viWorldSize.x)
		{
			Properties.ptrvTrackedPoint->x = float(Properties.viWorldSize.x);
			pPlayerObject->Properties.vfPosition.x = float(Properties.viWorldSize.x);
		}

		if (Properties.ptrvTrackedPoint->y < 0.01f)
		{
			Properties.ptrvTrackedPoint->y = 0.01f;
			pPlayerObject->Properties.vfPosition.y = 0.01f;
		}

		if (Properties.ptrvTrackedPoint->y > Properties.viWorldSize.y)
		{
			Properties.ptrvTrackedPoint->y = float(Properties.viWorldSize.y);
			pPlayerObject->Properties.vfPosition.y = float(Properties.viWorldSize.y);
		}

		olc::vf2d newPos = *Properties.ptrvTrackedPoint - olc::vf2d(1.5f, 1.5f);

		pPlayerObject->Properties.vfPosition = Properties.ptrTileTransFormedView->WorldToScreen(newPos);


	}









} // olc

#endif

