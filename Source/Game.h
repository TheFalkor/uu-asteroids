#pragma once

#include "SFML/Graphics.hpp"
#include "Player.h"
#include "UFO.h"
#include "Asteroid.h"
#include <vector>
#include "SFML/Audio.hpp"

class Game
{
public:
	Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title);
	~Game();
	/// Return Value
	/// true = Continue to run program
	/// false = Quits program
	bool Update(float deltaTime); 
	void Draw(sf::RenderWindow& window);
private:
	void CheckInput();
	void IncreaseScore(int pts);
	void WaveController();
	void EndGame();

	int score = 0;
	int wave = 0;
	float ufoRespawnCooldown = 0;
	float curUfoCooldown = 0;
	bool gameActive = true;

	Player player;
	UFO ufo;
	std::vector<Asteroid> asteroids;

	sf::Texture playerTexture;
	sf::Texture bulletTexture;
	sf::Texture asteroidTexture[4];
	sf::Texture ufoTexture;

	sf::Texture heartTexture;

	sf::Font font;
	sf::Text scoreText;
	sf::Text waveText;

	sf::Text gameoverText;

	sf::SoundBuffer teleportBuffer;
	sf::SoundBuffer thrustBuffer;
	sf::SoundBuffer destroyBuffer;
	sf::SoundBuffer shootBuffer;

	sf::Sound destroySound;
};