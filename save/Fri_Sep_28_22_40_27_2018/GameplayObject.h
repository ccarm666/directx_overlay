#ifndef GAMEPLAYOBJECT_H
#define GAMEPLAYOBJECT_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "GameSprite.h"
#include <string>

enum class ObjectStatus{ Active, Dying, Dead };

class GameplayObject
{
public:
	GameplayObject(float x, float y, float rotation, float speed, float maxSpeed);
	~GameplayObject();

//	virtual bool Initialize(LPDIRECT3DDEVICE9 device, std::string file, int width, int height);
	virtual bool Initialize(LPDIRECT3DDEVICE9 device, GameSprite *sp, int width, int height);
	virtual void Update(float gameTime);
	virtual void Draw(float gameTime);
	virtual void SetDrawStatus(bool doIt);
	ObjectStatus GetStatus() const;
	bool drawIt;
	void SetSpeed(float speed);
	void SetSprite(GameSprite *sp);
protected:
	//Informational Data (Name, Description, ...)
	std::string Name;
	std::string Description;

	//Physics Data
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	float rotation;
	float speed;
	
private:
	//Status Data
	ObjectStatus status;

	//Graphics Data
	GameSprite *sprite;

	//Physics Data
	float maxSpeed;
};

#endif /* GAMEPLAYOBJECT_H */