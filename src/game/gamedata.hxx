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
	 * Destructs the game data.
	 */
	~GameData();

	/**
	 * Constructs the game data.
	 */
	GameData();

	/**
	 * Resets all game data.
	 */
	void reset();

	/**
	 * Clears data.
	 */
	void clear();

	/**
	 * Saves data.
	 */
	void save();

	/**
	 * Gets current score.
	 */
	int getScore() const { return score; }

	/**
	 * Gets current stage.
	 */
	int getStage() const { return stage; }

	/**
	 * Advances to next stage.
	 * @return Returns true if next stage, otherwise, it is false and win the game.
	 */
	bool nextStage();

	/**
	 * Decrease divo life by one.
	 */
	void divoLifeDecrease()
	{
		divoLife--;
	}

	/**
	 * Checks divo can respawn.
	 */
	bool divoCanRelife()
	{
		return divoLife > 0;
	}

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

	/**
	 * Checks that all divoes dead.
	 */
	bool checkAllDivoDead();

	/**
	 * Retrieves bonus after get item.
	 */
	void getBonus(int item);

	/**
	 * Checks current time is reverse mode.
	 */
	bool isReverseMode() { return reverseMode; }

	/**
	 * Updates current time frame.
	 */
	void update(ULONGLONG timeUsed);

private:
	int score;
	int stage;
	bool reverseMode;
	ULONGLONG reverseTime;
	int divoLife;
	std::vector<Divo*> divoList;

	const ULONGLONG maxReverseTime = 2500;

	GameData(const GameData&);
	GameData& operator=(const GameData&);

	static GameData *singleton;
};

#endif // GAME_GAMEDATA_HXX

