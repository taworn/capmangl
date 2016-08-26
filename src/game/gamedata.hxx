/**
 * @file gamedata.hxx
 * @desc Game data header.
 */
#ifndef GAME_GAMEDATA_HXX
#define GAME_GAMEDATA_HXX

/**
 * A GameData class.
 */
class GameData
{
public:
	static GameData* instance() { return singleton; }

	/**
	 * Destructs the gamedata.
	 */
	~GameData();

	/**
	 * Constructs the gamedata.
	 */
	GameData();

	/**
	 * Clears data.
	 */
	void clear();

	/**
	 * Adds divo to list.
	 */
	void addDivo(Divo *divo)
	{
		divoList.push_back(divo);
	}

	/**
	 * Gets divo in list.
	 */
	Divo* getDivo(int id)
	{
		return divoList[id];
	}

	/**
	 * Gets number of divoes in list.
	 */
	int getDivoCount()
	{
		return (int)divoList.size();
	}

private:
	std::vector<Divo*> divoList;

	GameData(const GameData&);
	GameData& operator=(const GameData&);

	static GameData *singleton;
};

#endif // GAME_GAMEDATA_HXX

