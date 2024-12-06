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
		Map_TSX map_TSX; //TSXParser Map

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

		// Holds the tiledmap map info
		MapInfo sMapInfo;

		struct TileSetLocation
		{
			std::string strFirstgid = "";	// ID
			std::string strScource = "";	// Tiled Map TSX file location
		};

		// Holds the tiledmap TileSet info
		TileSetLocation sTileSetLocation;

		struct TileSetInfo
		{
			std::string strVersion = "";
			std::string strTiledVersion = "";
			std::string strName = "";
			olc::vf2d vfTileSize = { 0.0f, 0.0f };
			int32_t nTileCount = 0;
			int32_t nColumns = 0;


		};

		TileSetInfo sTileSetInfo;

		struct TileSetSpriteImage
		{
			std::string strSource = "";
			olc::vf2d vfSize = { 0.0f, 0.0f };

		};

		TileSetSpriteImage sTileSetSpriteImage;


		enum Collision
		{
			RECT = 0,
			POINT,
			ELLIPSE,
			POLYGON
		};

		//<polygon points="0,0 32.6667,33.3333 -0.666667,32.6667"/>

		struct CollisionType
		{
			Collision eCollision = Collision::RECT;	// Collision object type, Default RECT
			std::vector<olc::vf2d> vecPoints;
		};

		struct TileObject
		{
			int32_t nTileObjectID = 0;				// Tile Object ID
			std::string strName = "NOT_SET";		// Name if passed,default: "NOT_SET"
			std::string strClassType = "NOT_SET";	// Class type if passed, default: "NOT_SET"
			olc::vf2d vfPosition = { 0.0f, 0.0f };	// Object Start Poistion X,Y
			olc::vf2d vfSize = { 0.0f, 0.0f };		// Object Size	Width, Height
			float fRotationDeg = 0.0f;				// Object Rotation in Degrees
			float fRotationRad = 0.0f;				// Object Rotation in Radians
			CollisionType sCollisionType;			// Stores tthe Collision Type data, RECT, POINT, ELLIPSE, POLYGON
		};

		struct Tile
		{
			int32_t nTileID = 0;					// Tile ID
			std::string strClassType = "NOT_SET";	// Class type if passed, default: "NOT_SET"
			std::string strDrawOrder = "NOT_SET";	// Draw Order if passed, default: "NOT_SET"
			int32_t nObjectGroupID = 0;				// Object Group ID
			std::vector<TileObject> vecTileObjects;	// Vector of TileObject
			

		};

		std::vector<Tile> vecTiles;

		struct DecalInfo
		{
			bool bIsVisable = true;						// Is layer is visable
			bool bIsLocked = true;						// Is layer is locked
			int16_t nLayerID = 0;						// Layer id number
			std::string strName = "";					// Layer name
			std::string strClass = "";					// Layer Class name
			int32_t nWidth = 0;							// Layer Width
			int32_t nHeight = 0;						// Layer Height
			int32_t nTiledID = 0;						// Tiled Map Editor ID 
			olc::vf2d vfDrawLocation = { 0.0f, 0.0f };	// Locatoin of where to draw
			olc::vf2d vfSourcePos = { 0.0f, 0.0f };		// Location on Sprite Sheet
			olc::vf2d vfSoureSizePos = { 0.0f, 0.0f };	// Size of Partial Decal

			int32_t nDecalID = 0;						// Holds the Tile ID to draw this decal

			// Collision Info
			bool bHasCollision = false;					// Has collision object 
			Tile sCollisionTile;						// Stores the tile collision details


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

		// Rotates a point around the Center Position
		olc::vf2d RotatePoint(olc::vf2d vfCenterPos, float fRadians, olc::vf2d vfPoint);

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

		//Properties.strTiledMapTMXPath = "C:/Users/jgalv/source/repos/JohnnyGotTheRuns/JohnnyGotTheRuns/assets/tiledprojects/Industrial35x35.tsx";

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


		// TileSet Location
		for (auto& tileSetInfo : map.TilesetData.data)
		{
			if (tileSetInfo.first == "firstgid") sTileSetLocation.strFirstgid = tileSetInfo.second;
			if (tileSetInfo.first == "source") sTileSetLocation.strScource = tileSetInfo.second;

			// Load the tsx file for collections etc
			if (sTileSetLocation.strScource != "")
			{
				TSXParser tsxParser = TSXParser(sTileSetLocation.strScource);
				map_TSX = tsxParser.GetData();
			}
		}

		// Get the tileset data
		for (auto& tileSetInfo : map_TSX.TilesetData.data)
		{
			if (tileSetInfo.first == "version") sTileSetInfo.strVersion = tileSetInfo.second;
			if (tileSetInfo.first == "tiledversion") sTileSetInfo.strTiledVersion = tileSetInfo.second;
			if (tileSetInfo.first == "name") sTileSetInfo.strName = tileSetInfo.second;
			if (tileSetInfo.first == "tilewidth") sTileSetInfo.vfTileSize.x = std::stof(tileSetInfo.second);
			if (tileSetInfo.first == "tileheight") sTileSetInfo.vfTileSize.y = std::stof(tileSetInfo.second);
			if (tileSetInfo.first == "tilecount") sTileSetInfo.nTileCount = std::stoi(tileSetInfo.second);
			if (tileSetInfo.first == "columns") sTileSetInfo.nColumns = std::stoi(tileSetInfo.second);

		}



		// Get the Sprite image used for the tile set
		for (auto& imageInfo : map_TSX.ImageData.data)
		{
			if (imageInfo.first == "source") sTileSetSpriteImage.strSource = imageInfo.second;
			if (imageInfo.first == "width") sTileSetSpriteImage.vfSize.x = std::stof(imageInfo.second);
			if (imageInfo.first == "height") sTileSetSpriteImage.vfSize.y = std::stof(imageInfo.second);

		}


		for (auto& tileInfo : map_TSX.vecTiles)
		{
			// 1: Create a new struct
			Tile sTile;

			// Get the tile Data
			//  <tile id="236">
			for (auto& tileData : tileInfo.sTileData.data)
			{
				// Right need to explain this, the TMX CSV data tile ID will always be + 1 greater than 
				// The tileID held in the TSX data. There is a lot of reasons for this, but for our code to work correctly
				// we need to ensure our sTile.ID matchs that of which is in the TMX file. 
				if (tileData.first == "id") sTile.nTileID = (std::stoi(tileData.second) + 1);
				if (tileData.first == "type") sTile.strClassType = tileData.second;
			}

			// Get the Object Group data
			// <objectgroup draworder="index" id="2">
			for (auto& objectGroupData : tileInfo.sObjectGroupData.data)
			{
				if (objectGroupData.first == "id") sTile.nObjectGroupID = std::stoi(objectGroupData.second);
				if (objectGroupData.first == "draworder") sTile.strDrawOrder = objectGroupData.second;
			}

			

			// Get the Object data:
			/*
			*   <object id="1" name="Left_Triangle" type="clsLeftTriangle" x="0.176258" y="9.51793">
			*		<polygon points="0,0 4.51712,-9.51793 6.69781,0.352516"/>
			*   </object>
			*	<object id="2" name="Right_Triangle" type="clsRightTriangle" x="27.4963" y="9.87045">
			*		<polygon points="0,0 4.44713,-9.69419 7.22658,-0.528774"/>
			*   </object>
			*   <object id="3" x="13.5719" y="0.705032" width="7.22658" height="7.93161"/>
			*/
			for (auto& sObjectDataInfo : tileInfo.vecObjectDataInfo)
			{
				// Defaults
				TileObject sTileObject;
				sTileObject.sCollisionType.eCollision == Collision::RECT;

				// <object id="1" name="Left_Triangle" type="clsLeftTriangle" x="0.176258" y="9.51793">
				for (auto& objectData : sObjectDataInfo.sObjectData.data)
				{

					if (objectData.first == "id") sTileObject.nTileObjectID = std::stoi(objectData.second);
					if (objectData.first == "name") sTileObject.strName = objectData.second;
					if (objectData.first == "type") sTileObject.strClassType = objectData.second;
					if (objectData.first == "x") sTileObject.vfPosition.x = std::stof(objectData.second);
					if (objectData.first == "y") sTileObject.vfPosition.y = std::stof(objectData.second);
					if (objectData.first == "width") sTileObject.vfSize.x = std::stof(objectData.second);
					if (objectData.first == "height") sTileObject.vfSize.y = std::stof(objectData.second);
					if (objectData.first == "rotation")
					{
						sTileObject.fRotationDeg = std::stof(objectData.second);
						// Get our radians // TODO we need to increase PI for more accurate results 
						sTileObject.fRotationRad = sTileObject.fRotationDeg * 3.1415927f / 180.0f;

					}

				}

				
				if (sObjectDataInfo.sTypeData.tag == "rect") 
				{ 
					sTileObject.sCollisionType.eCollision = Collision::RECT;
				}
				if (sObjectDataInfo.sTypeData.tag == "point") sTileObject.sCollisionType.eCollision = Collision::POINT;
				if (sObjectDataInfo.sTypeData.tag == "polygon")
				{ 
					sTileObject.sCollisionType.eCollision = Collision::POLYGON;
				}
				if (sObjectDataInfo.sTypeData.tag == "ellipse") sTileObject.sCollisionType.eCollision = Collision::ELLIPSE;

				// <polygon points = "0,0 4.51712,-9.51793 6.69781,0.352516" / >
				for (auto& typeData : sObjectDataInfo.sTypeData.data)
				{
					/*
					* Lets parse out our points to string --> olc::vf2d
					* example: -0.666667,32.6667 (x, y)
					* As we are dealing with data we use typeData.second as this is where the data is stored
					*/
					std::string strX = typeData.second.substr(0, typeData.second.find(","));
					std::string strY = typeData.second.substr(typeData.second.find(",") + 1, std::string::npos);
					olc::vf2d vfPoint = { 0.0f, 0.0f };
					vfPoint.x = std::stof(strX);
					vfPoint.y = std::stof(strY);
					sTileObject.sCollisionType.vecPoints.push_back(vfPoint);

				}




				// Ok we need to check if we have a Triangle or Ploygon
				if (sTileObject.sCollisionType.vecPoints.size() > 3)
				{
					// ok we have a polygon, and need to treat it a little different
					// First we need to reverse the polygon
					std::reverse(sTileObject.sCollisionType.vecPoints.begin(), sTileObject.sCollisionType.vecPoints.end());
					olc::vf2d vfEndPoint = { 0.0f, 0.0f };

					// Now we need to add the final point, which is the same as the first point {0.0}
					// There will be a starting point offset that will be applied to this value
					// Example: <object id="9" x="0.197658" y="9.78406"> the x/y here are the offset
					// Example <polygon points = "0,0 10.2782,-9.78406 15.615,0.296487 25.5967,-9.78406 30.1428,0.0988289" / > we have more that 3 points we need to add the 0,0 at the end and reverse
					sTileObject.sCollisionType.vecPoints.push_back(vfEndPoint);
				}


				// Now we need to check if the object is rotated
				if (sTileObject.fRotationRad != 0.0f)
				{
					// Right the user has applied some rotation.
					// We have a few cases to manager here.

					switch (sTileObject.sCollisionType.eCollision)
					{
					case Collision::RECT:
					{
						// For a rect we need to convert to triangles (polgyon) as it makes out collision code easier to work with
						// 1: Get the 4 points of the rect
						olc::vf2d vfTopLeft = sTileObject.vfPosition;
						olc::vf2d vfTopRight = sTileObject.vfPosition + olc::vf2d{ sTileObject.vfSize.x, 0.0f };
						olc::vf2d vfBottomLeft = sTileObject.vfPosition + olc::vf2d{ 0.0f, sTileObject.vfSize.y };
						olc::vf2d vfBottomRight = sTileObject.vfPosition + sTileObject.vfSize;

						// 2: Get our center point : xCenter = (x1 + x2) / 2 , yCenter = (y1 + y2) / 2
						olc::vf2d vfCenterPoint = sTileObject.vfPosition; //olc::vf2d{ (vfTopLeft.x + vfTopRight.x) / 2.0f, (vfTopLeft.y + vfBottomLeft.y) / 2.0f };

						// 3: Rotate each point around the vfTopLeft position, TiledMap has already set this point, we do not need top calucate it
						//vfTopLeft = RotatePoint(vfCenterPoint, sTileObject.fRotationRad, vfTopLeft);
						vfTopRight = RotatePoint(vfCenterPoint, sTileObject.fRotationRad, vfTopRight);
						vfBottomLeft = RotatePoint(vfCenterPoint, sTileObject.fRotationRad, vfBottomLeft);
						vfBottomRight = RotatePoint(vfCenterPoint, sTileObject.fRotationRad, vfBottomRight);

						sTileObject.vfPosition = vfTopLeft;

						// 4: Now we need to convert into Polgon
						// NOTE a RECT in Tiled map has no offset position unlike a "real" triangle/ploygon in tilemap
						// For polygons we use the sTileObject.vfPosition as the offset point for the start point {0.0f,0.0f}
						// Example: TODO add <polygon....
						// Therefore we need to set sTileObject.vfPosition == {0.0f, 0.0f} so no offset is applied
						sTileObject.vfPosition = { 0.0f, 0.0f };
						sTileObject.sCollisionType.eCollision = Collision::POLYGON;

						sTileObject.sCollisionType.vecPoints.push_back(vfTopLeft);
						sTileObject.sCollisionType.vecPoints.push_back(vfTopRight);
						sTileObject.sCollisionType.vecPoints.push_back(vfBottomRight);
						sTileObject.sCollisionType.vecPoints.push_back(vfBottomLeft);
						sTileObject.sCollisionType.vecPoints.push_back(vfTopLeft);


						break;
					}
					default:
						break;
					}


				}
				
				sTile.vecTileObjects.push_back(sTileObject);
				

			} // END: for (auto& sObjectDataInfo : tileInfo.vecObjectDataInfo)

			vecTiles.push_back(sTile); // Push back the tile info for this decal

		}


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

		int test = 0; // TODO: Remove
		int32_t nIDs = 0;

		for (auto& layer : map.LayerData)
		{
			auto vecPartialDecalInfo = std::vector<DecalInfo>();
			auto rowYtiles = layer.tiles;
			auto layerTags = layer.tag.data;
			nIDs = 0; // Reset the Tile ID per layer

			for (auto& tiles : rowYtiles)
			{
				x = 0;
				for (auto& tile : tiles)
				{
					DecalInfo sDecalInfo;
					sDecalInfo.nDecalID = nIDs;

					// // id="1" name="L0" class="collision" width="140" height="24" visable="0" locked="1"
					for (auto& tag : layerTags)
					{
						if (tag.first == "id") sDecalInfo.nLayerID = std::stoi(tag.second);
						if (tag.first == "name") sDecalInfo.strName = tag.second;
						if (tag.first == "class") sDecalInfo.strName = tag.second;
						if (tag.first == "width") sDecalInfo.nWidth = std::stoi(tag.second);
						if (tag.first == "height") sDecalInfo.nHeight = std::stoi(tag.second);
						if (tag.first == "visable") sDecalInfo.bIsVisable = (std::stoi(tag.second) > 0) ? true : false;
						if (tag.first == "locked") sDecalInfo.bIsVisable = (std::stoi(tag.second) > 0) ? true : false;

					}

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

						float sourceX = tileX * sTileSetInfo.vfTileSize.x; // sMapInfo.nTileWidth;
						float sourceY = tileY * sTileSetInfo.vfTileSize.y; // sMapInfo.nTileHeight;

						for (auto& sTile : vecTiles)
						{
							if (sDecalInfo.nTiledID == sTile.nTileID)
							{
								sDecalInfo.bHasCollision = true;
								sDecalInfo.sCollisionTile = sTile;
								break;
							}
						}

						sDecalInfo.vfSourcePos = { sourceX , sourceY };

					}

					vecPartialDecalInfo.push_back(sDecalInfo);

					x++;
					nIDs++;
				}


				y++;
			}


			Properties.mapLayerInfo.insert({ nLayerCount, vecPartialDecalInfo });
			nLayerCount++;

		}

		//TODO: Add clean up


		bisLevelLoaded = true;
	}

	olc::vf2d LevelManager::RotatePoint(olc::vf2d vfCenterPos, float fRadians, olc::vf2d vfPoint) 
	{
		float tempX = vfPoint.x - vfCenterPos.x;
		float tempY = vfPoint.y - vfCenterPos.y;
		vfPoint.x = vfCenterPos.x + (tempX * cos(fRadians) - tempY * sin(fRadians));
		vfPoint.y = vfCenterPos.y + (tempX * sin(fRadians) + tempY * cos(fRadians));
		return vfPoint;
	}

	void LevelManager::ClearLevel()
	{
		delete Properties.renSpriteSheet.Decal();
		delete Properties.renSpriteSheet.Sprite();

		// TODO: Add code to clear all the structs and vectors!
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
		
		// Collision resizing
		olc::vf2d vfCollsionSize = { 0.0f, 0.0f };
		olc::vf2d vfOffSet = { 0.0f, 0.0f };
		
		std::vector <olc::vf2d> vfPolyPoints;
		std::vector <olc::vf2d> vfEmptyPoints;

		rect<float> worldTile;
		worldTile.pos.x = 0.0f;
		worldTile.pos.y = 0.0f;
		worldTile.size = { 35.0f, 35.0f };

		int test = 0; // TODO: Remove

		// Then looping through them and drawing them
		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				idx = vTile.y * Properties.viWorldSize.x + vTile.x;

				for (auto& layer : Properties.mapLayerInfo)
				{
					decalInfo = layer.second[idx];

					if (idx > 1540)
					{
						int pause = 0;
					}

					if (decalInfo.nTiledID == 0) continue; // If the tile does nothing just move on


					if (decalInfo.bHasCollision)
					{
						// NOTE: We are in world space so we need to get realworld....
						
						for (auto& tileObject : decalInfo.sCollisionTile.vecTileObjects)
						{
							switch (tileObject.sCollisionType.eCollision)
							{

							case Collision::RECT:
							{
								vfOffSet = Properties.tv->ScaleToWorld(tileObject.vfPosition);
								vfCollsionSize = Properties.tv->ScaleToWorld(tileObject.vfSize);
								Properties.tv->DrawRectDecal({ float(vTile.x + vfOffSet.x) , float(vTile.y + vfOffSet.y) }, vfCollsionSize, olc::RED);
								break;
							}
							case Collision::ELLIPSE:
							{
								break;
							}
							case Collision::POLYGON:
							{
							
								for (auto& vfPoint : tileObject.sCollisionType.vecPoints)
								{
									olc::vf2d vfPointnew = vTile + Properties.tv->ScaleToWorld(vfPoint + tileObject.vfPosition);
									vfPolyPoints.push_back(vfPointnew);
									olc::vf2d vfColour = { 0.0f, 0.0f };
									vfEmptyPoints.push_back(vfColour);

								}

								for (int i = 0; i < vfPolyPoints.size(); i++)
								{
									if (i == vfPolyPoints.size() - 1)
									{
										Properties.tv->DrawLineDecal(vfPolyPoints[i], vfPolyPoints[0], olc::RED);
									}
									else
									{
										Properties.tv->DrawLineDecal(vfPolyPoints[i], vfPolyPoints[i + 1], olc::RED);
									}
									
								}

								//Properties.tv->DrawPolygonDecal(nullptr, vfPolyPoints, vfEmptyPoints, olc::BLUE);

								vfPolyPoints.clear();
								vfEmptyPoints.clear();

								break;
							}
							case Collision::POINT:
							{
								break;
							}

							default:
								break;
							}
						}

					}

					switch (decalInfo.nLayerID)
					{
					case 1:
					{
						// This is our collision Layer, nothing to do just move on
						/*Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
							Properties.renSpriteSheet.Decal(),
							decalInfo.vfSourcePos,
							decalInfo.vfSoureSizePos);*/
						break;
					}
					case 2:
					{
						// this is our Ladder layer
						/*Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
							Properties.renSpriteSheet.Decal(),
							decalInfo.vfSourcePos,
							decalInfo.vfSoureSizePos);*/
						break;
					}
					case 3:
					{
						// this is our drawing layer
						/*Properties.tv->DrawPartialDecal({ (float)vTile.x, (float)vTile.y },
							Properties.renSpriteSheet.Decal(),
							decalInfo.vfSourcePos,
							decalInfo.vfSoureSizePos);*/
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

