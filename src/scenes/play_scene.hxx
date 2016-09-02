/**
 * @file play_scene.hxx
 * @desc Playing scene header.
 */
#ifndef SCENES_PLAY_SCENE_HXX
#define SCENES_PLAY_SCENE_HXX

#include "..\game\common.hxx"

/**
 * Playing game scene.
 */
class PlayScene : public Scene
{
public:
	virtual ~PlayScene();
	PlayScene();

protected:
	void init();
	void fini();

private:
	Sprite *spriteMap;
	Sprite *spritePacman;
	Map map;
	Divo movDivoes[4];
	Pacman movHero;
	ULONGLONG timeStart;

public:
	virtual void handleActivate(HWND hwnd, bool active);
	virtual bool handleKey(HWND hwnd, WPARAM key);
	virtual void render();
};

#endif // SCENES_PLAY_SCENE_HXX

