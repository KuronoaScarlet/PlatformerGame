#ifndef __SCENE3_H__
#define __SCENE3_H__

#include "Module.h"
#include "Animation.h"

class GuiControl;

class Scene3 : public Module
{
public:

	Scene3();

	// Destructor
	virtual ~Scene3();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);


	//Fonts
	int scoreFont = -1;
	char scoreText[10] = { "\0" };

	bool firstEntry = true;
	bool on;
	
	


private:
};


#endif // __SCENE1_H__