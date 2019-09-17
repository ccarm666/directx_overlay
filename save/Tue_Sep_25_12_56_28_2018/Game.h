#ifndef GAME_H
#define GAME_H

#include "GraphicsDevice.h"
#include "GameplayObject.h"
#include "GameTime.h"
#define MAX_NUMBER_OF_FACES 6
class Game
{
public:
	//Constructors and Destructor
	Game();
	~Game();
	
	//Game Functions
	bool Initialize(HWND hWnd);
	void Run();
	void Update(float gameTime);
	void Draw(float gameTime);
private:
	GraphicsDevice *gDevice;
	GameplayObject *player;
	GameplayObject *player2;
	GameSprite *faces[MAX_NUMBER_OF_FACES + 1]; // index zero is not used
	GameTime *gameTime;
};

#endif /* GAME_H */