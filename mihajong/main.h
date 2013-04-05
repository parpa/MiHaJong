#pragma once

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#if defined(__GNUC__) && defined(_WIN32) && defined(_UNICODE) /* Workaround for MinGW */
#define MINGW_UNICODE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
#endif
