#include "Map.h"

Map::Map() : mInit(false) {}

Map::~Map()
{
	Free();
}

void Map::Create(unsigned int cols, unsigned int rows)
{
	if (cols == 0 || rows == 0)
	{
		std::cerr << "Cols or rows = 0" << std::endl;
		return;
	}

	mSpriteMap.resize(cols);

	for (int i = 0; i < cols; i++)
	{
		mSpriteMap[i].resize(rows);
	}

	mInit = true;
}

void Map::Set(int newItem, int col, int row) {

	if (col < 0 || col > mCols - 1 || row < 0 || row < mRows - 1)
	{
		std::cerr << "Cant set negative map coord" << std::endl;
		return;
	}

	mSpriteMap[col][row] = newItem;
}

void Map::Set(int newItem, int index)
{
	if (index < 0 || index > mCols * mRows - 1)
	{
		std::cerr << "setting map co ord from out of range" << std::endl;
		return;
	}

	mSpriteMap[index % mCols][index / mCols] = newItem;
}

bool Map::LoadMap(const char* filepath, MapInfo* mapinfo)
{
	Free();

	if (ReadFile(filepath, mapinfo))
	{
		mInit = true;
		return true;
	}
	else
	{
		mInit = false;
		return false;
	}
}

bool Map::ReadFile(const char* path, MapInfo* mapinfo)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "failed to open file at " << path << std::endl;
		return false;
	}

	bool dimensionsSet = false;
	bool mapDataRead = false;

	int line = 0;

	int i = 0;

	while (!file.eof())
	{
		std::string curLine;
		std::getline(file, curLine);

		line++;
		
		if (curLine[0] == '#' || curLine.size() == 0)
		{
			continue;
		}

		std::vector<std::string> tokens = split(curLine, " ");

		if (tokens[0] == "dim")
		{
			if (tokens.size() != 3)
			{
				std::cerr << "failed to load map: dim missing operand or too many on line " << std::endl;
				std::cerr << "\tusage: dim [width : int][height : int]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;
				file.close();
				return false;
			}

			int width  = 0;
			int height = 0;

			try
			{
				width = std::stoi(tokens[1]);
				height = std::stoi(tokens[2]);
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\texception: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;
				file.close();
				return false;
			}


			mSpriteMap.resize(width);

			for (int i = 0; i < width; i++)
			{
				mSpriteMap[i].resize(height);
			}

			mCols = width; mRows = height;

			dimensionsSet = true;
			continue;
		}

		if(tokens[0] == "playerstart" && mapinfo)
		{
			if (tokens.size() != 3)
			{
				std::cerr << "failed to load map: incorrect usage of playerstart" << std::endl;
				std::cerr << "\tusage: playerstart [x : int] [y : int]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			try
			{
				mapinfo->playerStartPos = vec2(std::stoi(tokens[1]), std::stoi(tokens[2]));
				if (mapinfo->playerStartPos.x < 0 || mapinfo->playerStartPos.x > mCols - 1 || mapinfo->playerStartPos.y < 0 || mapinfo->playerStartPos.y > mRows - 1)
				{
					std::cerr << "failed to load map: playerstart out of range of map" << std::endl;
					std::cerr << "\tusage: playerstart [x : int] [y : int]" << std::endl;
					std::cerr << "\twhere x and y are in ranges of map width and height respectively" << std::endl;
					std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
					std::cerr << "\t" << curLine << std::endl;

					file.close();
					return false;
				}
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			continue;
		}

		if (tokens[0] == "playerspeed" && mapinfo)
		{
			if (tokens.size() != 2)
			{
				std::cerr << "failed to load map: incorrect usage of playerstart" << std::endl;
				std::cerr << "\tusage: playerspeed [speed : int]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			try
			{
				mapinfo->playerSpeed = std::stof(tokens[1]);
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			continue;
		}

		if (tokens[0] == "playerhitbox" && mapinfo)
		{
			if (tokens.size() != 3)
			{
				std::cerr << "failed to load map: incorrect usage of playerhitbox" << std::endl;
				std::cerr << "\tusage: playerhitbox [width : float] [height : float]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;
				
				file.close();
				return false;
			}

			try
			{
				mapinfo->playerHitbox = vec2(std::stof(tokens[1]), std::stof(tokens[2]));
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			continue;
		}

		if (tokens[0] == "playerhealth" && mapinfo)
		{
			if (tokens.size() != 2)
			{
				std::cerr << "failed to load map: incorrect usage of zombiehealth" << std::endl;
				std::cerr << "\tusage: zombiehealth [health : float]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			try
			{
				mapinfo->playerHealth = std::stof(tokens[1]);
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}
		}

		if (tokens[0] == "zombiehitbox" && mapinfo)
		{
			if (tokens.size() != 3)
			{
				std::cerr << "failed to load map: incorrect usage of zombiehitbox" << std::endl;
				std::cerr << "\tusage: zombiehitbox [width : float] [height : float]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			try
			{
				mapinfo->zombieHitbox = vec2{ std::stof(tokens[1]), std::stof(tokens[2]) };
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			continue;
		}

		if (tokens[0] == "zombiespeed" && mapinfo)
		{
			if (tokens.size() != 2)
			{
				std::cerr << "failed to load map: incorrect usage of zombiespeed" << std::endl;
				std::cerr << "\tusage: zombiespeed [speed : float]"<< std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			try
			{
				mapinfo->zombieSpeed = std::stof(tokens[1]);
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}
		}

		if (tokens[0] == "zombiespawn" && mapinfo)
		{
			if (tokens.size() != 3)
			{
				std::cerr << "failed to load map: incorrect usage of zombiespawn" << std::endl;
				std::cerr << "\tusage: zombiespawn [x : float] [y : float]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			try
			{
				mapinfo->zombieSpawns.push_back(vec2{ std::stof(tokens[1]), std::stof(tokens[2]) });
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			continue;
		}

		if (tokens[0] == "zombiehealth" && mapinfo)
		{
			if (tokens.size() != 2)
			{
				std::cerr << "failed to load map: incorrect usage of zombiehealth" << std::endl;
				std::cerr << "\tusage: zombiehealth [health : float]" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;
				
				file.close();
				return false;
			}

			try
			{
				mapinfo->zombieHealth = std::stof(tokens[1]);
			}
			catch (const std::exception& ex)
			{
				std::cerr << "failed to load map: cast failed" << std::endl;
				std::cerr << "\terror: " << ex.what() << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}
		}

		if (tokens[0] == "MAPDATASTART")
		{
			if (!dimensionsSet)
			{
				std::cerr << "failed to load map: dimension have not been set" << path << std::endl;
				std::cerr << "\tusage: MAPDATASTART" << std::endl;
				std::cerr << "\t[map data]..." << std::endl;
				std::cerr << "\tMAPDATAEND" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			mapDataRead = true;
			continue;
		}

		if (tokens[0] == "MAPDATAEND")
		{
			if (!mapDataRead)
			{
				std::cerr << "failed to load map: MAPDATAEND without MAPDATA START" << std::endl;
				std::cerr << "\tusage: MAPDATASTART" << std::endl;
				std::cerr << "\t[map data]..." << std::endl;
				std::cerr << "\tMAPDATAEND" << std::endl;
				std::cerr << "\tin file '" << path << "' on line " << line << std::endl;
				std::cerr << "\t" << curLine << std::endl;

				file.close();
				return false;
			}

			mapDataRead = false;
			continue;
		}

		if (mapDataRead)
		{
			for (int j = 0; j < mCols; j++)
			{
				mSpriteMap[j][i] = std::stoi(tokens[j]);
			}
			i++;
			continue;
		}
	}

	if (mapDataRead)
	{
		std::cerr << "warning: didnt have matching MAPDATAEND" << std::endl;
	}

	file.close();

	mInit = true;

	return true;
}

bool Map::ExportMap(const char* dstpath, const char* sprsheetpath)
{
	std::ofstream outfile(dstpath);

	if (!outfile.is_open())
	{
		std::cout << "failed to create file at file path " << dstpath << std::endl;
		return false;
	}

	outfile << "# ssm file generated by spritesheetmaker" << std::endl << std::endl;

	outfile << "dim " << mCols << " " << mRows << std::endl;
	outfile << "sspath " << sprsheetpath << std::endl << std::endl;
	

	outfile << "MAPDATASTART" << std::endl;

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			outfile << mSpriteMap[j][i] << " ";
		}
		outfile << std::endl;
	}

	outfile << "MAPDATAEND" << std::endl << std::endl;

	outfile.close();

	return true;
}

bool Map::Ready() const
{
	return mInit;
}

int Map::GetRows() const
{
	return mRows;
}

int Map::GetCols() const
{
	return mCols;
}

void Map::Free()
{
	for (std::vector<int>& arr : mSpriteMap)
	{
		arr.clear();
	}
	mSpriteMap.clear();

	mInit = false;
}

std::vector<int>& Map::operator[](unsigned int col)
{
	if (col > mCols - 1)
	{
		std::cerr << "Out of range." << std::endl;
	}
	return mSpriteMap[col];
}
