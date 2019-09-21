#include "Game.h"
extern  char nameOfBackground[200];
extern int xWinWidth, yWinHeight;
D3DXVECTOR3 sposition;
#pragma comment(lib,"WS2_32")
int debug = 1;
bool paint_white_square = false;
int number_offset;
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
	udp_debug_print("udp client operational\n");

	// udp client 
	serverInfo_debug.sin_family = AF_INET;
	serverInfo_debug.sin_port = htons(20320);
	serverInfo_debug.sin_addr.s_addr = inet_addr("127.0.0.1");
	socketC_debug = socket(AF_INET, SOCK_DGRAM, 0);
	my_printf("udp client operational\n");
	udp_debug_print("udp debug client operational\n");

	backGround = new GameSprite();
//	if (!backGround->Initialize(gDevice->device, "mycomm.png", 1600, 900)) return false;
	white_square_sprite = new GameSprite();
//	if (!backGround->Initialize(gDevice->device, nameOfBackground, 1600, 900)) return false;
	if (!backGround->Initialize(gDevice->device, nameOfBackground, xWinWidth, yWinHeight)) return false;
	faces[1] = new GameSprite();
	faces[2] = new GameSprite();
	faces[3] = new GameSprite();
	faces[4] = new GameSprite();
	faces[5] = new GameSprite();
	faces[6] = new GameSprite();
	int face_width, face_height, square_width, square_height;
	if(!(paint_white_square = get_dim_png("white_square.png", square_width, square_height))) 
	  {
		get_dim_png("face1.png", square_width, square_height);
		if (!white_square_sprite->Initialize(gDevice->device, "face1.png", (int)(face_scale * square_width), (int)(face_scale * square_height)))  return false;
	  } 
	else
	  {
		if (!white_square_sprite->Initialize(gDevice->device, "white_square.png", (int)(face_scale * square_width), (int)(face_scale * square_height)))  return false;
	  }
	get_dim_png("face1.png", face_width, face_height);
	if (!faces[1]->Initialize(gDevice->device, "face1.png", (int)(face_scale * face_width), (int)(face_scale * face_height)))  return false;
	number_offset = (int)((face_scale * face_width)/2); // help position numbers
	get_dim_png("face2.png", face_width, face_height);
	if (!faces[2]->Initialize(gDevice->device, "face2.png", (int)(face_scale * face_width), (int)(face_scale * face_height)))  return false;

	get_dim_png("face3.png", face_width, face_height);
	if (!faces[3]->Initialize(gDevice->device, "face3.png", (int)(face_scale * face_width), (int)(face_scale * face_height)))  return false;

	get_dim_png("face4.png", face_width, face_height);
	if (!faces[4]->Initialize(gDevice->device, "face4.png", (int)(face_scale * face_width), (int)(face_scale * face_height)))  return false;

	get_dim_png("face5.png", face_width, face_height);
	if (!faces[5]->Initialize(gDevice->device, "face5.png", (int)(face_scale * face_width), (int)(face_scale * face_height)))  return false;

	get_dim_png("face6.png", face_width, face_height);
	if (!faces[6]->Initialize(gDevice->device, "face6.png", (int)(face_scale * face_width), (int)(face_scale * face_height)))  return false;

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


	white_square_obj = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
	if (!white_square_obj->Initialize(gDevice->device, white_square_sprite,square_width , square_height)) return false;
	white_square_obj->SetDrawStatus(false);
	white_square_obj->SetPosition(1450,750);
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
		//		button_faces[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		//		if (!button_faces[i]->Initialize(gDevice->device, faces[1], 58, 86)) return false;
		//		button_faces[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_face = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_face->Initialize(gDevice->device, faces[1], 58, 86)) return false;
		tobii_button_matrix[i].button_face->SetDrawStatus(false);
		tobii_button_matrix[i].valid_index = false;
//		tobii_button_matrix[i].button_face->drawIt = false;
		//		tobii_button_matrix[i].button_face->SetSprite(faces[4]);

		//		button_num_1[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		//		if (!button_num_1[i]->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		//		button_num_1[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_num_1 = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_num_1->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		tobii_button_matrix[i].button_num_1->SetDrawStatus(false);
