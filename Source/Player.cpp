#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>


Player::Player()
{
}

Player::Player(sf::Texture& playerTexture, sf::Texture& bulletTexture, sf::SoundBuffer& thrustBuffer, sf::SoundBuffer& teleportBuffer, sf::SoundBuffer& shootBuffer, sf::SoundBuffer& destroyBuffer)
{
	this->bulletTexture = &bulletTexture;
	SetTexture(playerTexture);

	SetPosition(320, 240);
	sprite.setRotation(270);
	sprite.setScale(0.5f, 0.5f);

	radius = 16;

	teleportSound.setBuffer(teleportBuffer);
	thrustSound.setBuffer(thrustBuffer);
	shootSound.setBuffer(shootBuffer);
	destroySound.setBuffer(destroyBuffer);
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	Rotate(deltaTime);
	UpdateVelocity();
	Move(deltaTime);

	if(immuneTimerMS > 0)
	{
		immuneTimerMS -= deltaTime * 1000;

		if (immuneTimerMS < 0)
			immuneTimerMS = 0;
	}

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (!bulletList[i].Update(deltaTime))
			bulletList.erase(bulletList.begin() + i);
	}

	if (shootCooldown > 0)
		shootCooldown -= deltaTime;

	if (teleportCooldown > 0)
		teleportCooldown -= deltaTime;
}

void Player::Draw(sf::RenderWindow& window)
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i].DrawSprite(window);
	}

	if(immuneTimerMS%300 < 150)
		DrawSprite(window);
}

void Player::Rotate(float deltaTime)
{
	if (rotateSum != 0)
	{
		float rot = sprite.getRotation() + rotateSum * rotateVelocity * deltaTime;
		sprite.setRotation(rot);
		factorX = cos(rot * M_PI / 180);
		factorY = sin(rot * M_PI / 180);
	}
}

void Player::UpdateVelocity()
{
	if (engine == EngineState::DRIVE)
	{
		if(thrustSound.getStatus() != sf::SoundSource::Status::Playing)
			thrustSound.play();
		velocity.x += factorX * forwardAcc;
		velocity.y += factorY * forwardAcc;
	}
	else if (engine == EngineState::REVERSE)
	{
		if (thrustSound.getStatus() != sf::SoundSource::Status::Playing)
			thrustSound.play();
		velocity.x -= factorX * reverseAcc;
		velocity.y -= factorY * reverseAcc;
	}
	else if(engine == EngineState::DEFAULT)
	{
		thrustSound.stop();
		float xSlower = velocity.x / 100;
		velocity.x -= xSlower;

		float ySlower = velocity.y / 100;
		velocity.y -= ySlower;
	}

	if (velocity.x > velocityCap)
		velocity.x = velocityCap;

	if (velocity.y > velocityCap)
		velocity.y = velocityCap;

	if (velocity.x < -velocityCap)
		velocity.x = -velocityCap;

	if (velocity.y < -velocityCap)
		velocity.y = -velocityCap;
}

void Player::Teleport(std::vector<Asteroid>& asteroidList)
{
	if (teleportCooldown > 0)
		return;

	bool incorrectLocation = true;

	while (incorrectLocation)
	{
		incorrectLocation = false;
		float x = rand() % 640;
		float y = rand() % 480;

		SetPosition(x, y);

		for (int i = 0; i < asteroidList.size(); i++)
		{
			if (asteroidList[i].CheckCollisionWith(*this))
			{
				incorrectLocation = true;
			}
		}
	}

	teleportSound.play();
	teleportCooldown = 5;
}

void Player::Shoot()
{
	if (shootCooldown > 0)
		return;

	bulletList.push_back(Bullet(sprite.getPosition().x, sprite.getPosition().y, sprite.getRotation(), 1));
	bulletList.back().SetTexture(*bulletTexture);

	shootSound.play();
	shootCooldown = 0.3f;
}

bool Player::BulletCollision(GameObject& ref)
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].CheckCollisionWith(ref) && bulletList[i].lifetime > 0.1f)
		{
			bulletList.erase(bulletList.begin() + i);
			return true;
		}
	}

	return false;
}

void Player::Die()
{
	if (immuneTimerMS != 0)
		return;

	destroySound.play();
	health--;
	sprite.setPosition(320, 240);
	sprite.setRotation(270);
	factorX = 0;
	factorY = -1;
	velocity = sf::Vector2f(0, 0);
	immuneTimerMS = 2000;
}
