#include "Asteroid.h"
#include <random>

Asteroid::Asteroid()
{
}

Asteroid::Asteroid(sf::Texture& texture)
{
	SetTexture(texture);
	float xPos = rand() % 640;
	float yPos = rand() % 480;

	SetPosition(xPos, yPos);

	float xDir = (rand() % 2000 - 1000) / 1000.0f;
	float yDir = (rand() % 2000 - 1000) / 1000.0f;

	float speed = rand() % 100 + 100;

	velocity = sf::Vector2f(xDir * speed, yDir * speed);
	float scaleFactor = (size / 3.0f) * 0.75f;

	sprite.setScale(scaleFactor, scaleFactor);
	radius = 28 * (size / 3.0f);

	health = 2;
}

Asteroid::Asteroid(sf::Texture& texture, int size, int x, int y, sf::Vector2f velocity)
{
	SetTexture(texture);
	SetPosition(x, y);

	float xIncrease = rand() % 100 - 50;
	float yIncrease = rand() % 100 - 50;

	velocity.x += xIncrease;
	velocity.y += yIncrease;

	float speed = rand() % 150 + 100;
	this->velocity = velocity;
	this->size = size;

	float scaleFactor = (size / 3.0f) * 0.9f;

	sprite.setScale(scaleFactor, scaleFactor);
	radius = 32 * (size / 3.0f);

	health = 1;
}



Asteroid::~Asteroid()
{

}

void Asteroid::Update(float deltaTime)
{
	Move(deltaTime);
}
