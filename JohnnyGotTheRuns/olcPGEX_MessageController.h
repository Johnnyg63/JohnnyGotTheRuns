#pragma once


#include "olcPixelGameEngine.h"


/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_PGEX_MESSAGE_CONTROLLER

#endif

namespace olc
{
	class MessageController : public PGEX
	{

	public:

		MessageController(std::string strSpriteSheetPath);
		virtual ~MessageController();

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

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created sprite
		void DrawSprite();

		// Add your own public methods here


	public:

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
			std::string strName = "Message Controller";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , MessageController 2 etc

			bool bAutoScale = true;				// Automatically scales the background image to fit within the screen size

			olc::vf2d vfPosition = { 0.0f,0.0f };	// Image POS {x,y} (float), Default {0.0f,0.0f}

			std::string strSpriteSheetPath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""

			olc::Renderable renSpriteSheet;               // Keeps the sprImage and decImage in the one location

			/*
			* Holds the vSource and vSize of the image
			* Edit this value to the location of the Sprite when using a Sprite sheet
			*/
			ImageInfo sImageInfo;

		};

		ObjectProperites Properties;

	private:
		
		struct sFont
		{
			char font;			// Letter or number we want
			vi2d viLocation;	// Location in the sprite sheet
			vi2d viSize;		// Size of the Font
		};

		sFont GameFont;

		std::vector<sFont> vecFonts;

		// Loads the fonts into the vector, TODO: Convert to XML Reader!
		void LoadFonts(); 


	};

}


#ifdef OLC_PGEX_MESSAGE_CONTROLLER
#undef OLC_PGEX_MESSAGE_CONTROLLER

namespace olc
{

	MessageController::MessageController(std::string strSpriteSheetPath) : PGEX(true)
	{
		
#if defined (_MSC_VER)
		// Windows stuff

		if (strSpriteSheetPath.rfind("./", 0) != 0) {
			Properties.strSpriteSheetPath = "./" + strSpriteSheetPath;
		}
#else
		if (ImagePath.rfind("./", 0) == 0) {
			Properties.strSpriteSheetPath = strSpriteSheetPath.substr(2);
		}
#endif

		

	}

	// See Step 3: Rename to your Class name
	MessageController::~MessageController()
	{

	}

	// See Step 3: Rename to your Class name
	void MessageController::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

		// Load up our image
		Properties.renSpriteSheet.Load(Properties.strSpriteSheetPath);
		Properties.sImageInfo.vSize.x = Properties.renSpriteSheet.Sprite()->width;
		Properties.sImageInfo.vSize.y = Properties.renSpriteSheet.Sprite()->height;

	}

	// See Step 3: Rename to your Class name
	void MessageController::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	// See Step 3: Rename to your Class name
	bool MessageController::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void MessageController::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
	}

	// See Step 3: Rename to your Class name
	void MessageController::DrawDecal()
	{

	}

	// See Step 3: Rename to your Class name
	void MessageController::DrawSprite()
	{

	}

	void MessageController::LoadFonts()
	{
		// Right this will be a long list...., TODO convert to XML
		
		// Clear our vector
		vecFonts.clear();
		
		vecFonts.push_back({ 'A', {412, 835}, {61, 64}});
		vecFonts.push_back({ 'B', {487, 1537}, {50, 66} });
		vecFonts.push_back({ 'C', {460, 1613}, {52, 66} });
		vecFonts.push_back({ 'D', {432, 835}, {54, 66} });
		vecFonts.push_back({ 'E', {511, 1965}, {45, 64} });
		vecFonts.push_back({ 'F', {512, 963}, {44, 64} });
		vecFonts.push_back({ 'G', {460, 1107}, {52, 66} });
		vecFonts.push_back({ 'H', {473, 835}, {51, 64} });
		vecFonts.push_back({ 'I', {524, 835}, {22, 64} });
		vecFonts.push_back({ 'J', {512, 1027}, {42, 66} });
		vecFonts.push_back({ 'K', {432, 1821}, {53, 64} });
		vecFonts.push_back({ 'L', {512, 899}, {44, 64} });
		vecFonts.push_back({ 'M', {392, 1967}, {66, 64} });
		vecFonts.push_back({ 'N', {432, 1679}, {53, 64} });
		vecFonts.push_back({ 'O', {418, 1284}, {60, 66} });
		vecFonts.push_back({ 'P', {489, 1427}, {48, 65} });
		vecFonts.push_back({ 'Q', {418, 1205}, {60, 79} });
		vecFonts.push_back({ 'R', {478, 1249}, {51, 65} });
		vecFonts.push_back({ 'S', {511, 1899}, {46, 66} });
		vecFonts.push_back({ 'T', {460, 1043}, {52, 64} });
		vecFonts.push_back({ 'U', {485, 1757}, {51, 66} });
		vecFonts.push_back({ 'V', {400, 913}, {61, 64} });
		vecFonts.push_back({ 'W', {136, 1320}, {76, 64} });
		vecFonts.push_back({ 'X', {418, 1409}, {58, 64} });
		vecFonts.push_back({ 'Y', {432, 1473}, {57, 64} });
		vecFonts.push_back({ 'Z', {486, 1613}, {50, 64} });

	}



} // olc

#endif