//		tobii_button_matrix[i].button_num_1->drawIt = false;
		//		button_num_2[i] = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		//		if (!button_num_2[i]->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		//		button_num_2[i]->SetDrawStatus(false);

		tobii_button_matrix[i].button_num_2 = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
		if (!tobii_button_matrix[i].button_num_2->Initialize(gDevice->device, digit[0], 9, 11)) return false;
		tobii_button_matrix[i].button_num_2->SetDrawStatus(false);
//		tobii_button_matrix[i].button_num_2->drawIt = false;
	}
	if (!Init_tobii_button_matrix())
	{
		return false;
	}
	gameTime = new GameTime();
	if (!gameTime->Initialize())
	{
		return false;
	}
//	test_tobii_button_matrix();
	return true;
}

bool Game::Init_tobii_button_matrix()
{
	int i;
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
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
	//	if (gameTime->totalGameTime > 4.0f)
	//		button_faces[0]->SetSprite(faces[4]);
	//	if (gameTime->totalGameTime > 8.0f)
	//		button_faces[0]->SetDrawStatus(false);
	Update(gameTime->elapsedTime);
//	D3DRASTER_STATUS rStatus;
//	direct3d->GetRasterStatus(0, &rStatus);

//	while (rStatus.InVBlank)
//	{
//		Sleep(0); // Ensure other threads get CPU time.
//		pd3dDevice->GetRasterStatus(0, &rStatus);
//	}

	// Should be in a vertical blank period. Call your function here.
//	MyFunction();
	Draw(gameTime->elapsedTime);

}

void Game::Reset_button_matrix()
{
	int i;
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
		tobii_button_matrix[i].valid_index = false;
	}
}

void Game::Update(float gameTime)
{
	int i,len;
	char temp_buff[2025];
	//Update our sprites and other game logic
	white_square_obj->Update(gameTime);
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
		if (tobii_button_matrix[i].button_face) tobii_button_matrix[i].button_face->Update(gameTime);
		if (tobii_button_matrix[i].button_face) tobii_button_matrix[i].button_num_1->Update(gameTime);
		if (tobii_button_matrix[i].button_face) tobii_button_matrix[i].button_num_2->Update(gameTime);
	}
	if (len = udp_get_packet(temp_buff))
	{
		OutputDebugString(temp_buff);
		my_printf("ok got a packet\n");
//		udp_debug_print(temp_buff);
		//my_printf("%s\n", temp_buff);
		parse_udp_msg(temp_buff, len);
	}
	

}

void Game::Draw(float gTime)
{
	int i;
	bool some_face_drawn;
	//Simple RGB value for the background so use XRGB instead of ARGB.
	gDevice->Clear(D3DCOLOR_XRGB(0, 100, 100));
	gDevice->Begin();
	//master_clock = gameTime->totalGameTime; // update masterclock
	some_face_drawn = false;
	if (tobii_menu) tobii_menu->Draw(gTime);
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
		//		tobii_button_matrix[i].button_face->drawIt = true;
		if ((tobii_button_matrix[i].button_face->drawIt && tobii_button_matrix[i].valid_index)) {
			tobii_button_matrix[i].button_face->Draw(gTime);
			some_face_drawn = true;
		}
//		if ((tobii_button_matrix[i].button_face->drawIt && tobii_button_matrix[i].valid_index) && (tobii_button_matrix[i].ssvep_period > 0.0) && 
//			(fmod(gameTime->totalGameTime - tobii_button_matrix[i].face_first_on, tobii_button_matrix[i].ssvep_period) > 
//			(0.5*tobii_button_matrix[i].ssvep_period))) tobii_button_matrix[i].button_face->Draw(gTime);
//		else if ((tobii_button_matrix[i].button_face->drawIt && tobii_button_matrix[i].valid_index) && (tobii_button_matrix[i].ssvep_period == 0.0))
//			tobii_button_matrix[i].button_face->Draw(gTime);
		if (((gameTime->totalGameTime - tobii_button_matrix[i].number_first_on) < tobii_button_matrix[i].number_timer) && tobii_button_matrix[i].valid_index){
			tobii_button_matrix[i].button_num_1->Draw(gTime);
			tobii_button_matrix[i].button_num_2->Draw(gTime);
		}
	}
	if (some_face_drawn && paint_white_square) white_square_obj->Draw(gTime);
	gameTime->Update();
	gDevice->End();
	gDevice->Present();
}

