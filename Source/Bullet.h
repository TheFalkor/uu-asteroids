#pragma once
#include "GameObject.h"
#include "SFML/Graphics.hpp"

class Bullet : public GameObject
{
public:
	Bullet();
	Bullet(float xPosition, float yPosition, float angle, float speedPercentage);
	~Bullet();
	bool Update(float deltaTime);
	float lifetime = 0;

private:

};