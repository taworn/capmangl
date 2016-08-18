/**
 * @file animation.hxx
 * @desc Animation header.
 */
#ifndef ANIMATION_HXX
#define ANIMATION_HXX

#include <glm/glm.hpp>

class Sprite;

/**
 * An animation class.
 */
class Animation
{
public:
	/**
	 * Destructs an animation.
	 */
	~Animation();

	/**
	 * Constructs an animation.
	 */
	Animation(Sprite *s);

	/**
	 * Sets sprite.
	 */
	void setSprite(Sprite *s);

	/**
	 * Adds a playing animation, only 16 set allow.
	 */
	void add(int number, int start, int end, int time);

	/**
	 * Uses a playing animation.
	 */
	void use(int number);

	/**
	 * Draws animation.
	 */
	void draw(const glm::mat4 &mvpMatrix);

private:
	Sprite *sprite;

	static const int PLAYING_MAX = 16;
	struct PLAYING {
		int start;
		int end;
		int time;
	};
	PLAYING plays[PLAYING_MAX];
	int currentPlaying;
	int currentImage;
	ULONGLONG timeStart;

	Animation(const Animation&);
	Animation& operator=(const Animation&);
};

#endif // ANIMATION_HXX

