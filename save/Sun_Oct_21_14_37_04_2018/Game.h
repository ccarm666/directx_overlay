#ifndef GAME_H
#define GAME_H

#include "GraphicsDevice.h"
#include "GameplayObject.h"
#include "GameTime.h"
#define MAX_NUMBER_OF_FACES 6
#define MAX_NUMBER_OF_BUTTONS 100

typedef struct
{
	int id;
	float loc_x;			// this is essentially mid of the matrix element //
	float loc_y;			// this is essentially mid of the matrix element //
	int curr_face_num;		//current face number overlaying this location
	GameplayObject *button_face;
	bool face_on;          // is face on or off
	int digit1_id; // left most digit
	int digit2_id; // right most digit
	GameplayObject *button_num_1;
	GameplayObject *button_num_2;
	double number_first_on;
	double number_timer; // if it is positive then show number
} button;

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
	bool Game::Init_tobii_button_matrix();
	void Game::InitWinsock();
	void Game::my_printf(LPCTSTR lpszFormat, ...);
	void Game::test_tobii_button_matrix();
	void Game::udp_debug_print(char* msg);
	int Game::udp_get_packet(char* buffer);
	void Game::parse_udp_msg(char *udp_buff, int len);
	void Game::change_background_texture(char* buffer);
	void Game::Clear();
	void Game::setDigits(int buttonId);
private:
	GraphicsDevice *gDevice;
	//GameplayObject *player;
	//GameplayObject *player2;
	GameplayObject *tobii_menu;
//	GameplayObject *button_num_1[MAX_NUMBER_OF_BUTTONS];
//	GameplayObject *button_num_2[MAX_NUMBER_OF_BUTTONS];
//	GameplayObject *button_faces[MAX_NUMBER_OF_BUTTONS];
	GameSprite *faces[MAX_NUMBER_OF_FACES + 1]; // index zero is not used
	GameSprite *backGround;
	GameSprite *digit[10];
	GameTime *gameTime;
//	GameplayObject *tface;
	// this is where we store all matrix stuff, [0] isn't used 
	button tobii_button_matrix[MAX_NUMBER_OF_BUTTONS+1];
	SOCKET socketC;
	struct sockaddr_in serverInfo;
	SOCKET socketS;
	struct sockaddr_in local;
	struct sockaddr_in from;
	int curr_matrix_element_id;
	int flash_list[MAX_NUMBER_OF_BUTTONS + 1]; //this vector starts at zero
	int num_flash_items;
};

#endif /* GAME_H */