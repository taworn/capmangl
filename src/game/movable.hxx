/**
 * @file movable.hxx
 * @desc Movable header.
 */
#ifndef GAME_MOVABLE_HXX
#define GAME_MOVABLE_HXX

class Sprite;
class Animation;
class Map;

/**
 * A movable class.
 */
class Movable
{
public:
	static const int TIME_PER_ANI_FRAME = 250;

	/**
	 * Destructs the movable.
	 */
	~Movable();

	/**
	 * Constructs the movable.
	 */
	Movable();

	/**
	 * Sets map.  Used to bind Movable with Map.
	 */
	virtual void setMap(Map *map) = 0;

	/**
	 * Moves with direction, use constant Map::MOVE_*.
	 */
	void move(int direction);

	/**
	 * After move animation completed, it's call this function.
	 */
	virtual void nextMove();

	/**
	 * Moves with direction, use constant Map::MOVE_*.
	 */
	void play(ULONGLONG timeUsed);

	/**
	 * Draws movable.
	 */
	void draw(Sprite *sprite, const glm::mat4 *viewProjectMatrix, const glm::mat4 *scaleMatrix);

	// Gets X, Y position
	int getX() const { return point.x; }
	int getY() const { return point.y; }

	// Animation class' functions
	float getCurrentX() const { return animation.getCurrentX(); }
	float getCurrentY() const { return animation.getCurrentY(); }
	float getVelocityX() const { return animation.getVelocityX(); }
	float getVelocityY() const { return animation.getVelocityY(); }

protected:
	POINT point;
	bool walking;
	float distance;
	float target;
	int currentDirection;
	int nextDirection;
	ULONGLONG timePerDistance;
	ULONGLONG timeUsed;
	Animation animation;
	Map *map;

private:
	Movable(const Movable&);
	Movable& operator=(const Movable&);
};

#endif // GAME_MOVABLE_HXX

