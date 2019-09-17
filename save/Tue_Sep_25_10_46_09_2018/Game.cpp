#include "Game.h"

Game::Game()
{
	//Constructor
}

bool Game::Initialize(HWND hWnd)
{
	gDevice = new GraphicsDevice();
	if(!gDevice->Initialize(hWnd, true))
	{
		return false;
	}

 //   player = new GameplayObject(100, 200, (float)M_PI_4, 1.5f, 1.5f);
	player = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);

//	if(!player->Initialize(gDevice->device, "PlayerPaper.png", 58, 86))
	if (!player->Initialize(gDevice->device, "mycomm_mod.png", 1600,900 ))
	{
		return false;
	}

	player2 = new GameplayObject(100, 200, 0, 1.5f, 1.5f);
//	if(!player2->Initialize(gDevice->device, "PlayerPaper.png", 58, 86))
	if (!player2->Initialize(gDevice->device, "face1.png", 58, 86))
	{
		return false;
	}

	gameTime = new GameTime();
	if(!gameTime->Initialize())
	{
		return false;
	}

	return true;
}

void Game::Run()
{
	//Get game time then update + draw
	gameTime->Update();

	Update(gameTime->elapsedTime);
	Draw(gameTime->elapsedTime);
	if (gameTime->totalGameTime > 4.0f)
		player2->SetDrawStatus(false);
}

void Game::Update(float gameTime)
{
	//Update our sprites and other game logic
	if(player) player->Update(gameTime);
	if(player2) player2->Update(gameTime);
}

void Game::Draw(float gTime)
{
	//Simple RGB value for the background so use XRGB instead of ARGB.
	gDevice->Clear(D3DCOLOR_XRGB(0,100,100));
	gDevice->Begin();

	if(player) player->Draw(gTime);
	if(player2 && player2->drawIt) player2->Draw(gTime);
//	gameTime->Update();
	

	gDevice->End();
	gDevice->Present();
}

Game::~Game()
{
	if(player) { delete player; player = 0; }
	if(player2) { delete player2; player2 = 0; }
	if(gDevice) { delete gDevice; gDevice = 0; }
	if(gameTime) { delete gameTime; gameTime = 0; }
}