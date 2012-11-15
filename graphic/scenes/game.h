#ifndef GRAPHIC_SCENES_GAME_H
#define GRAPHIC_SCENES_GAME_H

#include "proto.h"

class TableProtoScene : public Scene {
protected:
	LPDIRECT3DTEXTURE9 tSideBar; LPD3DXSPRITE sSideBar; // �T�C�h�o�[
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPTSTR filename, unsigned width, unsigned height); // �e�N�X�`���ǂݍ���
	void InitSprite(LPD3DXSPRITE* sprite); // �X�v���C�g������
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // �X�v���C�g�\��
	void ShowSidebar();
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	virtual ~TableProtoScene();
};

#endif