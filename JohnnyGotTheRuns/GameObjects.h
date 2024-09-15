#pragma once

#include "pch.h"

/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_GAME_OBJECT

#endif

namespace olc
{
	/*
	*  Unique ID Manager
	*/
	class UniqueID {
	private:
		static int counter;
		int id = 0;

	public:
		UniqueID() : id(++counter) {}

		int getID() const {
			return id;
		}
	};

	int UniqueID::counter = 0;

	/*
	* Manages the Background Images
	*/
	class GameObjects
	{

	public:
		GameObjects();
		virtual ~GameObjects();

		enum RESULT {
			SUCCESS = 0,
			FAIL,
			NOT_FOUND
		};



		/*
		* Add a game object
		* strSpriteSheetPath : Full path to Sprite Sheet (Required)
		* eObjectType : PlayerObject::OBJECT_TYPE (NONE, PLAYER, GAME_CHAR, OTHER_PLAYER, ENEMY) (Required)
		* strObjectName : A string name for your object (Required)
		* nObjectNumber : An object number, handly for when you have muliple of same object, Default 0;
		* nLives :	Number of lives, Default: 3
		* vfVelocity : The velocity of the object, default {100.0f, 100.0f)
		*/
		std::shared_ptr<olc::PlayerObject> AddGameObject(const std::string strSpriteSheetPath,
															const PlayerObject::OBJECT_TYPE eObjectType,
															const std::string strObjectName, 
															const int8_t nObjectNumber = 0,
															const int32_t nLives = 3, 
															const olc::vf2d vfVelocity = { 100.0f, 100.0f });

		/*
		* Returns the first pointer found by strName, else nullptr
		*/
		std::shared_ptr<olc::PlayerObject> GetGameObject(const std::string strName);

		/*
		* Returns the first pointer found by bObjectNumber, else nullptr
		*/
		std::shared_ptr<olc::PlayerObject> GetGameObject(const int8_t nObjectNumber);

		/*
		* Returns the pointer by nObjectID (Unique), else nullptr
		*/
		std::shared_ptr<olc::PlayerObject> GetGameObject(const uint16_t nObjectID);

		/*
		* Erases All Game Object by strName
		*/
		olc::GameObjects::RESULT EraseGameObject(const std::string strName);

		/*
		* Erases All Game Object by nObjectNumber
		*/
		olc::GameObjects::RESULT EraseGameObject(const int8_t nObjectNumber);

		/*
		* Erases the pointer by nObjectID (Unique)
		*/
		olc::GameObjects::RESULT EraseGameObject(const uint16_t nObjectID);


		/*
		*  Loads the game objects for the passed level
		*/
		void LoadLevelObjects(uint16_t nLevel);

		/*
		* Clears all the game objects
		* bIncludePlayer: Clears the player object if set to true
		*/
		void ClearGameObjects(bool bIncludePlayer = false);


	public:



		struct ObjectProperites
		{
			std::string strName = "Main Background";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , GameObjects 2 etc

			std::vector<std::shared_ptr<olc::PlayerObject>> vecPlayerObjects;

		};

		ObjectProperites Properties;

	private:


	};

}


#ifdef OLC_GAME_OBJECT
#undef OLC_GAME_OBJECT

namespace olc
{

	// See Step 3: Rename to your Class name
	GameObjects::GameObjects()
	{

	}


	// See Step 3: Rename to your Class name
	GameObjects::~GameObjects()
	{

	}

	std::shared_ptr<olc::PlayerObject> GameObjects::AddGameObject(std::string strSpriteSheetPath,
																PlayerObject::OBJECT_TYPE eObjectType,
																std::string strObjectName, int8_t nObjectNumber,
																int32_t nLives, olc::vf2d vfVelocity)
	{
		std::shared_ptr<olc::PlayerObject> pPlayerObject;
		pPlayerObject = std::make_unique<olc::PlayerObject>(strSpriteSheetPath, eObjectType);

		UniqueID uniqueID;
		pPlayerObject->Properties.nObjectID = uniqueID.getID();

		pPlayerObject->Properties.strName = strObjectName;
		pPlayerObject->Properties.nObjectNumber = nObjectNumber;
		pPlayerObject->Properties.nLives = nLives;
		pPlayerObject->Properties.vfVelocity = vfVelocity;

		Properties.vecPlayerObjects.push_back(pPlayerObject);

		return pPlayerObject;

	}

	std::shared_ptr<olc::PlayerObject> GameObjects::GetGameObject(const std::string strName)
	{
		std::shared_ptr<olc::PlayerObject> pPlayerObject = nullptr;

		for (auto& playerObject : Properties.vecPlayerObjects)
		{
			if (playerObject->Properties.strName == strName)
			{
				pPlayerObject = playerObject;
				break;
			}
		}

		return pPlayerObject;
	}

