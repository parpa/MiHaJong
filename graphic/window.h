#ifndef GRAPHIC_WINDOW_H
#define GRAPHIC_WINDOW_H

#include <Windows.h>
#include <tchar.h>
#include "scrmanip.h"

class MainWindow {
private:
	HWND hWnd;
	static ScreenManipulator* myScreenManipulator; // DirectX �I�u�W�F�N�g
	static const LPTSTR myWindowClassName, WindowCaption;
	static const unsigned WindowWidth, WindowHeight;
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // �E�B���h�E�v���V�[�W��
	void initWindowClass(HINSTANCE hThisInst); // �E�B���h�E�N���X�̏�����
	void initWindow(HINSTANCE hThisInst, int nWinMode); // �E�B���h�E�̐���
public:
	MainWindow(HINSTANCE hThisInst, int nWinMode);
	~MainWindow();
	void Render();
};

#endif