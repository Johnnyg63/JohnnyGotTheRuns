#pragma once
#include "pch.h"



#ifdef OLC_PGEX_COLLISION

#endif

namespace olc
{
	/*
	* Manages Collision
	*/
	class Collision : public PGEX
	{

	public:
		Collision();
		virtual ~Collision();

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

		// Add your own public methods here
		void UpdateCollisions(olc::vf2d vfCenterPos, float fRadius, float fElapsedTime);


	public:


		struct ObjectProperites
		{
			std::string strName = "Collision Manager";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , Collision 2 etc

			olc::TileTransformedView* ptrTileTransFormedView;		// Pointer to Transformed view object to make world offsetting simple
			olc::vf2d* ptrvTrackedPoint;			// Pointer to the point that represents the player, it is "tracked" by the camera

			std::map<int, std::vector<olc::LevelManager::DecalInfo>>* ptrmapLayerInfo; // Pointer layers of DecalInfo vectors

			// Conveninet constants to define tile map world
			olc::vi2d viWorldSize = { 140, 24 }; // 2048 64 cells
			olc::vi2d viTileSize = { 35, 35 };

			olc::vi2d viSpriteSheetTiles = { 28, 14 };	// Stores the total number of tiles x,y in the sprite sheet (Important!)

			

		};

		ObjectProperites Properties;

	private:
		bool bisSpriteSheet = false; // Flag for sprite sheet or 1 image

		bool bChangeGround = false; // Flag used to change the background

	};

}


#ifdef OLC_PGEX_COLLISION
#undef OLC_PGEX_COLLISION

namespace olc
{

	// See Step 3: Rename to your Class name
	Collision::Collision() : PGEX(true)
	{
		
	}

	// See Step 3: Rename to your Class name
	Collision::~Collision()
	{

		
	}

	// See Step 3: Rename to your Class name
	void Collision::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

	

	}

	// See Step 3: Rename to your Class name
	void Collision::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	// See Step 3: Rename to your Class name
	bool Collision::OnBeforeUserUpdate(float& fElapsedTime)
	{
		

		// Fires just before the main OnUserUpdate
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void Collision::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
	}

	// See Step 3: Rename to your Class name
	void Collision::DrawDecal()
	{
		
	}

	void Collision::UpdateCollisions(olc::vf2d vfCenterPos, float fRadius, float fElapsedTime)
	{
		olc::vi2d vTileTL = Properties.ptrTileTransFormedView->GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = Properties.ptrTileTransFormedView->GetBottomRightTile().min(Properties.viWorldSize);

		olc::LevelManager::DecalInfo decalInfo;
		olc::vi2d vTile;
		int32_t idx = 0;
		int16_t nLayer = 0;

		using namespace olc::utils::geom2d;

		olc::vf2d vfDirection = { 0.0f, 0.0f };
		float fClosestX = 0.0f;
		float fClosestY = 0.0f;
		float fDistanceX = 0.0f;
		float fDistanceY = 0.0f;
		float fDistance = 0.0f;
		float fOverlap = 0.0f;
		//olc::vf2d worldTile = { 0.0f, 0.0f };

		rect<float> worldTile;
		worldTile.pos.x = 0.0f;
		worldTile.pos.y = 0.0f;
		worldTile.size = { 35.0f, 35.0f };

		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				idx = vTile.y * Properties.viWorldSize.x + vTile.x;

			
				/*
				* Note we add a* to declare we want to access the value
				* Javidx9 has a great video explaining pointers here : https://www.youtube.com/watch?v=iChalAKXffs)
				*/
				for (auto& layer : *Properties.ptrmapLayerInfo)
				{
					decalInfo = layer.second[idx];

					if (decalInfo.nTiledID == 0) continue; // If the tile does nothing just move on

					switch (decalInfo.nLayerID)
					{
					case 1:
					{
						// This is our collision layer
						//tv.DrawRectDecal({ (float)vTile.x, (float)vTile.y }, { 1.0f, 1.0f }, olc::RED);

						// Check for collision here
						worldTile.pos = Properties.ptrTileTransFormedView->WorldToScreen(vTile);

						bool bResult = overlaps(
							circle<float>{vfCenterPos, fRadius},
							worldTile);

						if (bResult)
						{
							fClosestX = std::clamp(vfCenterPos.x, worldTile.pos.x, worldTile.pos.x + worldTile.size.x);
							fClosestY = std::clamp(vfCenterPos.y, worldTile.pos.y, worldTile.pos.y + worldTile.size.y);

							fDistanceX = vfCenterPos.x - fClosestX;
							fDistanceY = vfCenterPos.y - fClosestY;

							fDistance = std::sqrt(fDistanceX * fDistanceX + fDistanceY * fDistanceY);
							fOverlap = fRadius - fDistance;

							if (fDistance != 0) {
								vfCenterPos.x += (fDistanceX / fDistance) * fOverlap;
								vfCenterPos.y += (fDistanceY / fDistance) * fOverlap;
								vfDirection.x += (fDistanceX / fDistance) * fOverlap;
								vfDirection.y += (fDistanceY / fDistance) * fOverlap;
							}
							else {
								// Handle the case where the circle's center is exactly on the rectangle's edge
								if (fDistanceX == 0) {
									vfCenterPos.y += (vfCenterPos.y > worldTile.pos.y + worldTile.size.y / 2) ? fOverlap : -fOverlap;
									vfDirection.y += (vfCenterPos.y > worldTile.pos.y + worldTile.size.y / 2) ? fOverlap : -fOverlap;
								}
								else {
									vfCenterPos.x += (vfCenterPos.x > worldTile.pos.x + worldTile.size.x / 2) ? fOverlap : -fOverlap;
									vfDirection.x += (vfCenterPos.x > worldTile.pos.x + worldTile.size.x / 2) ? fOverlap : -fOverlap;
								}
							}

							/*
							* Note we add a* to declare we want to update the value
							* Javidx9 has a great video explaining pointers here : https://www.youtube.com/watch?v=iChalAKXffs)
							*/ 
							*Properties.ptrvTrackedPoint += vfDirection * fElapsedTime;

							//pPlayer->Properties.vfPosition = tv.WorldToScreen((vTrackedPoint - olc::vf2d(1.5f, 1.5f)));

						}

						break;
					}
					default:
						break;
					}

					nLayer++;

				}


			}


	}



	



} // olc

#endif