	std::shared_ptr<olc::PlayerObject> GameObjects::GetGameObject(const int8_t nObjectNumber)
	{
		std::shared_ptr<olc::PlayerObject> pPlayerObject = nullptr;

		for (auto& playerObject : Properties.vecPlayerObjects)
		{
			if (playerObject->Properties.nObjectNumber == nObjectNumber)
			{
				pPlayerObject = playerObject;
				break;
			}
		}

		return pPlayerObject;
	}

	std::shared_ptr<olc::PlayerObject> GameObjects::GetGameObject(const uint16_t nObjectID)
	{
		std::shared_ptr<olc::PlayerObject> pPlayerObject = nullptr;

		for (auto& playerObject : Properties.vecPlayerObjects)
		{
			if (playerObject->Properties.nObjectID == nObjectID)
			{
				pPlayerObject = playerObject;
				break;
			}
		}

		return pPlayerObject;
	}

	olc::GameObjects::RESULT GameObjects::EraseGameObject(const std::string strName)
	{
		olc::GameObjects::RESULT result = olc::GameObjects::RESULT::NOT_FOUND;
		std::shared_ptr<olc::PlayerObject> pPlayerObject = GetGameObject(strName);

		if (pPlayerObject != nullptr)
		{
			Properties.vecPlayerObjects.erase(std::remove(Properties.vecPlayerObjects.begin(),
															Properties.vecPlayerObjects.end(),
															pPlayerObject),
															Properties.vecPlayerObjects.end());
		}

		return result;
	}

	olc::GameObjects::RESULT GameObjects::EraseGameObject(const int8_t nObjectNumber)
	{
		olc::GameObjects::RESULT result = olc::GameObjects::RESULT::NOT_FOUND;
		std::shared_ptr<olc::PlayerObject> pPlayerObject = GetGameObject(nObjectNumber);

		if (pPlayerObject != nullptr)
		{
			Properties.vecPlayerObjects.erase(std::remove(Properties.vecPlayerObjects.begin(), 
															Properties.vecPlayerObjects.end(), 
															pPlayerObject), 
															Properties.vecPlayerObjects.end());
		}

		return result;
	}

	olc::GameObjects::RESULT GameObjects::EraseGameObject(const uint16_t nObjectID)
	{
		olc::GameObjects::RESULT result = olc::GameObjects::RESULT::NOT_FOUND;
		std::shared_ptr<olc::PlayerObject> pPlayerObject = GetGameObject(nObjectID);

		if (pPlayerObject != nullptr)
		{
			Properties.vecPlayerObjects.erase(std::remove(Properties.vecPlayerObjects.begin(),
															Properties.vecPlayerObjects.end(),
															pPlayerObject),
															Properties.vecPlayerObjects.end());
		}

		return result;
	}



	void GameObjects::ClearGameObjects(bool bIncludePlayer)
	{
		// TODO: This won't work, need an interator

		if (!bIncludePlayer)
		{
			std::shared_ptr<olc::PlayerObject> pPlayerObject;

			for (auto& player : Properties.vecPlayerObjects)
			{
				if (player->Properties.eObjectType == PlayerObject::OBJECT_TYPE::PLAYER)
				{
					pPlayerObject = player;
					break;
				}
			}

			Properties.vecPlayerObjects.clear();

			Properties.vecPlayerObjects.push_back(pPlayerObject);

		}
	}




	void GameObjects::LoadLevelObjects(uint16_t nLevel)
	{
		// TODO: We need to link this to a levelloader class when I get around to creating it

		// Create Player:
		auto pPlayer = AddGameObject("assets/images/playerSpriteSheet.png",
			PlayerObject::OBJECT_TYPE::PLAYER,
			"Johnny", 0, 3, { 100.0f, 100.0f });

		pPlayer->Properties.vfStartPosition.x = 10.0f; //(GetScreenSize().x / 100.0f) * 74.0f;
		pPlayer->Properties.vfStartPosition.y = 10.0f;//(GetScreenSize().y / 100.0f) * 75.0f;
		pPlayer->Properties.vfPosition = pPlayer->Properties.vfStartPosition;
		pPlayer->Properties.vfMasterScaler = { 0.50f, 0.50f }; // Our player is HD and Big, bring him down a little

		// Create Chars

		auto pMale = AddGameObject("assets/images/maleSpriteSheet.png",
			PlayerObject::OBJECT_TYPE::GAME_CHAR,
			"Jim the Janitor", 1, 3, { 100.0f, 100.0f });


		pMale->Properties.vfStartPosition.x = 20.0f; // (GetScreenSize().x / 100.0f) * 20.0f;
		pMale->Properties.vfStartPosition.y = 20.0f; // (GetScreenSize().y / 100.0f) * 61.0f;
		pMale->Properties.vfPosition = pMale->Properties.vfStartPosition;
		pMale->Properties.vfMasterScaler = { 0.50f, 0.50f }; // Our player is HD and Big, bring him down a little



	}






} // olc

#endif
