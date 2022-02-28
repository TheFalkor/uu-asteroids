// Henrik Nilsson 2021-01-15
#include "Game.h"
#include <random>
#include <time.h>

Game::Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title)
{
	screenWidth = 640;
	screenHeight = 480;
	title = "Asteroids";
	srand(time(0));

	if (!playerTexture.loadFromFile("../Assets/Sprites/PlayerShip.png"))
		system("pause");

	if (!ufoTexture.loadFromFile("../Assets/Sprites/UFO.png"))
		system("pause");
	
	if (!bulletTexture.loadFromFile("../Assets/Sprites/Bullet.png"))
		system("pause");

	for (int i = 0; i < 4; i++)
	{
		if (!asteroidTexture[i].loadFromFile("../Assets/Sprites/Asteroid_" + std::to_string(i) + ".png"))
			system("pause");
	}


	if (!heartTexture.loadFromFile("../Assets/Sprites/UI_Heart.png"))
		system("pause");


	if (!font.loadFromFile("../Assets/Font/arial.ttf"))
		system("pause");


	if (!teleportBuffer.loadFromFile("../Assets/Audio/Teleport.wav"))
		system("pause");

	if (!thrustBuffer.loadFromFile("../Assets/Audio/Thrust.wav"))
		system("pause");

	if (!destroyBuffer.loadFromFile("../Assets/Audio/Destroy.wav"))
		system("pause");

	if (!shootBuffer.loadFromFile("../Assets/Audio/Shoot.wav"))
		system("pause");

	player = Player(playerTexture, bulletTexture, thrustBuffer, teleportBuffer, shootBuffer, destroyBuffer);
	ufo = UFO(ufoTexture, bulletTexture, player, shootBuffer);

	scoreText.setFont(font);
	scoreText.setPosition(50, 30);
	scoreText.setString(std::to_string(score));

	waveText.setFont(font);
	waveText.setPosition(590, 30);

	gameoverText.setFont(font);
	gameoverText.setCharacterSize(50);
	gameoverText.setPosition(180, 150);
	gameoverText.setString("GAME OVER");

	destroySound.setBuffer(destroyBuffer);

	WaveController();
};

Game::~Game()
{
}

bool Game::Update(float deltaTime)
{
	if (!gameActive)
		return true;

	CheckInput();
	player.Update(deltaTime);
	ufo.Update(deltaTime);

	if (!ufo.dead)
	{
		if (player.BulletCollision(ufo))
		{
			if (ufo.Hit())
			{
				destroySound.play();
				IncreaseScore(250);
			}

		}

		if (ufo.CheckCollisionWith(player))
			player.Die();
	}

	if (player.BulletCollision(player))
		player.Die();

	if (ufo.BulletCollision(player))
		player.Die();

	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i].Update(deltaTime);

		if (asteroids[i].CheckCollisionWith(player))
			player.Die();

		if (player.BulletCollision(asteroids[i]))
			asteroids[i].health -= 1;

		ufo.BulletCollision(asteroids[i]);

		if (asteroids[i].health <= 0)
		{
			destroySound.play();
			IncreaseScore(100 + (3 - asteroids[i].size) * 50);
			int size = asteroids[i].size - 1;


			float x = asteroids[i].sprite.getPosition().x;
			float y = asteroids[i].sprite.getPosition().y;
			sf::Vector2f velocity = asteroids[i].velocity;
			float distance = asteroids[i].radius;

			int factor = 1;
			if (velocity.x < 0 && velocity.y < 0 || velocity.x > 0 && velocity.y > 0)
				factor = -1;

			asteroids.erase(asteroids.begin() + i);

			if (size == 0)
				continue;
			int randomTexture = rand() % 4;
			asteroids.push_back(Asteroid(asteroidTexture[randomTexture], size, x - distance * factor, y - distance, velocity));
			randomTexture = rand() % 4;
			asteroids.push_back(Asteroid(asteroidTexture[randomTexture], size, x + distance * factor, y + distance, velocity));
		}

	}

	if (ufo.dead)
	{
		curUfoCooldown -= deltaTime;
		if (curUfoCooldown <= 0)
		{
			ufo.Respawn();
			curUfoCooldown = ufoRespawnCooldown;
		}
	}


	WaveController();
	if (player.health == 0)
		EndGame();

	return true;
}

void Game::Draw(sf::RenderWindow& window)
{
	if (gameActive)
	{
		for (int i = 0; i < asteroids.size(); i++)
		{
			asteroids[i].DrawSprite(window);
		}
	
		player.Draw(window);
		ufo.Draw(window);

		for (int i = 0; i < player.health; i++)
		{
			float x = 55 + i * 26;
			float y = 65;

			sf::Sprite hp = sf::Sprite(heartTexture);
			hp.setScale(1.5f, 1.5f);
			hp.setPosition(x, y);
			window.draw(hp);
		}

		window.draw(scoreText);
		window.draw(waveText);
	}
	else
	{
		window.draw(gameoverText);
		window.draw(scoreText);
	}

}

void Game::CheckInput()
{
	int rotationSum = 0;
	EngineState engineInput = EngineState::DEFAULT;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		engineInput = EngineState::DRIVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (engineInput == EngineState::DRIVE)
			engineInput = EngineState::DEFAULT;
		else
			engineInput = EngineState::REVERSE;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		rotationSum -= 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rotationSum += 1;
	}

	player.rotateSum = rotationSum;
	player.engine = engineInput;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		player.Shoot();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		player.Teleport(asteroids);
	}
}

void Game::IncreaseScore(int pts)
{
	int prevScore = score;
	score += pts;

	if (score / 1000 > prevScore / 1000)
		player.health += 1;

	scoreText.setString(std::to_string(score));
}

void Game::WaveController()
{
	bool new_wave = false;
	int wave_asteroids = 0;
	if (asteroids.size() == 0 && ufo.dead == true)
	{
		wave += 1;
		new_wave = true;
	}

	switch (wave)
	{
	case 1:
		wave_asteroids = 3;
		ufoRespawnCooldown = 30;
		break;
	case 2:
		wave_asteroids = 4;
		ufoRespawnCooldown = 30;
		break;
	case 3:
		wave_asteroids = 5;
		ufoRespawnCooldown = 25;
		break;
	case 4:
		wave_asteroids = 6;
		ufoRespawnCooldown = 20;
		break;
	case 5:
		wave_asteroids = 7;
		ufoRespawnCooldown = 15;
		break;
	}

	if (wave == 6)
	{
		gameoverText.setString("VICTORY");
		gameActive = false;
	}

	if (new_wave)
	{
		waveText.setString(std::to_string(wave));
		curUfoCooldown = ufoRespawnCooldown;
		for (int i = 0; i < wave_asteroids; i++)
		{
			int randomTexture = rand() % 4;

			asteroids.push_back(Asteroid(asteroidTexture[randomTexture]));
		}
	}
}

void Game::EndGame()
{
	gameActive = false;
	scoreText.setString("SCORE: " + std::to_string(score));
	scoreText.setPosition(230, 220);
}
