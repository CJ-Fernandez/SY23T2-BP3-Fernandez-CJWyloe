#include "Player.h"
#include "Scene.h"

Player::~Player()
{
	// Memory manage our bullets. Delete all bullets on player 
	for (int i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}
	bullets.clear();
}

void Player::start()
{
	// Load texture
	texture = loadTexture("gfx/player.png");

	// Initialize to avoid garbage values
	x = 100;
	y = 100;
	width = 0;
	height = 0;
	speed = 2;
	boost = 3;
	reloadTimeF = 8; // Reload time of 8 frames is 0.13 seconds
	reloadTimeG = 16; // Reload time of 16 frames is 0.26
	currentReloadTime = 0;

	// Query to texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
}

void Player::update()
{
	if (app.keyboard[SDL_SCANCODE_W])
	{
		y -= speed;
	}

	if (app.keyboard[SDL_SCANCODE_S])
	{
		y += speed;
	}

	if (app.keyboard[SDL_SCANCODE_A])
	{
		x -= speed;
	}

	if (app.keyboard[SDL_SCANCODE_D])
	{
		x += speed;
	}
	if (app.keyboard[SDL_SCANCODE_LSHIFT])
	{
		speed += boost;
	}

	if (app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		speed -= boost;

		if (speed < 2)
		{
			speed = 2;
		}
	}

	// Decrement player's reload timer
	if (currentReloadTime > 0)
		currentReloadTime--;

	if (app.keyboard[SDL_SCANCODE_F] && currentReloadTime == 0)
	{
		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet(x + width, y - 2 + height / 2, 1, 0, 10);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);
		bullet->start();

		// After firing, reset our reload timer
		currentReloadTime = reloadTimeF;
	}

	//Side cannons
	if (app.keyboard[SDL_SCANCODE_G] && currentReloadTime == 0)
	{
		SoundManager::playSound(sound);
		// Bullet from the right end
		Bullet* rightBullet = new Bullet(x, y + height, 1, 0, 10);
		bullets.push_back(rightBullet);
		getScene()->addGameObject(rightBullet);

		// Bullet from the left end
		Bullet* leftBullet = new Bullet(x, y, 1, 0, 10);
		bullets.push_back(leftBullet);
		getScene()->addGameObject(leftBullet);
		leftBullet->start();

		// After firing, reset our reload timer
		currentReloadTime = reloadTimeG;
	}

	// memory manage our bullets, when they go off the screen, delete them
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() > SCREEN_WIDTH)
		{
			// Cache the variable so we can delete it later
			// We can't delete it after erasing from the vector (leaked pointer)
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;

			// cant change our vector while looping inside it
			//might crash on the next loop iteration
			// to counter that, we delete one bullet per frame
			// we can also avoid lag this way
			break; 
		}
	}

}

void Player::draw()
{
	blit(texture, x, y);
}

int Player::getPositionX()
{
	return x;
}

int Player::getPositionY()
{
	return y;
}
