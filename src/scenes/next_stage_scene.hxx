/**
 * @file next_stage_scene.hxx
 * @desc Next stage scene header.
 */
#ifndef SCENES_NEXT_STAGE_SCENE_HXX
#define SCENES_NEXT_STAGE_SCENE_HXX

/**
 * Next stage scene.
 */
class NextStageScene : public Scene
{
public:
	virtual ~NextStageScene();
	NextStageScene();

protected:
	void init();
	void fini();

public:
	virtual void render();
};

#endif // SCENES_NEXT_STAGE_SCENE_HXX

