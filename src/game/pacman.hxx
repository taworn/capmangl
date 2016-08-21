/**
 * @file pacman.hxx
 * @desc Pacman header.
 */
#ifndef GAME_PACMAN_HXX
#define GAME_PACMAN_HXX

#include "movable.hxx"

/**
 * A pacman class.
 */
class Pacman : public Movable
{
public:
	/**
	 * Destructs the pacman.
	 */
	~Pacman();

	/**
	 * Constructs the pacman.
	 */
	Pacman();

	/**
	 * Sets map.
	 */
	virtual void setMap(Map *map);

private:
	Pacman(const Pacman&);
	Pacman& operator=(const Pacman&);
};

#endif // GAME_PACMAN_HXX

