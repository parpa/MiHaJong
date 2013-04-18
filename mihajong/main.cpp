#include "main.h"

#include <cstdlib>
#include "../mjcore/mjimport.h"
#include "../graphic/graphic.h"
#include "resource.h"

extern const GameTypeID myGameType;

#ifdef _WIN32

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	MSG msg; HWND hwnd;
	unsigned width, height; bool fullscreen;

	/* ウィンドウ設定情報を先行読み込み */
	preferenceInit();
	getWindowSize(&width, &height, &fullscreen);

	/* ウィンドウを初期化する */
	if (!mihajong_graphic::InitWindow(hInstance, nCmdShow, MAKEINTRESOURCE(IDI_ICON1), &hwnd, width, height, fullscreen))
		exit(1); // 失敗したら終了
	
	/* スタート */
	StartGame(myGameType, hwnd);

	/* メインループ */
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
			// メッセージがあればメッセージの処理
			if (!GetMessage(&msg, nullptr, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// アイドル時に再描画
			mihajong_graphic::RefreshWindow();
			Sleep(1);
		}
	}
	
	/* 終了処理 */
	TerminateGame();
	ExitProcess(0);
	return msg.wParam;
}


#else /*_WIN32*/

int main(int argc, char* argv) {
	/* TODO: 未実装箇所 */
}

#endif /*_WIN32*/