Game::~Game()
{
	int i;
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
		if (tobii_button_matrix[i].button_face) { delete tobii_button_matrix[i].button_face; tobii_button_matrix[i].button_face = 0; }
		if (tobii_button_matrix[i].button_face) { delete tobii_button_matrix[i].button_num_1; tobii_button_matrix[i].button_num_1 = 0; }
		if (tobii_button_matrix[i].button_face) { delete tobii_button_matrix[i].button_num_2; tobii_button_matrix[i].button_num_2 = 0; }
	}
	if (white_square_obj) { delete white_square_obj; white_square_obj = 0; }
	if (tobii_menu) { delete tobii_menu; tobii_menu = 0; }
	if (gDevice) { delete gDevice; gDevice = 0; }
	if (gameTime) { delete gameTime; gameTime = 0; }
}

void Game::InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}
void Game::my_printf_t(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[1025]; // get rid of this hard-coded buffer
	char mybuff[1025];
	nBuf = sprintf_s(szBuffer, 1025, lpszFormat, args);
	::OutputDebugString(szBuffer);
	sprintf_s(mybuff, 1025, lpszFormat, args);
	udp_debug_print(mybuff);
	va_end(args);
}


void Game::my_printf(const char *format, ...)
{
//	char buf[1024];

//	va_list arglist;
//	va_start(arglist, format);
//	vsprintf_s(buf, 1024, format, arglist);
//	udp_debug_print(buf);
//	va_end(arglist);
}

void Game::test_tobii_button_matrix()
{
	int i, j, k;
	float xc, yc, off_x, off_y;
	xc = 250.0;
	yc = 180.0;
	off_x = 150.0;
	off_y = 160.0;
	// put a null in the id so we know it is empty, zero is not used for valid id
	for (k = 0; k <= 23; k++) { //0 left,1599 right, 0 top,  900 bottom
		i = (k % 6) + 1;
		j = (k / 6) + 1;
		tobii_button_matrix[k + 1].id = k + 1;
		tobii_button_matrix[k + 1].loc_x = off_x + (i - 1)*xc;
		tobii_button_matrix[k + 1].valid_index = true;
		tobii_button_matrix[k + 1].button_face->drawIt = true;
		tobii_button_matrix[k + 1].loc_y = (j - 1)*yc + off_y;
		tobii_button_matrix[k + 1].button_face->SetPosition(tobii_button_matrix[k + 1].loc_x, tobii_button_matrix[k + 1].loc_y);
		tobii_button_matrix[k + 1].curr_face_num = (rand() % MAX_NUMBER_OF_FACES) + 1;
		tobii_button_matrix[k + 1].button_face->SetSprite(faces[tobii_button_matrix[k + 1].curr_face_num]);
		//		tobii_button_matrix[k + 1].face_on = true;
	//	tobii_button_matrix[k + 1].digit1_id = (k + 1) / 10;
	//	tobii_button_matrix[k + 1].digit2_id = (k + 1) - (((k + 1) / 10) * 10);
	//	tobii_button_matrix[k + 1].button_num_1->SetPosition(tobii_button_matrix[k + 1].loc_x, tobii_button_matrix[k + 1].loc_y);
	//	tobii_button_matrix[k + 1].button_num_1->SetSprite(digit[tobii_button_matrix[k + 1].digit1_id]);
	//	tobii_button_matrix[k + 1].button_num_2->SetPosition(tobii_button_matrix[k + 1].loc_x+10, tobii_button_matrix[k + 1].loc_y);
	//	tobii_button_matrix[k + 1].button_num_2->SetSprite(digit[tobii_button_matrix[k + 1].digit2_id]);
	//	tobii_button_matrix[k + 1].number_timer = 2.0f;
	//	tobii_button_matrix[k + 1].number_first_on = -1.0f;
		setDigits(k + 1);
	//		my_printf("putting in test data for matrix_id=%d face_id=%d at %d  first_on is %f send_msg_to_netv\(\"Alter_coordxy,%d,%f,%f\"\) \n", tobii_button_matrix[k + 1].id, tobii_button_matrix[k + 1].curr_face_num, tobii_button_matrix[k + 1].loc_x, tobii_button_matrix[k + 1].loc_y, k + 1, (i - 1)*xc, (j - 1)*yc);
	}
	// exit(0);
}

