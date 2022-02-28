#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "Player.h"

class UFO : public GameObject
{
public:
	UFO();
	UFO(sf::Texture& UFOTexture, sf::Texture& bulletTexture, Player& player, sf::SoundBuffer& shootBuffer);
	~UFO();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
	void Shoot();
	bool BulletCollision(GameObject& ref);
	bool Hit();
	void Respawn();
	
	bool dead = true;
private:
	int health = 3;
	float burstCooldown = 2;
	float shootCooldown = 0.25f;
	int shotCount = 0;
	std::vector<Bullet> bulletList;
	sf::Texture* bulletTexture = nullptr;
	Player* player = nullptr;

	sf::Sound shootSound;
};