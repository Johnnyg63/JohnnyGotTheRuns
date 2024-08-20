#pragma once
#include "pch.h"
/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_PGEX_LEVEL_LOADER

#endif

namespace olc
{
	/*
	* Manages the Background Images
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

		/*
		* Stores data required for the background image(s) to display correctly
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

	// See Step 3: Rename to your Class name
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
		if (Properties.strTiledMapTMXPath == "" || Properties.strTiledMapTMXPath == "") return;

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
		bisLevelLoaded = true;
	}


	void LevelLoader::DrawLevel()
	{
		// Fires just After the main OnUserUpdate
		if (bisLevelLoaded)
		{
			size_t nCount = map.LayerData.size();
			nCount = map.MapData.data.size();
			nCount = map.TilesetData.data.size();

			int x = 0;
			int y = 0;
			bool bTest = false;

			for (auto& layer : map.LayerData)
			{
				auto rowYtiles = layer.tiles;  // 11 Rows
				for (auto& tiles : rowYtiles)
				{
					x = 0;
					for (auto& tile : tiles)
					{

						int tileId = tile;
						if (tileId > 0.)
						{
							// Draw something
							int tileX = (tileId - 1) % 14;		// Number of X tiles Johnngy!!!!... number of tiles 
							int tileY = (tileId - 1) / 14;

							float spriteX = x * 70;
							float spriteY = y * 70;

							float sourceX = tileX * 70;
							float sourceY = tileY * 70;

							pge->DrawRectDecal({ spriteX, spriteY }, { 70.0f, 70.0f });
							pge->DrawPartialDecal({ spriteX, spriteY }, { 70.0f, 70.0f }, Properties.renSpriteSheet.Decal(), { sourceX, sourceY }, { 70.0f, 70.0f });

						}

						x++;
					}

					y++;
				}

			}
		}
	

	}

	void LevelLoader::ClearLevel()
	{
		delete Properties.renSpriteSheet.Decal();
		delete Properties.renSpriteSheet.Sprite();
	}



} // olc

#endif

