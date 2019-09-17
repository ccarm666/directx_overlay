#include "Game.h"
D3DXVECTOR3 sposition;
#pragma comment(lib,"WS2_32")

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
	InitWinsock();
	// udp server
	int fromlen = sizeof(from);
	local.sin_family = AF_INET;
	local.sin_port = htons(12000);
	local.sin_addr.s_addr = INADDR_ANY;
	socketS = socket(AF_INET, SOCK_DGRAM, 0);
	int iResult;
	u_long iMode = 1;
	iResult = ioctlsocket(socketS, FIONBIO, &iMode);
	if (iResult != NO_ERROR) {
		my_printf("Error! unable to set nonblocking\n");
		exit(1);
	}
	iResult = bind(socketS, (sockaddr*)&local, sizeof(local));
	if (iResult != NO_ERROR) {
		my_printf("Error! bind failed %d\n", iResult);
		exit(1);
	}
	my_printf("udp server operational\n");

	// udp client 
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(4321);
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
	socketC = socket(AF_INET, SOCK_DGRAM, 0);
	my_printf("udp client operational\n");



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

	


	for (i = 0; i < MAX_NUMBER_OF_BUTTONS; i++) {
		button_faces[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!button_faces[i]->Initialize(gDevice->device, faces[1], 58, 86)) return false;
		button_faces[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_face = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_face->Initialize(gDevice->device, faces[1], 58, 86)) return false;
		tobii_button_matrix[i].button_face->SetDrawStatus(false);
		tobii_button_matrix[i].button_face->SetSprite(faces[4]);

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
	button_faces[0] = new GameplayObject(0, 0, 0.0f, 1.5f, 1.5f);
	if (!button_faces[0]->Initialize(gDevice->device, faces[1], 58, 86)) return false;
	button_faces[0]->SetDrawStatus(true);
	if (!Init_tobii_button_matrix())
	{
		return false;
	}
	gameTime = new GameTime();
	if (!gameTime->Initialize())
	{
		return false;
	}
	test_tobii_button_matrix();
	return true;
}

bool Game::Init_tobii_button_matrix()
{
	int i;
	for (i = 0; i < MAX_NUMBER_OF_BUTTONS; i++) {
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
		if (tobii_button_matrix[i].button_face) tobii_button_matrix[i].button_face->Update(gameTime);
		if (tobii_button_matrix[i].button_face) tobii_button_matrix[i].button_num_1->Update(gameTime);
		if (tobii_button_matrix[i].button_face) tobii_button_matrix[i].button_num_2->Update(gameTime);
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
		tobii_button_matrix[i].button_face->drawIt = true;
		if (tobii_button_matrix[i].button_face->drawIt) tobii_button_matrix[i].button_face->Draw(gTime);
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
void Game::InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}
void Game::my_printf(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[1025]; // get rid of this hard-coded buffer
	nBuf = sprintf_s(szBuffer, 1025, lpszFormat, args);
	::OutputDebugString(szBuffer);
	va_end(args);
}
void Game::test_tobii_button_matrix()
{
	int i, j, k, xc, yc, off_x, off_y;
	xc = 300;
	yc = 200;
	off_x = 610;
	off_y = -330;
	// put a null in the id so we know it is empty, zero is not used for valid id
	for (k = 0; k <= 23; k++) { //800 left,-800 right, -400 top, 400 bottom
		i = (k % 6) + 1;
		j = (k / 6) + 1;
		tobii_button_matrix[k + 1].id = k + 1;
		tobii_button_matrix[k + 1].loc_x = off_x - (i - 1)*xc;
		tobii_button_matrix[k + 1].loc_y = (j - 1)*yc + off_y;
		tobii_button_matrix[k + 1].curr_face_num = (rand() % MAX_NUMBER_OF_FACES) + 1;
		tobii_button_matrix[k + 1].face_on = true;
		tobii_button_matrix[k + 1].digit1_id = (k + 1) / 10;
		tobii_button_matrix[k + 1].digit2_id = (k + 1) - (((k + 1) / 10) * 10);
		tobii_button_matrix[k + 1].number_timer = 20000.0f;
		tobii_button_matrix[k + 1].number_first_on = -1.0f;
		my_printf("putting in test data for matrix_id=%d face_id=%d at %d  first_on is %f send_msg_to_netv\(\"Alter_coordxy,%d,%d,%d\"\) \n", tobii_button_matrix[k + 1].id, tobii_button_matrix[k + 1].curr_face_num, tobii_button_matrix[k + 1].loc_x, tobii_button_matrix[k + 1].loc_y, k + 1, (i - 1)*xc, (j - 1)*yc);
	}
	// exit(0);
}
