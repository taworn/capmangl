/**
 * @file map.hxx
 * @desc Map header.
 */
#ifndef GAME_MAP_HXX
#define GAME_MAP_HXX

#include <vector>
#include <glm/glm.hpp>

class Movable;

/**
 * A map class.
 */
class Map
{
public:
	static const int MOVE_LEFT = 1;
	static const int MOVE_RIGHT = 2;
	static const int MOVE_UP = 4;
	static const int MOVE_DOWN = 8;

	/**
	 * Destructs the map.
	 */
	~Map();

	/**
	 * Constructs the map.
	 */
	Map();

	/**
	 * Loads map data.
	 */
	bool load();

	/**
	 * Gets start position for Divo.
	 */
	void getDivoStartPosition(POINT *p, POINTFLOAT *pf);

	/**
	 * Gets start position for Pacman.
	 */
	void getPacmanStartPosition(POINT *p, POINTFLOAT *pf);

	/**
	 * Checks if direction is can be pass.
	 */
	bool canMove(Movable *movable, int direction, POINT *p, POINTFLOAT *pf);

	/**
	 * Checks 4-directions which ways can move or not.
	 * @return Bit flags in order: left, right, up, down.
	 */
	int canPreviewMove(Movable *movable);

	/**
	 * Draws map.
	 */
	void draw(Sprite *sprite, const glm::mat4 *viewProjectMatrix, const glm::mat4 *scaleMatrix);

private:
	union MapData {
		struct {
			char image;
			char block;
			char eat;
			char special;
		};
		int data;
	};
	int width, height;
	char *mapData;
	int *imageData;

	std::vector<float> horzBounds;
	std::vector<float> horzPoints;
	std::vector<float> vertBounds;
	std::vector<float> vertPoints;

	POINT startDivo;
	POINT startPacman;

	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