void Game::udp_debug_print(char* msg)
{
	char buffer[2000];
	int i;
	for (i = 0; i < 1025; i++)
		buffer[i] = '\0';
	int len = sizeof(serverInfo);
	_snprintf_s(buffer, sizeof(buffer), "%s\n", msg);
//	OutputDebugString(buffer);
	if (sendto(socketC, buffer, 1024, 0, (sockaddr*)&serverInfo, len) == SOCKET_ERROR) {
		//	if (sendto(socketC, buffer, my_strlen(msg), 0, (sockaddr*)&serverInfo, len) == SOCKET_ERROR) {
		printf("socket error\n");
	}
}

bool Game::get_dim_png(char* file_name, int &pngwidth, int &pngheight)
{
	std::ifstream in(file_name);
	unsigned int width, height;
	if(!in.seekg(16)) return false;
	in.read((char *)&width, 4);
	in.read((char *)&height, 4);
	pngwidth = ntohl(width);
	pngheight = ntohl(height);
	return true;
}
int Game::udp_get_packet(char* buffer)
{
	int iResult;
	int fromlen = sizeof(from);
	char dbuffer[1024];

	iResult = recvfrom(socketS, buffer, 1024, 0, (sockaddr*)&from, &fromlen);
	if (WSAGetLastError() == WSAEWOULDBLOCK)
	{
		return(0);
	}
	//Sleep(1);
	sprintf_s(dbuffer, sizeof(dbuffer), "number of bytes received is %d\n", iResult);

	//udp_debug_print(dbuffer);
	//Sleep(1);
   // udp_debug_print(buffer);
	//Sleep(1);
	OutputDebugString("\nthis is a test\n");
	OutputDebugString(buffer);
	sprintf_s(dbuffer, sizeof(dbuffer), "last error is %d\n", WSAGetLastError());
	//udp_debug_print(dbuffer);
	return(iResult);
}

