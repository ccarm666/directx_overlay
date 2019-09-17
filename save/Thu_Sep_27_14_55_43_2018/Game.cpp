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

	backGround = new GameSprite();
	if (!backGround->Initialize(gDevice->device, "mycomm_mod.png", 1600, 900)) return false;
	faces[1] = new GameSprite();
	faces[2] = new GameSprite();
	faces[3] = new GameSprite();
	faces[4] = new GameSprite();
	faces[5] = new GameSprite();
	faces[6] = new GameSprite();
	if (!faces[1]->Initialize(gDevice->device, "face1.png", 50, 58)) return false;
	if (!faces[2]->Initialize(gDevice->device, "face2.png", 48, 58)) return false;
	if (!faces[3]->Initialize(gDevice->device, "face2.png", 50, 58)) return false;
	if (!faces[4]->Initialize(gDevice->device, "face2.png", 51, 58)) return false;
	if (!faces[5]->Initialize(gDevice->device, "face2.png", 52, 58)) return false;
	if (!faces[6]->Initialize(gDevice->device, "face2.png", 51, 57)) return false;
	
	digit[0] = new GameSprite();
	digit[1] = new GameSprite();
	digit[2] = new GameSprite();
	digit[3] = new GameSprite();
	digit[4] = new GameSprite();
	digit[5] = new GameSprite();
	digit[6] = new GameSprite();
	digit[7] = new GameSprite();
	digit[8] = new GameSprite();
	digit[9] = new GameSprite();
	if (!digit[0]->Initialize(gDevice->device, "0.png", 50, 58)) return false;
	if (!digit[1]->Initialize(gDevice->device, "1.png", 50, 58)) return false;
	if (!digit[2]->Initialize(gDevice->device, "2.png", 50, 58)) return false;
	if (!digit[3]->Initialize(gDevice->device, "3.png", 50, 58)) return false;
	if (!digit[4]->Initialize(gDevice->device, "4.png", 50, 58)) return false;
	if (!digit[5]->Initialize(gDevice->device, "5.png", 50, 58)) return false;
	if (!digit[6]->Initialize(gDevice->device, "6.png", 50, 58)) return false;
	if (!digit[7]->Initialize(gDevice->device, "7.png", 50, 58)) return false;
	if (!digit[8]->Initialize(gDevice->device, "8.png", 50, 58)) return false;
	if (!digit[9]->Initialize(gDevice->device, "9.png", 50, 58)) return false;
	

 //   player = new GameplayObject(100, 200, (float)M_PI_4, 1.5f, 1.5f);
	player = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);

//	if(!player->Initialize(gDevice->device, "PlayerPaper.png", 58, 86))
	if (!player->Initialize(gDevice->device, backGround, 1600,900 ))
	{
		return false;
	}

	player2 = new GameplayObject(100, 200, 0, 1.5f, 1.5f);
//	if(!player2->Initialize(gDevice->device, "PlayerPaper.png", 58, 86))
	if (!player2->Initialize(gDevice->device, faces[1], 58, 86))
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
		//		player2->SetDrawStatus(false);
		player2->SetSprite(faces[4]);
	if (gameTime->totalGameTime > 8.0f)
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