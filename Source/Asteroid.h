#pragma once
#include "GameObject.h"
#include "SFML/Graphics.hpp"

class Asteroid : public GameObject
{
public:
	Asteroid();
	Asteroid(sf::Texture& texture);
	Asteroid(sf::Texture& texture, int size, int x, int y, sf::Vector2f velocity);
	~Asteroid();
	void Update(float deltaTime);

	int health = 2;
	int size = 3;
private:

};