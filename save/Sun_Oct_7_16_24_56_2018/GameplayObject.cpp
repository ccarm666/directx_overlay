#include "GameplayObject.h"

GameplayObject::GameplayObject(float x, float y, float rotation, float speed, float maxSpeed)
{
	float twopi = (float)(M_PI * 2);
	position.x = x;
	position.y = y;
	position.z = 0;
	this->drawIt = true;
	if(rotation > twopi) rotation = twopi;
	if(rotation < 0) rotation = 0;

	if(speed > maxSpeed) speed = maxSpeed;
	else if (speed < 0) speed = 0;

	this->rotation = rotation;
	this->speed = speed;

	velocity.x = cos(rotation) * speed;
	velocity.y = sin(rotation) * speed;
	velocity.z = 0;

	this->maxSpeed = maxSpeed;
}

//bool GameplayObject::Initialize(LPDIRECT3DDEVICE9 device, std::string file, int width, int height)
bool GameplayObject::Initialize(LPDIRECT3DDEVICE9 device, GameSprite *sprite, int width, int height)
{
	status = ObjectStatus::Active;
	this->sprite = sprite;
//	if(!sprite)
//	{
//		sprite = new GameSprite();
//		if(!sprite->Initialize(device, file, width, height))
//			return false;
//	}

	return true;
}

void GameplayObject::Update(float gameTime)
{
	if(status == ObjectStatus::Active)
	{
		position.x += velocity.x;
		position.y += velocity.y;
		position.z = 0;
	}
}

void GameplayObject::Draw(float gameTime)
{
	if(sprite) sprite->Draw(gameTime, position);
}

ObjectStatus GameplayObject::GetStatus() const
{
	return status;
}

void GameplayObject::SetSpeed(float speed)
{
	if(speed >= 0 && speed <= maxSpeed)
	{
		this->speed = speed;

		velocity.x = cos(rotation) * speed;
		velocity.y = sin(rotation) * speed;
		velocity.z = 0;
	}
}

void GameplayObject::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	position.z = 0;
}

GameplayObject::~GameplayObject()
{
	if(sprite)
	{
		delete sprite;
		sprite = 0;
	}
}

void GameplayObject::SetDrawStatus(bool doIt)
{
	this->drawIt = doIt;
}

void  GameplayObject::SetSprite(GameSprite *sp)
{
	sprite = sp;

}