void Game::parse_udp_msg(char *udp_buff, int len)
{
	int bci_num_display, bci_num_x, bci_num_y;
	char *result;
	char *cmd;
	char *next_token1 = NULL;
	char delims[] = ",";
	std::string p_string, n_string;

	int j,i;
	int rnd_face_num;		// random face number   
	char temp_space[2025];
	int xsize, ysize;
	udp_buff[len] = '\0';
	my_printf("Received the following:\n");
//	my_printf("%s\n", udp_buff);
	udp_debug_print("in parse\n");
	udp_debug_print(udp_buff);
	my_printf("-------------------------------------------------------\n");

	result = strtok_s(udp_buff, delims, &next_token1);	// parse udp commands
	//udp_debug_print("in parse after strtok_s below is the first result\n");
//	udp_debug_print(result);
//	sprintf_s(temp_space, 1024, "result is %s and length is %d", result,strlen(result));
//	udp_debug_print(temp_space);

	if (isdigit(*result))
	{
		char *cmd = "Flash_list";
		Clear(); 
		p_string = "Flash _list ";
		num_flash_items = 0;
		while (result != NULL)
		{
			p_string = p_string + result + " ";
			flash_list[num_flash_items++] = atoi(result);
//			sprintf_s(temp_space, 1024, "flash list for %d is %s", num_flash_items - 1, result);
//			udp_debug_print(temp_space);

//			my_printf("result is %s\n", result);
			result = strtok_s(NULL, delims, &next_token1);

		}
		sprintf_s(temp_space, 1024, "flash list is %d long\n", num_flash_items);
		udp_debug_print(temp_space);
		strcpy_s(temp_space, 1024, p_string.c_str());
		udp_debug_print(temp_space);
		if (debug)
		{
			my_printf("flash list is %d long\n", num_flash_items);
		}
	}
	else if (!strncmp(result, "Autoflash 1", 100))
	{
		cmd = "Autoflash";
		udp_debug_print("*******************************************");
		udp_debug_print(cmd);
		udp_debug_print("*******************************************");
		if (debug)
		{
			my_printf("Autoflash Command is %s\n", cmd);
		}
		// is j=0 right ??? I thought I started at 1
		for (j = 0; j < num_flash_items; j++)
		{
			rnd_face_num = (rand() % MAX_NUMBER_OF_FACES) + 1;	// select a pseudo random face from MAX_NUMBER_OF_FACES (faces are numbered 1..MAX_NUMBER_OF_FACES)

  //		  //draw_sprite(flash_list[j],rnd_face_num,TRUE);
			tobii_button_matrix[flash_list[j]].button_face->SetDrawStatus(true);
			tobii_button_matrix[flash_list[j]].face_first_on = gameTime->totalGameTime;
	//		tobii_button_matrix[flash_list[j]].button_face->drawIt = true;
			tobii_button_matrix[flash_list[j]].curr_face_num = rnd_face_num; // hey I need to set a pointer
			tobii_button_matrix[flash_list[j]].button_face->SetSprite(faces[tobii_button_matrix[flash_list[j]].curr_face_num]);
			if (debug)
			{
				my_printf("flash item=%d random number is %d at %d \n",
					flash_list[j], rnd_face_num,
					tobii_button_matrix[flash_list[j]].loc_x);
			}

		}
		if (debug)
		{
			my_printf("\n");
		}
	}

	else if (!strncmp(result, "Conceal", 100))
	{
		cmd = "Conceal";
		udp_debug_print(cmd);
		if (debug)
		{
			my_printf("Conceal Command is %s\n", cmd);
		}
		Clear();
		for (j = 0; j < num_flash_items; j++)
		{
//			rnd_face_num = tobii_button_matrix[flash_list[j]].curr_face_num;
			//		  p300_matrix[flash_list[j]].id=0;
			//	      draw_sprite(flash_list[j],rnd_face_num,FALSE);
//			tobii_button_matrix[flash_list[j]].button_face->drawIt = false;
			udp_debug_print("setting flash list to conceal");
			sprintf_s(temp_space, 1024, "Flash list index is %d\n", flash_list[j]);
			udp_debug_print(temp_space);
			tobii_button_matrix[flash_list[j]].button_face->SetDrawStatus(false);
			tobii_button_matrix[flash_list[j]].face_first_on = 0.0;
			//          p300_matrix[flash_list[j]].face_on=false;
			if (debug)
			{
				my_printf("flash item=%d random number is %d\n",
					flash_list[j],
					tobii_button_matrix[flash_list[j]].curr_face_num);
			}

		}
		if (debug)
		{
			my_printf("\n");
		}
	}
	else if (!strncmp(result, "End", 100))
	{			// a way to end UDP loop remotely, will need to be sent manually
		cmd = "End";
		udp_debug_print(cmd);
		if (debug)
		{
			my_printf("End Command is %s\n", cmd);
		}
		//close_graphics ();
	   // return 0;
	}
	else if (!strncmp(result, "Alter_coordxy", 100))
	{
		cmd = "Alter_coordxy";
		if (debug)
		{
			my_printf("Alter_coordxy Command is %s\n", cmd);
		}
		result = strtok_s(NULL, delims, &next_token1);
		for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
			tobii_button_matrix[i].button_face->SetDrawStatus(false);
		}
		curr_matrix_element_id = atoi(result);
		if (curr_matrix_element_id > MAX_NUMBER_OF_BUTTONS)
		{
			my_printf
			("an attempt to alter %d which is greater than max number of elements\n",
				curr_matrix_element_id);
		}
		else
		{
			result = strtok_s(NULL, delims, &next_token1);
		//	tobii_button_matrix[curr_matrix_element_id].digit1_id = (curr_matrix_element_id) / 10;
		//	tobii_button_matrix[curr_matrix_element_id].digit2_id = (curr_matrix_element_id)-(((curr_matrix_element_id) / 10) * 10);
		//	tobii_button_matrix[curr_matrix_element_id].number_timer = 2.0f;
		//	tobii_button_matrix[curr_matrix_element_id].number_first_on = 0;
			tobii_button_matrix[curr_matrix_element_id].loc_x = float(atoi(result));
			tobii_button_matrix[curr_matrix_element_id].id = curr_matrix_element_id;
			tobii_button_matrix[curr_matrix_element_id].valid_index = true;
			if (debug)
				printf("curr_matrix_element %d x %f\n",
					curr_matrix_element_id,
					tobii_button_matrix[curr_matrix_element_id].loc_x);
			//      if (xbounds_check (curr_matrix_element_id))
			//	exit (1);
			result = strtok_s(NULL, delims, &next_token1);
			tobii_button_matrix[curr_matrix_element_id].loc_y = float(atoi(result));
			sprintf_s(temp_space, 1024, "alter_xy for %d is %f,%f", curr_matrix_element_id, tobii_button_matrix[curr_matrix_element_id].loc_x - offset_x, tobii_button_matrix[curr_matrix_element_id].loc_y - offset_y);
			tobii_button_matrix[curr_matrix_element_id].button_face->SetPosition(tobii_button_matrix[curr_matrix_element_id].loc_x-offset_x, tobii_button_matrix[curr_matrix_element_id].loc_y-offset_y);
			udp_debug_print(temp_space);
			setDigits(curr_matrix_element_id);
			if (debug)
				printf("curr_matrix_element %d y %f \n",
					curr_matrix_element_id,
					tobii_button_matrix[curr_matrix_element_id].loc_y);
			//      if (ybounds_check (curr_matrix_element_id))
			//	exit (1);
			//      display_num (curr_matrix_element_id,
			//		   p300_matrix[curr_matrix_element_id].loc_x,
			//		   p300_matrix[curr_matrix_element_id].loc_y);

			my_printf("just displayed numbers\n");
		}
		//           my_printf( "result is \"%s\"\n", result );
	}
	else if (!strncmp(result, "Reset_coordxy", 100))
	{
		cmd = "Reset_coordxy";
		udp_debug_print(cmd);
		if (debug)
		{
			my_printf("Reset_coordxy Command is %s\n", cmd);
		}
//		Init_tobii_button_matrix();
		Reset_button_matrix();
		Clear();
		
	}
	else if (!strncmp(result, "Alter_ssvep", 100))
	{
	    result = strtok_s(NULL, delims, &next_token1);
		curr_matrix_element_id = atoi(result);
		if (curr_matrix_element_id > MAX_NUMBER_OF_BUTTONS)
		{
			my_printf
			("an attempt to alter %d which is greater than max number of elements\n",
				curr_matrix_element_id);
		}
		else
		{
			result = strtok_s(NULL, delims, &next_token1);
			tobii_button_matrix[curr_matrix_element_id].ssvep_period = atof(result);
		}
 }
	else if (!strncmp(result, "Display_bci_num", 100))
	{
		cmd = "Display_bci_num";
		udp_debug_print(cmd);
		if (debug)
		{
			my_printf("Display_bci_num Command is %s\n", cmd);
		}
		result = strtok_s(NULL, delims, &next_token1);
		bci_num_display = atoi(result);
		if (curr_matrix_element_id > MAX_NUMBER_OF_BUTTONS)
		{
			my_printf
			("an attempt to display bci num %d which is geater than max number of elements\n",
				bci_num_display);
		}
		else
		{
			result = strtok_s(NULL, delims, &next_token1);
			bci_num_x = atoi(result);
			result = strtok_s(NULL, delims, &next_token1);
			bci_num_y = atoi(result);
		}
		my_printf("       Display_bci_num at %d\n", bci_num_display);
	}
	else if (!strncmp(result, "Change_background_texture", 100))
	{
		result = strtok_s(NULL, delims, &next_token1); // skip over name
		n_string = result;
		result = strtok_s(NULL, delims, &next_token1);
		xsize = atoi(result);
		result = strtok_s(NULL, delims, &next_token1);
		ysize = atoi(result);
		cmd = "Change_background_texture";
		udp_debug_print(cmd);
		udp_debug_print("********");
		udp_debug_print(result);
		udp_debug_print("********");
//		change_background_texture("C:\\Users\\steve\\Desktop\\SpriteWithTimer\\mycomm.png",xsize,ysize);
		change_background_texture(n_string.c_str(), xsize, ysize);
		if (debug)
		{
			my_printf("Change_background_texture Command is %s %s\n", cmd, result);
		}

	}
	else if (!strncmp(result, "Clear", 100))
	{
		cmd = "Clear";
		udp_debug_print(cmd);
		Clear();
		if (debug)
		{
			my_printf("Clear Command is %s\n", cmd);
		}
		//	  system
		//	    ("SCREEN_X_RES=1280 SCREEN_Y_RES=720 imgtool --bitfmt=rgb565 --fill=240,0,240");
	}
	else if (!strncmp(result, "Exit", 100))
	{
		cmd = "Exit";
		udp_debug_print(cmd);
		if (debug)
		{
			my_printf("Exit Command is %s\n", cmd);
		}
		exit(1);
	}
	else
	{
		cmd = "Unknown";
		udp_debug_print(cmd);
		if (debug)
		{
			my_printf("Unknown Command is %s\n", cmd);
		}
	}
	if (debug)
	{
		//	  my_printf ("command is %s %s\n", cmd, result);
	}

}

