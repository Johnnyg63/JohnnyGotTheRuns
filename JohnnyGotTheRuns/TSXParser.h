
#include "pch.h"
#include <strstream>

using namespace olc;

struct XMLTag_TSX
{
	std::string tag;
	std::map<std::string, std::string> data;

	const std::string FormatTagData(std::map<std::string, std::string>tiles)
	{
		std::string displayStr = "";

		for (std::map<std::string, std::string>::iterator it = data.begin(); it != data.end(); it++)
		{
			displayStr += "  " + it->first + ": " + it->second + "\n";
		}

		return displayStr;
	}

	friend std::ostream& operator << (std::ostream& os, XMLTag_TSX& rhs)
	{
		os << rhs.tag << "\n" << rhs.FormatTagData(rhs.data) << "\n";

		return os;
	}

	int GetInteger(std::string dataTag)
	{
		return std::stoi(data[dataTag]);
	}

	double GetDouble(std::string dataTag)
	{
		return std::stod(data[dataTag]);
	}

	bool GetBool(std::string dataTag)
	{
		if (data[dataTag] == "0")
		{
			return false;
		}
		else
		{
			return true;
		}

	}

};



struct Tile
{
	XMLTag_TSX sTileData;
	XMLTag_TSX sObjectGroupData;
	XMLTag_TSX sObjectData;
	XMLTag_TSX sTypeData;
};



struct Map_TSX
{
	XMLTag_TSX ImageData;
	XMLTag_TSX TilesetData;
	std::vector<Tile> vecTiles;

};

class TSXParser {

public:

	Map_TSX GetData()
	{
		return parsedMapInfo;
	}

private:

	Map_TSX parsedMapInfo;
	Tile sTile;

	bool bIsFirstPass = true;

	void ParseTag(std::string tag)
	{
		XMLTag_TSX newTag;
		int test = 0;   // TODO: Removed

		//First character is a '<' so we discard it.
		tag.erase(0, 1);

		//last characters in the tag '>'. 
		tag.erase(tag.length() - 1, 1);


		// We have an edge case, where the last characters can be > or /> 
		// We need to manage this with a conditional statement
		char lastChar = tag.at(tag.length() - 1);
		if (lastChar == '/')
		{
			//last characters in the tag '/'. 
			tag.erase(tag.length() - 1, 1);
		}


		// Now parse by spaces.
		std::stringstream s(tag); //Turn it into a string stream to now parse into individual whitespaces.
		std::string data;

		while (s.good())
		{
			s >> data;

			if (newTag.tag.length() == 0)
			{
				//Tag's empty, so first line is the tag.
				newTag.tag = data;
				std::cout << "Tag: " << newTag.tag << "\n";
			}
			else
			{
				// TODO: Edge case there will be tags that are not in the format of key-->value, example:  <point/>
				// We need to manage this

				std::string key = data.substr(0, data.find("="));
				std::string value = data.substr(data.find("=") + 1, std::string::npos);

				//Strip Quotation marks, if they exist. 
				if (value.substr(0, 1) == "\"") value = value.substr(1, std::string::npos);
				if (value.substr(value.length() - 1, 1) == "\"") value = value.substr(0, value.length() - 1);

				newTag.data[key] = value;
				std::cout << "  " << key << ":" << newTag.data[key] << "\n";

			} // END if(newTag.tag.length() == 0...

		} // END While(sd.good())


		if (newTag.tag == "tileset")
		{
			parsedMapInfo.TilesetData = newTag;

		}
		else if (newTag.tag == "image")
		{
			parsedMapInfo.ImageData = newTag;
		}
		else if (newTag.tag == "tile")
		{
			if (!bIsFirstPass) UpdateVectorIfRequired();

			bIsFirstPass = false;
			// Set the default of rect for tiles that do not have a type (point, ellipse, polygon)
			sTile.sTypeData.tag = "RECT";
			sTile.sTypeData.data.clear();
			sTile.sTileData = newTag;
		}
		else if (newTag.tag == "objectgroup")
		{
			sTile.sObjectGroupData = newTag;
		}
		else if (newTag.tag == "object")
		{
			sTile.sObjectData = newTag;
		}
		else if (newTag.tag == "point")
		{
			sTile.sTypeData = newTag;
		}
		else if (newTag.tag == "ellipse")
		{
			sTile.sTypeData = newTag;

		}
		else if (newTag.tag == "polygon")
		{
			sTile.sTypeData = newTag;

		}
		else if (newTag.tag == "/object")
		{
			UpdateVectorIfRequired();

		}
		else if (newTag.tag == "/objectgroup")
		{
			UpdateVectorIfRequired();
		}
		else if (newTag.tag == "/tile")
		{
			UpdateVectorIfRequired();
		}
		else
		{
			std::cout << "Unsupported tag format! Ignoring." << "\n";
		}




		std::cout << "\n" << "=============\n";
	}

	/*
	* We only update the tile vectors if there are no duplicates
	*/
	void UpdateVectorIfRequired()
	{
		bool bFound = false;

		Tile tile = { sTile }; // Copy off our tile

		for (auto& tile : parsedMapInfo.vecTiles)
		{
			if (tile.sTileData.data == sTile.sTileData.data)
			{
				bFound = true;
				break;
			}
		}
		if(!bFound)	parsedMapInfo.vecTiles.push_back(tile);
	}

public:
	TSXParser(std::string file)

	{
		std::ifstream f(file, std::ios::in);

		std::string accumulator = "";

		while (f.good()) 
		{
			std::string data;
			f >> data;
			if (data.empty()) continue;

			if (accumulator.length() > 0) 
			{
				accumulator += " " + data;

				//Check if it ends with '>'
				if (data[data.length() - 1] == '>') 
				{
					ParseTag(accumulator);
					accumulator = "";
				}
			}
			else
				if (data[0] == '<') 
				{
					//Beginning of XML tag.
					accumulator = data;

					// Let check if it is an end tag </object>
					if (data[data.length() - 1] == '>')
					{
						// ok this is a possiable end tag
						accumulator = data;
						ParseTag(accumulator);
						accumulator = "";
					}

				}
				else 
				{
					//Start reading in data for this layer.
					std::vector<int>rowData;

					while (data.find(",") != std::string::npos)
					{
						std::string datapiece = data.substr(0, data.find(","));
						data = data.substr(data.find(",") + 1, std::string::npos);
						rowData.push_back(stoi(datapiece));
					}
					if (data.length()) 
					{
						rowData.push_back(stoi(data));
					}
					//parsedMapInfo.LayerData[parsedMapInfo.LayerData.size() - 1].tiles.push_back(rowData);
				}
		}

		
	}

	virtual ~TSXParser()
	{
		// TODO
	}
};