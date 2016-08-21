/**
 * @file divo.hxx
 * @desc Divo header.
 */
#ifndef GAME_DIVO_HXX
#define GAME_DIVO_HXX

#include "movable.hxx"

/**
 * A divo class.
 */
class Divo : public Movable
{
public:
	/**
	 * Destructs the divo.
	 */
	~Divo();

	/**
	 * Constructs the divo.
	 */
	Divo();

	/**
	 * Sets divo identifer, just used to distint animation's set.
	 */
	void setId(int divoId);

	/**
	 * Sets map.
	 */
	virtual void setMap(Map *map);

	/**
	 * After move animation completed, it's call this function.
	 */
	virtual void nextMove();

	/**
	 * Checks whether divo is walking or stand still.
	 */
	bool isIdle() const { return !walking; }

private:
	Divo(const Divo&);
	Divo& operator=(const Divo&);
};

#endif // GAME_DIVO_HXX

