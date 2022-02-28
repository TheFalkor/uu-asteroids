#include "Bullet.h"
#define _USE_MATH_DEFINES
#include <math.h>

Bullet::Bullet()
{
}

Bullet::Bullet(float xPosition, float yPosition, float angle, float speedPercentage)
{
	sprite.setScale(0.15f, 0.15f);
	SetPosition(xPosition, yPosition);

	sprite.setRotation(angle);
	

	float rot = sprite.getRotation();
	float xFactor = cos(rot * M_PI / 180);
	float yFactor = sin(rot * M_PI / 180);
	
	velocity = sf::Vector2f(xFactor * 400 * speedPercentage, yFactor * 400 * speedPercentage);
	radius = 3;	//temporary
}

Bullet::~Bullet()
{
}

bool Bullet::Update(float deltaTime)
{
	lifetime += deltaTime;
	Move(deltaTime);

	if (lifetime >= 2 && worldWrapped)
		return false;

	return true;
}

