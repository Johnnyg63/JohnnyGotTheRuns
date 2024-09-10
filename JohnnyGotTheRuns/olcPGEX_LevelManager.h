#pragma once
#include "pch.h"


#ifdef OLC_PGEX_LEVEL_MANAGER
// TODO:
#endif

namespace olc
{
	/*
	* Manages the Loading of Level Graphics
	*/
	class LevelManager : public PGEX
	{

	public:
		LevelManager();
		LevelManager(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel);
		virtual ~LevelManager();

		// Fires just before the main OnUserCreate
		virtual void OnBeforeUserCreate() override;

		// Fires just After the main OnUserCreate
		virtual void OnAfterUserCreate() override;

		// Fires just before the main OnUserUpdate
		virtual bool OnBeforeUserUpdate(float& fElapsedTime) override;

		// Fires just After the main OnUserUpdate
		virtual void OnAfterUserUpdate(float fElapsedTime) override;

		// Loads the level, will also clear any existing level data
		void LoadLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel);

		// Deletes the level
		void ClearLevel();


		/*
		* Displays the current loaded level
		* vTileTL = Top left tile in World Space
		* vTileBR = Bottom right tile in World Space
		* decalInfo = Level Loader DecalInfo so we can display the level
		*/
		void DisplayLevel(float fElapsedTime, olc::vi2d vTileTL, olc::vi2d vTileBR);




	public:

		Map map; //TMXParser Map !


		struct DecalInfo
		{
			int16_t nLayer = 0;							// Stores layer number
			int16_t nTiledID = 0;						// Stores Tiled Map Editor ID
			olc::vf2d vfDrawLocation = { 0.0f, 0.0f };	// Stores the locatoin of where to draw
			olc::vf2d vfSourcePos = { 0.0f, 0.0f };		// Stores Location on Sprite Sheet
			olc::vf2d vfSoureSizePos = { 0.0f, 0.0f };	// Stores size of Partial Decal
		};

		DecalInfo sDecalInfo;

		/*
		* Stores data required for the Sprite Sheet objects to display correctly
		*/
		struct ImageInfo
		{
			olc::vf2d vSource = { 0.0f, 0.0f }; // Source poisition to draw drawing from, default: {0.0f, 0.0f)
			olc::vf2d vSize = { 1.0f, 1.0f };	// The size of the image to be drawn, default: Full passed in image size, edit this to when using SpriteSheets to the location of the sprite
			olc::vf2d vScale = { 1.0f, 1.0f };	// Scaling factor (Decal Only), default: {1.0, 1.0}, when AutoScale is set this value will be automactically updated
			olc::Pixel pxTint = olc::WHITE;		// Tint colour for background, set to olc::DARK_GREY for a night time effect
		};

		struct ObjectProperites
		{
			std::string strName = "LevelX";	// Object Name. Default "New Player"
			uint16_t nLevelNumber = 0;	    // Object Number, Default 0 i.e. Backupground 1 , LevelManager 2 etc

			bool bAutoScale = true;				// Automatically scales the background image to fit within the screen size

			olc::vf2d vfPosition = { 0.0f,0.0f };	// Image POS {x,y} (float), Default {0.0f,0.0f}

			std::string strSpriteSheetPath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""
			std::string strTiledMapTMXPath = "";     // Sprite path, i.e. "maps/level.tmx", Default: ""

			olc::Renderable renSpriteSheet;               // Keeps the sprImage and decImage in the one location

			std::vector<olc::Renderable> vecPlayerFrames;   // Holds the different frames for a object

			std::vector<DecalInfo> vecPartialDecalInfo;		// Stores the DecalInfo struct for tiled map graphics

			std::map<int, std::vector<DecalInfo>> mapLayerInfo; // Stores the layers of DecalInfo vectors

			olc::TileTransformedView* tv;		// Pointer to Transformed view object to make world offsetting simple

			// Conveninet constants to define tile map world
			olc::vi2d viWorldSize = { 140, 24 }; // 2048 64 cells
			olc::vi2d viTileSize = { 35, 35 };

			olc::vi2d viSpriteSheetTiles = { 28, 14 };	// Stores the total number of tiles x,y in the sprite sheet (Important!)

		};

		ObjectProperites Properties;

	private:
		bool bisLevelLoaded = false; // Use to stop execution until a level is loaded

	};

}


#ifdef OLC_PGEX_LEVEL_MANAGER
#undef OLC_PGEX_LEVEL_MANAGER

namespace olc
{


	LevelManager::LevelManager() : PGEX(true)
	{
		// Nothing to do here but to wait until we are ready for the level
		bisLevelLoaded = false;
	}

	LevelManager::LevelManager(std::string strSpriteSheetPath,
		std::string strTiledMapTMXPath,
		uint16_t nLevel) : PGEX(true)
	{
		bisLevelLoaded = false;

		Properties.strSpriteSheetPath = strSpriteSheetPath;

		Properties.strTiledMapTMXPath = strTiledMapTMXPath;

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


	LevelManager::~LevelManager()
	{

	}

	void LevelManager::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate
		if (Properties.strSpriteSheetPath == "" || Properties.strTiledMapTMXPath == "") return;

		LoadLevel(Properties.strSpriteSheetPath, Properties.strTiledMapTMXPath, Properties.nLevelNumber);

		bisLevelLoaded = true;
	}


	void LevelManager::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
		if (bisLevelLoaded)
		{

		}
	}

	bool LevelManager::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		if (bisLevelLoaded)
		{

		}

		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	void LevelManager::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
		if (bisLevelLoaded)
		{

		}

	}

	void LevelManager::LoadLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel)
	{
		Properties.renSpriteSheet.Load(strSpriteSheetPath);
		TMXParser tmxParser = TMXParser(Properties.strTiledMapTMXPath);
		map = tmxParser.GetData();

		// Lod the data into the vector for processing later
		//vecPartialDecalInfo

		Properties.vecPartialDecalInfo.clear();
		int nLayerCount = 0;

		// TODO Remove this crap
		size_t nCount = map.LayerData.size();
		nCount = map.MapData.data.size();
		nCount = map.TilesetData.data.size();

		int x = 0;
		int y = 0;
		nLayerCount = 0;
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

					float spriteX = x * 35;
					float spriteY = y * 35;


					DecalInfo sDecalInfo;
					sDecalInfo.nLayer = nLayerCount;
					sDecalInfo.nTiledID = tileId;
					sDecalInfo.vfDrawLocation = { spriteX , spriteY };
					sDecalInfo.vfSoureSizePos = { 35.0f, 35.0f };

					if (tileId > 0)
					{
						// Draw something
						int tileX = (tileId - 1) % 28;		// Number of X tiles Johnngy!!!!... number of tiles on the SpriteSheet!
						int tileY = (tileId - 1) / 28;

						float sourceX = tileX * 35;
						float sourceY = tileY * 35;

						//pge->DrawPartialDecal({ spriteX, spriteY }, { 35.0f, 35.0f }, Properties.renSpriteSheet.Decal(), { sourceX, sourceY }, { 35.0f, 35.0f });
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


		bisLevelLoaded = true;
	}

	void LevelManager::ClearLevel()
	{
		delete Properties.renSpriteSheet.Decal();
		delete Properties.renSpriteSheet.Sprite();
	}

	void LevelManager::DisplayLevel(float fElapsedTime, olc::vi2d vTileTL, olc::vi2d vTileBR)
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


			} // End for Loop vtiles

	}


} // olc

#endif

