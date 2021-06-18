#pragma once
#include "Entity.h"
#include "input.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "camera.h"
#include <cmath>



class Stage {
public:
	virtual void init() {};
	virtual void init(int window_width, int window_height) {};
	virtual void render(float time) {}
	virtual void update(float seconds_elapsed) {}
};

class InitStage : public Stage {
public:
	virtual void init(int window_width, int window_height);
	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};
class ControlStage : public Stage {
public:
	
	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};
class MenuStage : public Stage {
public:
	virtual void init(int window_width, int window_height);

	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};
class MenuStage2 : public Stage {
public:
	

	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};
class LoadStage : public Stage {
public:
	virtual void init(int window_width, int window_height);

	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};

class PlayStage : public Stage {
public:
	virtual void init(int window_width, int window_height);
	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};
class EagleStage : public Stage {
public:
	virtual void init() {

	}

	virtual void render(float time);
	virtual void update(float seconds_elapsed);
};
class VictoryStage : public Stage {
public:
	

	virtual void render(float time);
	
};
class LoseStage : public Stage {
public:
	

	virtual void render(float time);
	
};