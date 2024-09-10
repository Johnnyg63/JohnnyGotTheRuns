#pragma once

#include "pch.h"

/*
*  Displays the level, that it in a nutshell
*/


class GameLevel : public olc::PixelGameEngine
{
public:
	GameLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel);
	virtual ~GameLevel();

	/*
	* Displays the current loaded level
	* vTileTL = Top left tile in World Space
	* vTileBR = Bottom right tile in World Space
	* decalInfo = Level Loader DecalInfo so we can display the level
	*/
	void DisplayLevel(float fElapsedTime, olc::vi2d vTileTL, olc::vi2d vTileBR);

	/*
	*  Loads the Level Graphics
	*/
	void LoadLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel);

	/*
	* Clears the level from memory
	*/
	void ClearLevel();

	Map map; //TMXParser Map !


	struct DecalInfo
	{
		int16_t nLayer = 0;							// Stores layer number
		int16_t nTiledID = 0;						// Stores Tiled Map Editor ID
		olc::vf2d vfDrawLocation = { 0.0f, 0.0f };	// Stores the locatoin of where to draw
		olc::vf2d vfSourcePos = { 0.0f, 0.0f };		// Stores Location on Sprite Sheet
		olc::vf2d vfSoureSizePos = { 0.0f, 0.0f };	// Stores size of Partial Decal
	};

	struct ObjectProperites
	{
		std::string strName = "Level??";	// Object Name. Default "New Player"
		uint16_t nLevelNumber = 0;			// Level Number, Default 0 

		olc::TileTransformedView* tv;		// Pointer to Transformed view object to make world offsetting simple

		// Conveninet constants to define tile map world
		olc::vi2d viWorldSize = { 140, 24 }; // 2048 64 cells
		olc::vi2d viTileSize = { 35, 35 };

		olc::vi2d viSpriteSheetTiles = { 28, 14 };	// Stores the total number of tiles x,y in the sprite sheet (Important!)

		std::string strSpriteSheetPath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""
		std::string strTiledMapTMXPath = "";     // Sprite path, i.e. "maps/level.tmx", Default: ""

		olc::Renderable renSpriteSheet;						// Keeps the sprImage and decImage in the one location

		std::map<int, std::vector<DecalInfo>> mapLayerInfo; // Stores the layers of DecalInfo vectors

		std::vector<DecalInfo> vecPartialDecalInfo;			// Stores the DecalInfo struct for tiled map graphics

	};

	ObjectProperites Properties;

private:
	bool bisLevelLoaded = false;
};

GameLevel::GameLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel)
{
	bisLevelLoaded = false;

	Properties.strSpriteSheetPath = strSpriteSheetPath;

	Properties.strTiledMapTMXPath = strTiledMapTMXPath;

	Properties.nLevelNumber = nLevel;

	Properties.strName = "Level ID: " + std::to_string(nLevel);

#if defined (_MSC_VER)
	// Windows stuff

	if (strSpriteSheetPath.rfind("./", 0) != 0) {
		Properties.strSpriteSheetPath = "./" + strSpriteSheetPath;
	}

	if (strTiledMapTMXPath.rfind("./", 0) != 0) {
		Properties.strTiledMapTMXPath = "./" + strTiledMapTMXPath;
	}
#endif

}

GameLevel::~GameLevel()
{
	Properties.vecPartialDecalInfo.clear();
	Properties.mapLayerInfo.clear();
}

void GameLevel::DisplayLevel(float fElapsedTime, olc::vi2d vTileTL, olc::vi2d vTileBR)
{
	// Displays the level
	olc::vi2d vTile;
	int32_t idx = 0;
	int16_t nLayer = 0;
	DecalInfo decalInfo;

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

	// Then looping through them and drawing them
	for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
		for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
		{
			idx = vTile.y * Properties.viWorldSize.x + vTile.x;

			//tv.DrawRectDecal({ (float)vTile.x, (float)vTile.y }, { 1.0f, 1.0f }, olc::BLACK);

			for (auto& layer : Properties.mapLayerInfo)
			{
				decalInfo = layer.second[idx];

				if (decalInfo.nTiledID == 0) continue; // If the tile does nothing just move on

				switch (decalInfo.nLayer)
				{
				case 0:
				{
					// This is our collision layer
					//tv.DrawRectDecal({ (float)vTile.x, (float)vTile.y }, { 1.0f, 1.0f }, olc::RED);
					break;
				}
				case 1:
				{
					// this is our Ladder layer
					Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
						Properties.renSpriteSheet.Decal(),
						decalInfo.vfSourcePos,
						decalInfo.vfSoureSizePos);
					break;
				}
				default:
					// this is our drawing layer
					Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
						Properties.renSpriteSheet.Decal(),
						decalInfo.vfSourcePos,
						decalInfo.vfSoureSizePos);
					break;
				}

				nLayer++;

			}


		}



}

void GameLevel::LoadLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel)
{

#if defined (_MSC_VER)
	// Windows stuff

	if (strSpriteSheetPath.rfind("./", 0) != 0) {
		Properties.strSpriteSheetPath = "./" + strSpriteSheetPath;
	}

	if (strTiledMapTMXPath.rfind("./", 0) != 0) {
		Properties.strTiledMapTMXPath = "./" + strTiledMapTMXPath;
	}
#endif

	// Clear existing data
	ClearLevel();

	// Loads the level
	Properties.renSpriteSheet.Load(strSpriteSheetPath);
	Properties.nLevelNumber = nLevel;
	TMXParser tmxParser = TMXParser(Properties.strTiledMapTMXPath);
	map = tmxParser.GetData();

	Properties.vecPartialDecalInfo.clear();
	int nLayerCount = 0;
	int x = 0;
	int y = 0;


	for (auto& layer : map.LayerData)
	{
		auto vecPartialDecalInfo = std::vector<DecalInfo>();
		auto rowYtiles = layer.tiles;
		for (auto& tiles : rowYtiles)
		{
			x = 0;
			for (auto& tile : tiles)
			{
				int tileId = tile;

				float spriteX = x * Properties.viTileSize.x; // 35.0f
				float spriteY = y * Properties.viTileSize.y; 

				DecalInfo sDecalInfo;
				sDecalInfo.nLayer = nLayerCount;
				sDecalInfo.nTiledID = tileId;
				sDecalInfo.vfDrawLocation = { spriteX , spriteY };
				sDecalInfo.vfSoureSizePos = Properties.viTileSize;

				if (tileId > 0)
				{
					// Draw something
					int tileX = (tileId - 1) % Properties.viSpriteSheetTiles.x;		// Number of X tiles Johnngy!!!!... number of tiles on the SpriteSheet!
					int tileY = (tileId - 1) / Properties.viSpriteSheetTiles.x;

					float sourceX = tileX * Properties.viTileSize.x;
					float sourceY = tileY * Properties.viTileSize.y;
					sDecalInfo.vfSourcePos = { sourceX , sourceY };

				}

				vecPartialDecalInfo.push_back(sDecalInfo);

				x++;
			}

			y++;
		}


		Properties.mapLayerInfo.insert({ nLayerCount, vecPartialDecalInfo });
		nLayerCount++;

	}

	Properties.strName = "Level ID: " + std::to_string(nLevel);
	bisLevelLoaded = true;
}

 void GameLevel::ClearLevel()
{
	 Properties.vecPartialDecalInfo.clear();
	 Properties.mapLayerInfo.clear();
}