void Game::change_background_texture(const char* name,int xsize,int ysize) {
	//if (tobii_menu) { delete tobii_menu; tobii_menu = 0; }
	//tobii_menu = new GameplayObject(0, 0, 0.0f, 0.0f, 0.0f);
	//delete backGround;
//	backGround->Initialize(gDevice->device, name, 1600, 900);
	backGround->Initialize(gDevice->device, name, xsize, ysize);
	//tobii_menu->Initialize(gDevice->device, backGround, 1600, 900);	
}

void Game::Clear() {
	int i;
	for (i = 1; i < MAX_NUMBER_OF_BUTTONS + 1; i++) {
		tobii_button_matrix[i].button_face->SetDrawStatus(false);
		tobii_button_matrix[i].face_first_on = 0.0;
//		tobii_button_matrix[i].button_face->drawIt = false;
	}
}
void Game::setDigits(int buttonId) {
	tobii_button_matrix[buttonId].digit1_id = (buttonId) / 10;
	tobii_button_matrix[buttonId].digit2_id = (buttonId)-(((buttonId) / 10) * 10);
	tobii_button_matrix[buttonId].button_num_1->SetPosition(tobii_button_matrix[buttonId].loc_x+number_offset, tobii_button_matrix[buttonId].loc_y+number_offset);
	tobii_button_matrix[buttonId].button_num_1->SetSprite(digit[tobii_button_matrix[buttonId].digit1_id]);
	tobii_button_matrix[buttonId].button_num_2->SetPosition(tobii_button_matrix[buttonId].loc_x + number_offset + 10, tobii_button_matrix[buttonId].loc_y + number_offset);
	tobii_button_matrix[buttonId].button_num_2->SetSprite(digit[tobii_button_matrix[buttonId].digit2_id]);
	tobii_button_matrix[buttonId].number_timer = 4.0f;
	tobii_button_matrix[buttonId].number_first_on = gameTime->totalGameTime;
}
void Game::udp_debug_print_debug(char* msg)
{
	char buffer[2000];
	int i;
	for (i = 0; i < 1025; i++)
		buffer[i] = '\0';
	int len = sizeof(serverInfo_debug);
	_snprintf_s(buffer, sizeof(buffer), "%s\n", msg);
	i = 0;
	for (i = 0; buffer[i] != '\0'; i++)
	{
	}
	//	OutputDebugString(buffer);
	if (sendto(socketC_debug, buffer, i, 0, (sockaddr*)&serverInfo_debug, len) == SOCKET_ERROR) {
		printf("socket error\n");
	}
}
