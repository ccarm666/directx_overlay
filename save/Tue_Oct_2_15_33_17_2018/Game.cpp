#include "Game.h"
D3DXVECTOR3 sposition;

Game::Game()
{
	//Constructor
}

bool Game::Initialize(HWND hWnd)
{
	int i;
	gDevice = new GraphicsDevice();
	if (!gDevice->Initialize(hWnd, true))
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
	if (!digit[0]->Initialize(gDevice->device, "0.png", 9, 11)) return false;
	if (!digit[1]->Initialize(gDevice->device, "1.png", 9, 11)) return false;
	if (!digit[2]->Initialize(gDevice->device, "2.png", 9, 11)) return false;
	if (!digit[3]->Initialize(gDevice->device, "3.png", 9, 11)) return false;
	if (!digit[4]->Initialize(gDevice->device, "4.png", 9, 11)) return false;
	if (!digit[5]->Initialize(gDevice->device, "5.png", 9, 11)) return false;
	if (!digit[6]->Initialize(gDevice->device, "6.png", 9, 11)) return false;
	if (!digit[7]->Initialize(gDevice->device, "7.png", 9, 11)) return false;
	if (!digit[8]->Initialize(gDevice->device, "8.png", 9, 11)) return false;
	if (!digit[9]->Initialize(gDevice->device, "9.png", 9, 11)) return false;



	tobii_menu = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
	if (!tobii_menu->Initialize(gDevice->device, backGround, 1600, 900))
	{
		return false;
	}

	button_faces[0] = new GameplayObject(0, 0, 0.0f, 1.5f, 1.5f);
	if (!button_faces[0]->Initialize(gDevice->device, faces[1], 58, 86)) return false;
	button_faces[0]->SetDrawStatus(true);


	for (i = 1; i < MAX_NUMBER_OF_BUTTONS; i++) {
		button_faces[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!button_faces[i]->Initialize(gDevice->device, faces[1], 58, 86)) return false;
		button_faces[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_face = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_face->Initialize(gDevice->device, faces[1], 58, 86)) return false;
		tobii_button_matrix[i].button_face->SetDrawStatus(false);

		button_num_1[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!button_num_1[i]->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		button_num_1[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_num_1 = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_num_1->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		tobii_button_matrix[i].button_num_1->SetDrawStatus(false);

		button_num_2[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!button_num_2[i]->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		button_num_2[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_num_2 = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_num_2->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		tobii_button_matrix[i].button_num_2->SetDrawStatus(false);

	}
	gameTime = new GameTime();
	if (!gameTime->Initialize())
	{
		return false;
	}
	return true;
}

bool Game::Init_tobii_button_matrix()
{
	int i;
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS; i++) {
		tobii_button_matrix[i].face_on = false;
		tobii_button_matrix[i].id = 0;
		tobii_button_matrix[i].number_first_on = -1.0;
		tobii_button_matrix[i].digit1_id = (i) / 10;
		tobii_button_matrix[i].digit2_id = (i)-(((i) / 10) * 10);
	}


	return true;
}

void Game::Run()
{
	//Get game time then update + draw
	gameTime->Update();
	if (gameTime->totalGameTime > 4.0f)
		button_faces[0]->SetSprite(faces[4]);
	if (gameTime->totalGameTime > 8.0f)
		button_faces[0]->SetDrawStatus(false);
	Update(gameTime->elapsedTime);
	Draw(gameTime->elapsedTime);

}

void Game::Update(float gameTime)
{
	int i;
	//Update our sprites and other game logic
	for (i = 0; i < MAX_NUMBER_OF_BUTTONS; i++) {
		if (button_faces[i]) button_faces[i]->Update(gameTime);
		if (button_num_1[i]) button_num_1[i]->Update(gameTime);
		if (button_num_2[i]) button_num_2[i]->Update(gameTime);
	}
	//	if(button_faces[0]) button_faces[0]->Update(gameTime);

}

void Game::Draw(float gTime)
{
	int i;
	//Simple RGB value for the background so use XRGB instead of ARGB.
	gDevice->Clear(D3DCOLOR_XRGB(0, 100, 100));
	gDevice->Begin();
	//master_clock = gameTime->totalGameTime; // update masterclock
	if (tobii_menu) tobii_menu->Draw(gTime);
	for (i = 0; i < MAX_NUMBER_OF_BUTTONS; i++) {
		if (button_faces[i] && button_faces[i]->drawIt) button_faces[i]->Draw(gTime);
		if (button_num_1[i] && button_num_1[i]->drawIt) button_num_1[i]->Draw(gTime);
		if (button_num_2[i] && button_num_2[i]->drawIt) button_num_2[i]->Draw(gTime);
	}
	gameTime->Update();
	gDevice->End();
	gDevice->Present();
}

Game::~Game()
{
	int i;
	for (i = 0; i < MAX_NUMBER_OF_BUTTONS; i++) {
		if (button_faces[i]) { delete button_faces[i]; button_faces[i] = 0; }
		if (button_num_1[i]) { delete button_num_1[i]; button_num_1[i] = 0; }
		if (button_num_2[i]) { delete button_num_2[i]; button_num_2[i] = 0; }
	}
	if (tobii_menu) { delete tobii_menu; tobii_menu = 0; }
	if (gDevice) { delete gDevice; gDevice = 0; }
	if (gameTime) { delete gameTime; gameTime = 0; }
}
