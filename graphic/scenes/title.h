#ifndef GRAPHIC_SCENES_TITLE_H
#define GRAPHIC_SCENES_TITLE_H

#include "proto.h"
#include <array>
#include <cstdint>
#include "../text.h"
#include "../../mjcore/strcode.h"

namespace mihajong_graphic {

class TitleScreen : public Scene {
private:
	static const unsigned nsTitleLogo = 3;
	class TitleSprite;
	std::array<TitleSprite*, nsTitleLogo> sTitleLogo;
	void clearWithGameTypeColor();
	FILETIME startTime;
	static const unsigned timePerFrame = 166667u;
	uint64_t elapsed();
	void zoomingLogo(TitleSprite* sprite, int X, int Y, unsigned startF, unsigned endF);
	TextRenderer* myTextRenderer;
	void menuLabelSlide(unsigned ID, const CodeConv::tstring& menustr, int X, int Y, unsigned startF, unsigned endF);
	void menuLabels();
	unsigned menuCursor;
public:
	TitleScreen(ScreenManipulator* const manipulator);
	~TitleScreen();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
};

class TitleScreen::TitleSprite {
private:
	static LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE sprite;
	RECT rect;
	int width, height;
public:
	static void LoadTexture(LPDIRECT3DDEVICE9 device);
	static void DisposeTexture();
	TitleSprite(LPDIRECT3DDEVICE9 device, int X, int Y, int Width, int Height);
	~TitleSprite();
	void show(int X, int Y, float scale = 1.0f, uint8_t opacity = 0xff);
};

}
#endif
