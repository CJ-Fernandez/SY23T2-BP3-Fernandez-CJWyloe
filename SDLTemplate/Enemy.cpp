#include "Enemy.h"
#include "Scene.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::start()
{
	// Load texture
	texture = loadTexture("gfx/enemy.png");

	// Initialize to avoid garbage values
	directionX = -1;
	directionY = 1;
	width = 0;
	height = 0;
	speed = 2;
	reloadTime = 60; // Reload time of 60 frames is 1 seconds
	currentReloadTime = 0;
	directionChangeTime = (rand() % 300) + 180; // Direction change time of 3-8 seconds
	currentDirectionChangeTime = 0;

	// Query to texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
}

void Enemy::update()
{
	// Move
	x += directionX * speed;
	y += directionY * speed;

	// Basic AI, switch directions every X seconds
	if (currentDirectionChangeTime > 0)
		currentDirectionChangeTime--;

	if (currentDirectionChangeTime == 0)
	{
		// Flip Directions
		directionY = -directionY;
		currentDirectionChangeTime = directionChangeTime;
	}

	// Boundary check to prevent enemies from going off the screen vertically
	if (y < 0)
	{
		y = 0;
		directionY = 1; // Change direction to move down
	}
	else if (y > SCREEN_HEIGHT - height)
	{
		y = SCREEN_HEIGHT - height;
		directionY = -1; // Change direction to move up
	}

	// Decrement enemy's reload timer
	if (currentReloadTime > 0)
		currentReloadTime--;

	// Only fire when our reload time is ready
	if (currentReloadTime == 0)
	{
		float dx = -1;
		float dy = 0;

		calcSlope(playerTarget->getPositionX(), playerTarget->getPositionY(), x, y, &dx, &dy);

		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet(x + width, y - 2 + height / 2, dx, dy, 10);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);
		bullet->start();

		// After firing, reset our reload timer
		currentReloadTime = reloadTime;
	}

	// memory manage our bullets, when they go off the screen, delete them
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() < 0)
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

void Enemy::draw()
{
	blit(texture, x, y);
}

void Enemy::setPlayerTarget(Player* player)
{
	playerTarget = player;
}

void Enemy::setPosition(int xPos, int yPos)
{
	this->x = xPos;
	this->y = yPos;
}
