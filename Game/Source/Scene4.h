#ifndef __SCENE4_H__
#define __SCENE4_H__

#include "Module.h"
#include "Animation.h"

class GuiControl;

class Scene4 : public Module
{
public:

	Scene4();

	// Destructor
	virtual ~Scene4();

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