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
	bool load(const char *fileName);

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
	 * Checks whether that the floor has item.
	 */
	bool checkAndGetItem(Movable *movable, int *item);

	/**
	 * Draws map.
	 */
	void draw(Sprite *sprite, const glm::mat4 *viewProjectMatrix, const glm::mat4 *scaleMatrix);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int* getImageData() { return imageData; }

private:
	int width, height;
	POINT startDivo;
	POINT startPacman;
	char *mapData;
	int *imageData;
	int itemCount;

	std::vector<float> horzBounds;
	std::vector<float> horzPoints;
	std::vector<float> vertBounds;
	std::vector<float> vertPoints;

	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

