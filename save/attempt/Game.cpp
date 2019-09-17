#include "Game.h"
D3DXVECTOR3 sposition;
#define MAX_NUMBER_OF_MATRIX_ELEMENTS 110
#define MAX_TEST_ELENENTS 64
#define FACE_HEIGHT 40
#define FACE_WIDTH  40
#define MAX_NUMBER_OF_FACES 6
#define SERV_PORT  12000
#define MAXSCREEN_WIDTH 1280
#define MAXSCREEN_HEIGHT 720
#define X_JAIL_COORD 1100
#define Y_JAIL_CORRD 600
// display_num defs
#define BUF_LEN 30
#define DIGIT_HEIGHT 11
#define DIGIT_WIDTH 9
#define DIGIT_GAP 1
#define NUMBER_OF_NUMBERS 100 
#define MAX_STR_LENGTH 100

// Global variables
int debug = 1;
int number_of_faces = MAX_NUMBER_OF_FACES;
int number_of_matrix_elements = 24;
int curr_matrix_element_id = 0;
int matrix_x_origin = 0;
int bci_num_display = 0;
int bci_num_x, bci_num_y;
int num_flash_items = 0;
int flash_list[MAX_NUMBER_OF_MATRIX_ELEMENTS + 1]; //this vector starts at zero
double master_clock = 0.0;
double prev_master_clock = 0.0;
double NUMBER_LIFE = 10000.0; // 10 secs 
float XSCALE = -1.0f;
float XOFFSET = 610.0f;
float YSCALE = 1.0f;
float YOFFSET = -330.0f;
char bkgrnd_str[MAX_STR_LENGTH];
HRESULT hstatus;

bool checkDeviceCaps();
void onLostDevice();
void onResetDevice();
void updateScene(float dt);
void drawScene();
void parse_udp_msg(char *udp_buff, int len);
void display_num(int num, int x_loc, int y_loc);
void drawDigit(int digit_index, float x, float y);
void draw_sprite(int matrix_id, int rnd_face, bool pink);
void init_p300_matrix();
// Helper functions.
void drawBkgd();
void drawFace(int, float, float);
void udp_debug_print(char*);
int udp_get_packet(char*);
//void parse_udp_msg(char*);
void my_printf(LPCTSTR lpszFormat, ...);
void Clear();
int my_strlen(const char* str);
void test_p300_matrix();
void update_faces_and_numbers();
float trans_x(int x);
float trans_y(int y);
void change_background_texture(LPCSTR bkgrd_string);
ID3DXSprite* mSprite;

IDirect3DTexture9* mBkgdTex;
D3DXVECTOR3 mBkgdCenter;

IDirect3DTexture9* mShipTex;
IDirect3DTexture9* mShipTex2;
IDirect3DTexture9* mFaceTex[MAX_NUMBER_OF_FACES + 1]; //we don't use the first face at index 0
IDirect3DTexture9* digitTex[10]; // we will use the first index 0 since these are for displaying numbers

D3DXVECTOR3 mShipCenter;
D3DXVECTOR3 mShipPos;
float       mShipSpeed;
float       mShipRotation;
typedef struct
{
	int id;
	char *face_file;		//this is the filename fred.raw
	char *pink_file;		//this is normal
	int face_id;          //just the mFaceTex index 

	int height;
	int width;
	int offset_x;			// this is essentially mid of the face
	int offset_y;			// this is essentially mid of the face
	unsigned short int *pink_data;
	unsigned short int *face_data;
} bci_face;

typedef struct
{
	int number; // corresponds to texture index
	int x_coord;
	int y_coord;
	bool sprite_on;
	double sprite_timer;
} netv_sprite;



typedef struct
{
	int id;
	char *name;
	int loc_x;			// this is essentially mid of the matrix element //
	int loc_y;			// this is essentially mid of the matrix element //
	int curr_face_num;		//current face number overlaying this location
	double on_1;          // last three on off times intervals
	double off_1;         // multiple p3 and face ep's?????
	double on_2;
	double off_2;
	double on_3;
	double off_3;
	double period;
	double first_on;     // master clock when first flashed
	double ssvep_period; // if period is zero then ignore
	bool face_on;          // is face on or off
	int digit1_id; //left most digit
	int digit2_id; // right most digit
	double number_first_on;
	double number_timer; // if it is positive then show number
} bci_matrix;

// this is where we store all  face stuff, [0] isn't used 
bci_face p300_face[MAX_NUMBER_OF_FACES + 1];
// this is where we store all matrix stuff, [0] isn't used 
bci_matrix p300_matrix[MAX_NUMBER_OF_MATRIX_ELEMENTS + 1];

netv_sprite number_display[NUMBER_OF_NUMBERS];
// client and server sockets, the client is for debug into since directx doesn't have a console
SOCKET socketC;
struct sockaddr_in serverInfo;

SOCKET socketS;
struct sockaddr_in local;
struct sockaddr_in from;
int flash;



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
		tobii_button_matrix[i] =

	}
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


gameTime = new GameTime();
if (!gameTime->Initialize())
{
	return false;
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
	master_clock = gameTime->totalGameTime; // update masterclock
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

/////////////////////////////////////////////////////////////// adding spritedemo code


