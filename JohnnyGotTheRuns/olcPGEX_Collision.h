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
		void UpdateCollisions(olc::vf2d* vfPositionPos, olc::vf2d vfCenterPos, float fRadius, float fElapsedTime);


	public:

		struct ObjectProperites
		{
			bool bIsEnabled = false;			// Important, we only set this to true when we need to use this class
			std::string strName = "Collision Manager";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , Collision 2 etc

			olc::TileTransformedView* ptrTileTransFormedView;		// Pointer to Transformed view object to make world offsetting simple
			olc::vf2d* ptrvTrackedPoint;			// Pointer to the point that represents the player, it is "tracked" by the camera

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


#ifdef OLC_PGEX_COLLISION
#undef OLC_PGEX_COLLISION

namespace olc
{


	Collision::Collision() : PGEX(true)
	{

	}


	Collision::~Collision()
	{


	}


	void Collision::OnBeforeUserCreate()
	{

	}


	void Collision::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	bool Collision::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		if (Properties.bIsEnabled)
		{
			// We manage collisions before we draw

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
					UpdateCollisions(&playerObject->Properties.vfPosition, playerObject->collCircle.vfCenterPos, playerObject->collCircle.fRadius, fElapsedTime);

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
					UpdateCollisions(Properties.ptrvTrackedPoint, playerObject->collCircle.vfCenterPos, playerObject->collCircle.fRadius, fElapsedTime);

					break;
				}

				default:
					break;
				}
			}
		}


		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void Collision::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
		if (Properties.bIsEnabled)
		{

		}
	}

	// See Step 3: Rename to your Class name
	void Collision::DrawDecal()
	{

	}

	void Collision::UpdateCollisions(olc::vf2d* vfPositionPos, olc::vf2d vfCenterPos, float fRadius, float fElapsedTime)
	{
		olc::vi2d vTileTL = Properties.ptrTileTransFormedView->GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = Properties.ptrTileTransFormedView->GetBottomRightTile().min(Properties.viWorldSize);

		olc::LevelManager::DecalInfo decalInfo;
		olc::vi2d vTile;
		int32_t idx = 0;
		int16_t nLayer = 0;

		using namespace olc::utils::geom2d;

		olc::vf2d vfDirection = { 0.0f, 0.0f };
		olc::vf2d vfClosest = { 0.0f, 0.0f };
		olc::vf2d vfDistance = { 0.0f, 0.0f };
		float fDistance = 0.0f;
		float fOverlap = 0.0f;
		
		rect<float> worldTile;
		worldTile.pos.x = 0.0f;
		worldTile.pos.y = 0.0f;
		worldTile.size = { 35.0f, 35.0f };

		// Polygon stuff
		olc::vf2d vfPoints[2];
		olc::vf2d vfNewClosest = { 0.0f, 0.0f };
		bool bIsFirstClosest = true;

		bool bOverLaps = false; // Is set when a circle overlaps a Rect/Triangle

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
					bOverLaps = false;	// Reset our overlap
					bIsFirstClosest = true;
					decalInfo = layer.second[idx];	// We only care about the data (layer.data)

					if (decalInfo.nTiledID == 0) continue; // If the tile does nothing just move on

					if (decalInfo.bHasCollision)
					{
						
						// Check for collision here
						worldTile.pos = Properties.ptrTileTransFormedView->WorldToScreen(vTile);

						for (auto& tileObject : decalInfo.sCollisionTile.vecTileObjects)
						{
							switch (tileObject.eCollision)
							{

							case LevelManager::Collision::RECT:
							{
								worldTile.pos += tileObject.vfPosition;
								worldTile.size = tileObject.vfSize;
								bOverLaps = overlaps(circle<float>{vfCenterPos, fRadius}, worldTile);
								if (bOverLaps)
								{
									// Get the closest point between a circle and a rectangle
									vfClosest = closest(worldTile, circle<float>{vfCenterPos, fRadius});
								}

								break;
							}
							case LevelManager::Collision::ELLIPSE:
							{
								break;
							}
							case LevelManager::Collision::POLYGON:
							{
								// Ok by right we should be looping through the trianges for our collision
								// but in our case, the polygon points, angles etc have all been worked out by Tiled Map editor
								// therefore we can just loop through the outer lines of out polygon and use these to manage collision
								// It should have the excat same affect :)
								//worldTile.pos += tileObject.vfPosition;


								auto& vPoints = decalInfo.sCollisionTile.sCollisionType.vecPoints;

								for (int i = 1; i < vPoints.size(); i++)
								{
									vfPoints[0] = worldTile.pos + (vPoints[i - 1] + tileObject.vfPosition);
									vfPoints[1] = worldTile.pos + (vPoints[i] + tileObject.vfPosition);
									bOverLaps = overlaps(line<float>{vfPoints[0], vfPoints[1]}, circle<float>{ vfCenterPos, fRadius });

									// If we over lap lets find the closet first and move back from there
									if (bOverLaps)
									{
										vfNewClosest = closest(line<float>{vfPoints[0], vfPoints[1]},
											circle<float>{ vfCenterPos, fRadius });
										
										if (bIsFirstClosest)
										{
											vfClosest = vfNewClosest;
											bIsFirstClosest = false;
										}

										if (vfClosest > vfNewClosest)
										{
											vfClosest = vfNewClosest;
										}

									}
									
								}
								
								break;
							}
							case LevelManager::Collision::POINT:
							{
								break;
							}

							default:
								break;
							}
						}



						// Lets update our position if we are over lapping
						if (bOverLaps)
						{
							vfDistance = vfCenterPos - vfClosest;

							fDistance = std::sqrt(vfDistance.x * vfDistance.x + vfDistance.y * vfDistance.y);
							fOverlap = fRadius - fDistance;

							if (fDistance != 0)
							{
								vfCenterPos += (vfDistance / fDistance) * fOverlap;
								vfDirection += (vfDistance / fDistance) * fOverlap;
							}
							else
							{
								// Handle the case where the circle's center is exactly on the rectangle's edge
								if (vfDistance.x == 0) {
									vfCenterPos.y += (vfCenterPos.y > worldTile.pos.y + worldTile.size.y / 2) ? fOverlap : -fOverlap;
									vfDirection.y += (vfCenterPos.y > worldTile.pos.y + worldTile.size.y / 2) ? fOverlap : -fOverlap;
								}
								else {
									vfCenterPos.x += (vfCenterPos.x > worldTile.pos.x + worldTile.size.x / 2) ? fOverlap : -fOverlap;
									vfDirection.x += (vfCenterPos.x > worldTile.pos.x + worldTile.size.x / 2) ? fOverlap : -fOverlap;
								}
							}

							/*
							* Note we add *a to declare we want to update the value
							* Javidx9 has a great video explaining pointers here : https://www.youtube.com/watch?v=iChalAKXffs)
							*/

							*vfPositionPos += vfDirection * fElapsedTime;
						}

					}

					nLayer++;

				}


			}


	}







} // olc

#endif
