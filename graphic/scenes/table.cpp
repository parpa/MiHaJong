#include "table.h"
#include "../scrmanip.h"

GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, _T("data\\frame.png"), 768, 768); InitSprite(&sBorder);
	LoadTexture(&tBaize, _T("data\\baize.png"), 674, 674); InitSprite(&sBaize);
}

GameTableScreen::~GameTableScreen() {
	if (tBorder) tBorder->Release();
	if (sBorder) sBorder->Release();
	if (tBaize) tBaize->Release();
	if (sBaize) sBaize->Release();
}

void GameTableScreen::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // �o�b�t�@�N���A
	ShowSidebar();
	ShowSprite(sBaize, tBaize, 47, 47, 674, 674);
	ShowSprite(sBorder, tBorder, 0, 0, 768, 768);
}