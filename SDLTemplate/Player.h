#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include <vector>

class Player : public GameObject
{
public:
	~Player();
	void start();
	void update();
	void draw();

	// Getter and Setter for reload times
	float getReloadTimeF() const { return reloadTimeF; }
	void setReloadTimeF(float value) { reloadTimeF = value; }

	float getReloadTimeG() const { return reloadTimeG; }
	void setReloadTimeG(float value) { reloadTimeG = value; }

	int getPositionX();
	int getPositionY();

private:
	SDL_Texture* texture;
	Mix_Chunk* sound;
	int x;
	int y;
	int width;
	int height;
	int speed;
	int boost;
	float reloadTimeF;
	float reloadTimeG;
	float reloadTime;
	float currentReloadTime;
	std::vector<Bullet*> bullets;
};

