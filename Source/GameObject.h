#pragma once

#include "SFML/Graphics.hpp"

class GameObject 
{	
public:
	GameObject();
	~GameObject();
	void SetPosition(float x, float y);
	void SetTexture(sf::Texture& texture);
	void DrawSprite(sf::RenderWindow& window);

	void Move(float deltaTime);
	bool CheckCollisionWith(GameObject& ref);

	sf::Sprite sprite;
	sf::Vector2f velocity;

	float radius = 0;
	bool worldWrapped = false;
private:
	void WorldWrap();
};