#include "init.h"
#include "event.h"

namespace mihajong_graphic {

HINSTANCE GraphicDLL = nullptr;
MainWindow* myMainWindow = nullptr;

EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon, HWND* hwndPtr) {
	/* ウィンドウの初期化 */
	try {
		myMainWindow = new MainWindow(hInstance, nCmdShow, icon);
		ui::UIEvent = new ui::UI_Event();
		if (hwndPtr) *hwndPtr = myMainWindow->gethwnd();
	}
	catch (LPTSTR e) {
		MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

EXPORT void RefreshWindow() {
	myMainWindow->Render();
}

EXPORT BOOL Transit(sceneID scene) try {
	if (!myMainWindow) throw _T("ウィンドウが初期化されていません");
	myMainWindow->transit(scene);
	return TRUE;
}
catch (LPTSTR e) {
	MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
	return FALSE;
}

EXPORT void CleanupWindow() {
	delete myMainWindow;
	delete ui::UIEvent;
}

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		mihajong_graphic::GraphicDLL = hinstDLL;
		break;
	case DLL_PROCESS_DETACH:
		mihajong_graphic::GraphicDLL = nullptr;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
