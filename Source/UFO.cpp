#include "UFO.h"
#define _USE_MATH_DEFINES
#include <math.h>

UFO::UFO()
{
}

UFO::UFO(sf::Texture& UFOTexture, sf::Texture& bulletTexture, Player& player, sf::SoundBuffer& shootBuffer)
{
	this->bulletTexture = &bulletTexture;
	this->player = &player;
	SetTexture(UFOTexture);

	float y = rand() % 480;
	SetPosition(0, y);

	velocity = sf::Vector2f(100, 0);
	radius = 32;	//mayeb change

	shootSound.setBuffer(shootBuffer);
}

UFO::~UFO()
{
}

void UFO::Update(float deltaTime)
{
	if (!dead)
	{
		Move(deltaTime);

		burstCooldown -= deltaTime;

		if (burstCooldown <= 0)
		{
			shootCooldown -= deltaTime;

			if (shootCooldown <= 0)
			{
				shootCooldown = 0.25f;
				shotCount += 1;

				Shoot();

				if (shotCount == 3)
				{
					burstCooldown = 2;
					shotCount = 0;
				}
			}
		}
	}


	for (int i = 0; i < bulletList.size(); i++)
	{
		if (!bulletList[i].Update(deltaTime))
			bulletList.erase(bulletList.begin() + i);
	}
}

void UFO::Draw(sf::RenderWindow& window)
{
	if(!dead)
		DrawSprite(window);

	for (int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i].DrawSprite(window);
	}
}

void UFO::Shoot()
{
	float deltaX = player->sprite.getPosition().x - sprite.getPosition().x;
	float deltaY = player->sprite.getPosition().y - sprite.getPosition().y;

	float angle = atan2(deltaY, deltaX) * (180.0f / M_PI);

	bulletList.push_back(Bullet(sprite.getPosition().x, sprite.getPosition().y, angle, 0.5f));
	bulletList.back().SetTexture(*bulletTexture);

	shootSound.play();
}

bool UFO::BulletCollision(GameObject& ref)
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].CheckCollisionWith(ref))
		{
			bulletList.erase(bulletList.begin() + i);
			return true;
		}
	}

	return false;
}

bool UFO::Hit()
{
	if (dead)
		return false;

	if (--health == 0)
	{
		dead = true;
	}

	return dead;
}

void UFO::Respawn()
{
	dead = false;
	health = 3;
	float burstCooldown = 2;
	float shootCooldown = 0.25f;
	int shotCount = 0;

	float y = rand() % 480;
	SetPosition(0, y);
}
