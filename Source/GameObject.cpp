#include "GameObject.h"


GameObject::GameObject()
{
	sprite.setOrigin(sf::Vector2f(32, 32));
	velocity = sf::Vector2f(0, 0);
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition(float x, float y)
{
	sprite.setPosition(x, y);
}

void GameObject::SetTexture(sf::Texture& texture)
{
	sprite.setTexture(texture);
}

void GameObject::DrawSprite(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void GameObject::Move(float deltaTime)
{
	sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);

	WorldWrap();
}

bool GameObject::CheckCollisionWith(GameObject& ref)
{
	float xDistance = ref.sprite.getPosition().x - sprite.getPosition().x;
	float yDistance = ref.sprite.getPosition().y - sprite.getPosition().y;

	float diaDistance = sqrt(pow(xDistance, 2) + pow(yDistance, 2));

	if (diaDistance <= ref.radius + radius)
		return true;

	return false;
}

void GameObject::WorldWrap()
{
	worldWrapped = false;
	float x = sprite.getPosition().x;
	float y = sprite.getPosition().y;

	if (x < 0)
		sprite.setPosition(640, y);

	if (x > 640)
		sprite.setPosition(0, y);

	if (y < 0)
		sprite.setPosition(x, 480);

	if (y > 480)
		sprite.setPosition(x, 0);

	if (x != sprite.getPosition().x || y != sprite.getPosition().y)
		worldWrapped = true;
}