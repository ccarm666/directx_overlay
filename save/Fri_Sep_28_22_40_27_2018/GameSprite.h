#ifndef GAMESPRITE_H
#define GAMESPRITE_H

//Texture and Sprites and other functions.
#include <d3dx9.h>
#include <string>

class GameSprite
{
public:
	//Constructors and Destructor
	GameSprite();
	~GameSprite();
	
	//Sprite Functions
	bool Initialize(LPDIRECT3DDEVICE9 device, std::string file, int width, int height);
	bool IsInitialized();
	virtual void Draw(float gameTime, D3DXVECTOR3 position);
private:
	//Pointers
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	//Attributes
	D3DCOLOR color;
	bool initialized;
};

#endif /* GAMESPRITE_H */