#pragma once

#include "GameObject.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Bullet.h"
#include "Asteroid.h"

enum class EngineState
{
	DEFAULT		= 0,
	REVERSE		= 1,
	DRIVE		= 2
};

class Player : public GameObject
{
public:
	Player();
	Player(sf::Texture& playerTexture, sf::Texture& bulletTexture, sf::SoundBuffer& thrustBuffer, sf::SoundBuffer& teleportBuffer, sf::SoundBuffer& shootBuffer, sf::SoundBuffer& destroyBuffer);
	~Player();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
	void Rotate(float deltaTime);
	void UpdateVelocity();
	void Teleport(std::vector<Asteroid>& asteroidList);
	void Shoot();
	bool BulletCollision(GameObject& ref);
	void Die();

	int rotateSum = 0;

	int health = 5;
	EngineState engine = EngineState::DEFAULT;

private:
	float rotateVelocity = 360;
	float forwardAcc = 4;
	float reverseAcc = 2;
	float velocityCap = 300;

	float factorX = 0;
	float factorY = -1;

	int immuneTimerMS = 0;

	float shootCooldown = 0;
	float teleportCooldown = 0;
	std::vector<Bullet> bulletList;
	sf::Texture* bulletTexture = nullptr;

	sf::Sound shootSound;
	sf::Sound teleportSound;
	sf::Sound thrustSound;
	sf::Sound destroySound;
};