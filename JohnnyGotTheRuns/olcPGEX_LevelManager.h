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

		//<map version="1.10" tiledversion="1.11.0" orientation="orthogonal" renderorder="right-down" width="140" height="24" 
		//		tilewidth="35" tileheight="35" infinite="0" nextlayerid="5" nextobjectid="1"

		struct MapInfo
		{
			std::string strVersion = "";		// Version
			std::string strTiledVersion = "";	// Tiled Map Version
			std::string strOrientation = "";	// Orientation
			std::string strRenderorder = "";	// Order and direction to render (Keep is simple folks use right-down
			int32_t nWidth = 0;					// Layer Width
			int32_t nHeight = 0;				// Layer Height
			int32_t nTileWidth = 0;				// Layer Width
			int32_t nTileHeight = 0;			// Layer Height
			bool bIsInfinite = false;			// Is the layer infinite
			int32_t nNextLayerID = 0;			// The next layer id
			int32_t nNextObjectID = 0;			// The next object id


		};

		MapInfo sMapInfo;


		//<layer id="1" name="L0" class="collision" width="140" height="24" visable="0" locked="1"

		struct DecalInfo
		{
			bool bIsVisable = true;						// Is layer is visable
			bool bIsLocked = true;						// Is layer is locked
			int16_t nLayerID = 0;						// Layer id number
			std::string strName = "";					// Layer name
			std::string strClass = "";					// Layer Class name
			int32_t nWidth = 0;							// Layer Width
			int32_t nHeight = 0;						// Layer Height
			int16_t nTiledID = 0;						// Tiled Map Editor ID
			olc::vf2d vfDrawLocation = { 0.0f, 0.0f };	// Locatoin of where to draw
			olc::vf2d vfSourcePos = { 0.0f, 0.0f };		// Location on Sprite Sheet
			olc::vf2d vfSoureSizePos = { 0.0f, 0.0f };	// Size of Partial Decal
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
		// Lets load and parse our data
		Properties.renSpriteSheet.Load(strSpriteSheetPath);
		TMXParser tmxParser = TMXParser(Properties.strTiledMapTMXPath);
		map = tmxParser.GetData();

		//<map version="1.10" tiledversion="1.11.0" orientation="orthogonal" renderorder="right-down" width="140" height="24" 
		//		tilewidth="35" tileheight="35" infinite="0" nextlayerid="5" nextobjectid="1"

		for (auto& mapInfo : map.MapData.data)
		{
			if (mapInfo.first == "version") sMapInfo.strVersion = mapInfo.second;
			if (mapInfo.first == "tiledversion") sMapInfo.strTiledVersion = mapInfo.second;
			if (mapInfo.first == "orientation") sMapInfo.strOrientation = mapInfo.second;
			if (mapInfo.first == "renderorder") sMapInfo.strRenderorder = mapInfo.second;

			if (mapInfo.first == "width") sMapInfo.nWidth = std::stoi(mapInfo.second);
			if (mapInfo.first == "height") sMapInfo.nHeight = std::stoi(mapInfo.second);
			if (mapInfo.first == "tilewidth") sMapInfo.nTileWidth = std::stoi(mapInfo.second);
			if (mapInfo.first == "tileheight") sMapInfo.nTileHeight = std::stoi(mapInfo.second);
			if (mapInfo.first == "nextlayerid") sMapInfo.nNextLayerID = std::stoi(mapInfo.second);
			if (mapInfo.first == "nextobjectid") sMapInfo.nNextObjectID = std::stoi(mapInfo.second);

			if (mapInfo.first == "infinite") sMapInfo.bIsInfinite = (std::stoi(mapInfo.second) > 0) ? true : false;

		}

		// lets clear things up
		Properties.vecPartialDecalInfo.clear();
		int16_t nLayerCount = 0;
		int32_t x = 0;
		int32_t y = 0;

		// Important this is needed to ensure the DrawPartialDecal correctly finds the location with the spritesheet
		int16_t nSpriteSheetTileCount = Properties.renSpriteSheet.Sprite()->width / sMapInfo.nTileWidth;

		for (auto& layer : map.LayerData)
		{
			auto vecPartialDecalInfo = std::vector<DecalInfo>();
			auto rowYtiles = layer.tiles;
			auto layerTags = layer.tag.data;

			DecalInfo sDecalInfo;

			// // id="1" name="L0" class="collision" width="140" height="24" visable="0" locked="1"
			for (auto& tag : layerTags)
			{
				if (tag.first == "id") sDecalInfo.nLayerID = std::stoi(tag.second);
				if (tag.first == "name") sDecalInfo.strName = tag.second;
				if (tag.first == "class") sDecalInfo.strName = tag.second;
				if (tag.first == "width") sDecalInfo.nWidth = std::stoi(tag.second);
				if (tag.first == "height") sDecalInfo.nHeight = std::stoi(tag.second);
				if (tag.first == "visable") sDecalInfo.bIsVisable = (std::stoi(tag.second) > 0)? true : false;
				if (tag.first == "locked") sDecalInfo.bIsVisable = (std::stoi(tag.second) > 0) ? true : false;

			}


			for (auto& tiles : rowYtiles)
			{
				x = 0;
				for (auto& tile : tiles)
				{

					int tileId = tile;

					float spriteX = x * sMapInfo.nTileWidth;
					float spriteY = y * sMapInfo.nTileHeight;

					sDecalInfo.nTiledID = tileId;
					sDecalInfo.vfDrawLocation = { spriteX , spriteY };
					sDecalInfo.vfSoureSizePos = { (float)sMapInfo.nTileWidth, (float)sMapInfo.nTileHeight };

					if (tileId > 0)
					{
						// Draw something
						int tileX = (tileId - 1) % nSpriteSheetTileCount;		// Number of X tiles Johnngy!!!!... number of tiles on the SpriteSheet!
						int tileY = (tileId - 1) / nSpriteSheetTileCount;

						float sourceX = tileX * sMapInfo.nTileWidth;
						float sourceY = tileY * sMapInfo.nTileHeight;

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

					switch (decalInfo.nLayerID)
					{
					case 1:
					{
						// This is our collision Layer, nothing to do just move on
						break;
					}
					case 2:
					{
						// this is our Ladder layer
						Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
							Properties.renSpriteSheet.Decal(),
							decalInfo.vfSourcePos,
							decalInfo.vfSoureSizePos);
						break;
					}
					case 3:
					{
						// this is our drawing layer
						Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
							Properties.renSpriteSheet.Decal(),
							decalInfo.vfSourcePos,
							decalInfo.vfSoureSizePos);
						break;
					}
					default:
						break;
					}

				}


			} // End for Loop vtiles

	}


} // olc

#endif

