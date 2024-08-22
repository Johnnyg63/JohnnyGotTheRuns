#pragma once
#include "pch.h"


#ifdef OLC_PGEX_LEVEL_LOADER
// TODO:
#endif

namespace olc
{
	/*
	* Manages the Loading of Level Graphics
	*/
	class LevelLoader : public PGEX
	{

	public:
		LevelLoader();
		LevelLoader(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel);
		virtual ~LevelLoader();

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

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created decal
		void DrawLevel();

		// Deletes the level
		void ClearLevel();
		// Add your own public methods here


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
			uint16_t nLevelNumber = 0;	    // Object Number, Default 0 i.e. Backupground 1 , LevelLoader 2 etc

			bool bAutoScale = true;				// Automatically scales the background image to fit within the screen size

			olc::vf2d vfPosition = { 0.0f,0.0f };	// Image POS {x,y} (float), Default {0.0f,0.0f}

			std::string strSpriteSheetPath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""
			std::string strTiledMapTMXPath = "";     // Sprite path, i.e. "maps/level.tmx", Default: ""

			olc::Renderable renSpriteSheet;               // Keeps the sprImage and decImage in the one location

			std::vector<olc::Renderable> vecPlayerFrames;   // Holds the different frames for a object

			/*
			* Holds the vSource and vSize of the image
			* Edit this value to the location of the Sprite when using a Sprite sheet
			*/
			ImageInfo sImageInfo;

			/*
			*
			* Stores the location of the partial image from the sprSpriteSheet
			* ImageInfo.vSource {x,y} of the top left of the partial image to draw
			* ImageInfo.vSize {w, h} size of the partial image to be drawn
			*
			*/
			std::vector<ImageInfo> vecPartialImages;

			std::vector<DecalInfo> vecPartialDecalInfo;		// Stores the DecalInfo struct for tiled map graphics

			std::map<int, std::vector<DecalInfo>> mapLayerInfo; // Stores the layers of DecalInfo vectors

		};

		ObjectProperites Properties;

	private:
		bool bisLevelLoaded = false; // Use to stop execution until a level is loaded

	};

}


#ifdef OLC_PGEX_LEVEL_LOADER
#undef OLC_PGEX_LEVEL_LOADER

namespace olc
{


	LevelLoader::LevelLoader() : PGEX(true)
	{
		// Nothing to do here but to wait until we are ready for the level
		bisLevelLoaded = false;
	}

	LevelLoader::LevelLoader(std::string strSpriteSheetPath,
		std::string strTiledMapTMXPath,
		uint16_t nLevel) : PGEX(true)
	{
		bisLevelLoaded = false;

#if defined (_MSC_VER)
		// Windows stuff

		if (strSpriteSheetPath.rfind("./", 0) != 0) {
			Properties.strSpriteSheetPath = "./" + strSpriteSheetPath;
		}

		if (strTiledMapTMXPath.rfind("./", 0) != 0) {
			Properties.strTiledMapTMXPath = "./" + strTiledMapTMXPath;
		}

#else
		if (strSpriteSheetPath.rfind("./", 0) == 0) {
			Properties.strSpriteSheetPath = strSpriteSheetPath.substr(2);
		}

		if (strTiledMapTMXPath.rfind("./", 0) == 0) {
			Properties.strTiledMapTMXPath = strTiledMapTMXPath.substr(2);
		}
#endif



	}


	LevelLoader::~LevelLoader()
	{

	}

	void LevelLoader::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate
		if (Properties.strSpriteSheetPath == "" || Properties.strTiledMapTMXPath == "") return;

		LoadLevel(Properties.strSpriteSheetPath, Properties.strTiledMapTMXPath, Properties.nLevelNumber);

		bisLevelLoaded = true;
	}


	void LevelLoader::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
		if (bisLevelLoaded)
		{

		}
	}

	bool LevelLoader::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		if (bisLevelLoaded)
		{

		}

		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	void LevelLoader::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
		if (bisLevelLoaded)
		{

		}

	}

	void LevelLoader::LoadLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel)
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
			vecPartialDecalInfo.resize(140 * 24);

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


	void LevelLoader::DrawLevel()
	{
		// Fires just After the main OnUserUpdate
		if (bisLevelLoaded)
		{

		}


	}

	void LevelLoader::ClearLevel()
	{
		delete Properties.renSpriteSheet.Decal();
		delete Properties.renSpriteSheet.Sprite();
	}



} // olc

#endif

