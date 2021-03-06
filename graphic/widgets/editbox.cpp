﻿#include "editbox.h"
#include <cmath>
#include "../extchar.h"
#include "../resource.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../chrwidth.h"
#include <cassert>
#include <algorithm>
#include <stdexcept>
#ifndef _WIN32
#include <X11/keysym.h>
#include "../keycode.h"
#endif /*_WIN32*/

namespace mihajong_graphic {

using namespace character_width;

#ifdef _WIN32
EditBox::EditBox(HWND hwnd, DevicePtr device, int X, int Y, unsigned width, float scale)
#else /*_WIN32*/
EditBox::EditBox(Window hwnd, DevicePtr device, int X, int Y, unsigned width, float scale)
#endif /*_WIN32*/
{
	assert(width >= 8);
	myHWnd = hwnd; myDevice = device;
	myRegion = std::make_tuple(X, Y, width);
	myTextRenderer = new SmallTextRenderer(device);
#if defined(_WIN32) && defined(WITH_DIRECTX)
	D3DXCreateLine(device, &cursorLine);
#endif
	maxStr = 0u; cursorPos = 0u; scrollPos = 0u;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_TEXTBOX));
	isActive = false;
	myScale = scale;
}

EditBox::~EditBox() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (myTexture) myTexture->Release();
	if (cursorLine) cursorLine->Release();
#endif
	if (myTextRenderer) delete myTextRenderer;
}

TransformMatrix EditBox::getMatrix(int X, int Y, unsigned width) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix mat, mat1; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat1);
	D3DXMatrixTranslation(&mat, -X, -Y, 0.0f);
	D3DXMatrixScaling(&mat1, (float)(width * halffontsz) / 77.0f, 1.0f, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, myScale, myScale, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixTranslation(&mat1, X, Y, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
#else
	glPushMatrix(); glLoadIdentity();
	glTranslatef(0.0f, (float)Geometry::WindowHeight, 0.0f);
	glTranslatef((float)X * Geometry::WindowScale(), -(float)Y * Geometry::WindowScale(), 0.0f);
	glScalef(myScale, myScale, 1.0f);
	glTranslatef(-(float)X * Geometry::WindowScale(), (float)Y * Geometry::WindowScale(), 0.0f);
	glScalef(Geometry::WindowScale(), Geometry::WindowScale(), 1.0f);
	glTranslatef(0.0f, -(float)Geometry::WindowHeight, 0.0f);
	TransformMatrix mat; glGetFloatv(GL_MODELVIEW_MATRIX, &mat[0]);
	glPopMatrix();
#endif
	return mat;
}

void EditBox::renderFrame(int X, int Y, unsigned width) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix matrixScale; D3DXMatrixIdentity(&matrixScale); TransformMatrix matrixScale1; D3DXMatrixIdentity(&matrixScale1);
	D3DXMatrixTranslation(&matrixScale, (float)(-X), (float)(-Y), 0.0f);
	D3DXMatrixScaling(&matrixScale1, myScale, myScale, 1.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
	D3DXMatrixTranslation(&matrixScale1, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
	D3DXMatrixScaling(&matrixScale1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
#else
	glPushMatrix(); glLoadIdentity();
	glTranslatef(0.0f, (float)Geometry::WindowHeight, 0.0f);
	glTranslatef((float)X * Geometry::WindowScale(), -(float)Y * Geometry::WindowScale(), 0.0f);
	glScalef(myScale, myScale, 1.0f);
	glTranslatef(-(float)X * Geometry::WindowScale(), (float)Y * Geometry::WindowScale(), 0.0f);
	glScalef(Geometry::WindowScale(), Geometry::WindowScale(), 1.0f);
	glTranslatef(0.0f, -(float)Geometry::WindowHeight, 0.0f);
	TransformMatrix matrixScale; glGetFloatv(GL_MODELVIEW_MATRIX, &matrixScale[0]);
	glPopMatrix();
#endif

	RECT rect; rect.left = 0; rect.right = 5;
	if (isActive) {rect.top = 28; rect.bottom = 56;}
	else {rect.top = 0; rect.bottom = 28;}
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X - 5, Y - 5, 5, 28, 0xffffffff, &rect, 0, 0, &matrixScale);
	rect.left = 5; rect.right = 82;
	TransformMatrix mat = getMatrix(X, Y, width);
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y - 5, width * halffontsz, 28, 0xffffffff, &rect, 0, 0, &mat);
	rect.left = 82; rect.right = 87;
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X + width * halffontsz, Y - 5, 5, 28, 0xffffffff, &rect, 0, 0, &matrixScale);
}

void EditBox::renderIMCandidateFrame(int X, int Y, unsigned width, unsigned lines) {
	assert(lines != 0);
	RECT rect;
	unsigned spriteNum = 0u;
	auto drawLine = [&rect, &spriteNum, X, Y, width, this](int y) -> void {
#if defined(_WIN32) && defined(WITH_DIRECTX)
		TransformMatrix matrixScale; D3DXMatrixIdentity(&matrixScale); TransformMatrix matrixScale1; D3DXMatrixIdentity(&matrixScale1);
		D3DXMatrixTranslation(&matrixScale, (float)(-X), (float)(-Y), 0.0f);
		D3DXMatrixScaling(&matrixScale1, myScale, myScale, 1.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
		D3DXMatrixTranslation(&matrixScale1, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
		D3DXMatrixScaling(&matrixScale1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
#else
		glPushMatrix(); glLoadIdentity();
		glTranslatef(0.0f, (float)Geometry::WindowHeight, 0.0f);
		glTranslatef((float)X * Geometry::WindowScale(), -(float)Y * Geometry::WindowScale(), 0.0f);
		glScalef(myScale, myScale, 1.0f);
		glTranslatef(-(float)X * Geometry::WindowScale(), (float)Y * Geometry::WindowScale(), 0.0f);
		glScalef(Geometry::WindowScale(), Geometry::WindowScale(), 1.0f);
		glTranslatef(0.0f, -(float)Geometry::WindowHeight, 0.0f);
		TransformMatrix matrixScale; glGetFloatv(GL_MODELVIEW_MATRIX, &matrixScale[0]);
		glPopMatrix();
#endif

		rect.left = 0; rect.right = 5;
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X - 5, Y + y, 5, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &matrixScale);
		rect.left = 5; rect.right = 82;
		TransformMatrix mat = getMatrix(X, Y, width);
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y + y, width * halffontsz, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &mat);
		rect.left = 82; rect.right = 87;
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X + width * halffontsz, Y + y, 5, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &matrixScale);
	};
	rect.top = 28 + 0; rect.bottom = 28 + 5; drawLine(-5);
	rect.top = 28 + 4; rect.bottom = 28 + 24;
	for (unsigned i = 0; i < lines; i++) drawLine(20 * i);
	rect.top = 28 + 24; rect.bottom = 28 + 28; drawLine(20 * lines);
}

unsigned EditBox::scrollRBound(IMStat& imStat) {
	bool imeflag = imStat.isOpened();
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	unsigned cols = 0;
	CodeConv::tstring s(myText.substr(0, cursorPos) + imStat.getGCSCompStr() + myText.substr(cursorPos, myText.size()));
	for (unsigned i = scrollPos; i < s.size(); i++) {
		cols += isFullWidth(s[i]) ? 2 : 1;
		if (cols > width) return i;
		else if (cols == width) return i + 1;
	}
	return s.size();
}

void EditBox::renderNormalText(IMStat& imStat, unsigned start, unsigned end, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	unsigned start_ = start, end_ = end;
	if (start < cursorPos) {
		if ((start_ += scrollPos) >= end_) return; // Do nothing if completely out of frame
	} else {
		if (cols >= width) return; // Do nothing if completely out of frame
	}
	const unsigned startcol = cols;
	for (unsigned i = start_; i < end_; i++) {
		if (i == cursorPos) cursorcol = cols;
		cols += isFullWidth(myText[i]) ? 2 : 1;
		if (cols > width) end_ = i;
		else if (cols == width) end_ = i + 1;
	}
	if (isLeadingByte(myText, end_ - 1)) --end_;
	myTextRenderer->NewText(TextID++, myText.substr(start_, end_ - start_), X + startcol * myScale * halffontsz, Y, myScale, 1.0f, 0xffffffff);
}

void EditBox::renderIMText(IMStat& imStat, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	unsigned startcol = 0u, startchr = 0u;
#ifdef _WIN32
	BYTE tmpInfo;
#else /*_WIN32*/
	char tmpInfo;
#endif /*_WIN32*/
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	CodeConv::tstring convStr(imStat.getGCSCompStr());
#ifdef _WIN32
	std::vector<BYTE> charInfo(imStat.getGCSCompAttr());
#else /*_WIN32*/
	std::vector<char> charInfo(imStat.getGCSCompAttr());
#endif /*_WIN32*/
	int imCursor(imStat.getGCSCursorPos());
	startcol = cols;
	if (!convStr.empty()) cursorcol = -1;
	unsigned strStartPos = 0u;
	if (cursorPos < scrollPos) strStartPos = startchr = (scrollPos - cursorPos);
	for (unsigned i = strStartPos; i <= convStr.size(); i++) {
		if (convStr.empty()) break;
		if (i == imCursor) cursorcol = cols;
		if ((cols >= width) || (i == convStr.size()) || ((i > strStartPos) && (i < charInfo.size()) && (tmpInfo != charInfo[i]))) {
			ArgbColor color;
			switch (tmpInfo) {
#ifdef _WIN32
			case ATTR_INPUT:
				color = 0xff00ccff; break;
			case ATTR_CONVERTED:
				color = 0xff00cc33; break;
			case ATTR_TARGET_CONVERTED:
				color = 0xffff6600; break;
			case ATTR_TARGET_NOTCONVERTED:
				color = 0xff9900ff; break;
#endif /*_WIN32*/
			default:
				color = 0xffff0000; break;
			}
			if (isLeadingByte(convStr, i - 1)) --i;
			myTextRenderer->NewText(TextID++, convStr.substr(startchr, i - startchr), X + startcol * myScale * halffontsz, Y, myScale, 1.0f, color);
			startcol = cols; startchr = i;
			if ((cols >= width) || (i == convStr.size())) break;
		}
		if (i < charInfo.size()) tmpInfo = charInfo[i];
		else tmpInfo = -1;
		cols += isFullWidth(convStr[i]) ? 2 : 1;
	}
}

void EditBox::renderIMCandidates(IMStat& imStat, int X, int Y, unsigned& TextID) {
	unsigned candidateNum, pageStart, pageSize;
	std::vector<CodeConv::tstring> candidates;
	std::tie(candidateNum, candidates, pageStart, pageSize) = imStat.getCandidateList();
	unsigned wndcols = 0;
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		unsigned tmpcols = 0;
		for (const auto& k : candidates[i])
			tmpcols += isFullWidth(k) ? 2: 1;
		if (wndcols < tmpcols) wndcols = tmpcols;
	}
	if (!candidates.empty()) {
		CodeConv::tostringstream o; o << _T("(") << (candidateNum + 1) << _T(" / ") << candidates.size() << _T(")");
		if ((wndcols + 4) < o.str().size()) wndcols = o.str().size() - 4;
		renderIMCandidateFrame(X, Y, wndcols + 4, pageSize + 1);
		myTextRenderer->NewText(TextID++, o.str(), X, Y, myScale, 1.0f, 0xffcccccc);
	}
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		CodeConv::tostringstream o;
		o << (i - pageStart + 1) << ". " << candidates[i];
		myTextRenderer->NewText(TextID++, o.str(), X, Y + 20 * myScale * (i - pageStart + 1), myScale, 1.0f,
			(candidateNum == i) ? 0xffff6600 : 0xffffffff);
	}
}

void EditBox::renderCursor(IMStat& imStat, int X, int Y, signed& cursorcol) {
	ArgbColor color =
		(imStat.isOpened() ? 0xffcc0000 : 0xff000066) |
		((int)(sin((double)myTimer.elapsed() / 200000.0) * 96.0 + 120.0) << 8);
#if defined(_WIN32) && defined(WITH_DIRECTX)
	D3DXVECTOR2 vec[] = {
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * myScale * halffontsz), Geometry::WindowScale() * Y),
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * myScale * halffontsz), Geometry::WindowScale() * (Y + int(18.0f * myScale))),
	};
	cursorLine->SetWidth(2);
	cursorLine->Begin();
	cursorLine->Draw(vec, 2, color);
	cursorLine->End();
#else
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); glLoadIdentity(); // ここで行列をリセットしておかないとおかしな事になる
	glLineWidth(2);
	glColor4d(
		(double)((color & 0x00ff0000) >> 16) / 255.0,
		(double)((color & 0x0000ff00) >>  8) / 255.0,
		(double)((color & 0x000000ff)      ) / 255.0,
		(double)((color & 0xff000000) >> 24) / 255.0);
	glBegin(GL_LINES);
	glVertex2f((X + (float)cursorcol * myScale * (float)halffontsz) * Geometry::WindowScale(), Geometry::WindowHeight - Y * Geometry::WindowScale());
	glVertex2f((X + (float)cursorcol * myScale * (float)halffontsz) * Geometry::WindowScale(), Geometry::WindowHeight - (Y + int(18.0f * myScale)) * Geometry::WindowScale());
	glEnd();
	glFlush();
	glPopMatrix();
#endif
}

void EditBox::scroll(IMStat& imStat) {
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	const CodeConv::tstring s(myText.substr(0, cursorPos) + (isActive ? imStat.getGCSCompStr() : CodeConv::tstring()) + myText.substr(cursorPos, myText.size()));
	const auto compAttr(imStat.getGCSCompAttr());
	const int paragraphLength =
#ifdef _WIN32
		std::count_if(compAttr.begin(), compAttr.end(), [](BYTE p) {
			return ((p == ATTR_TARGET_CONVERTED) || (p == ATTR_TARGET_NOTCONVERTED));
		}
	);
#else /*_WIN32*/
		0;
#endif /*_WIN32*/
	const unsigned trueRBound = (scrollRBound(imStat) < paragraphLength) ? 0 : (scrollRBound(imStat) - paragraphLength);
	unsigned textRightmostToFill = s.size(), fillCols = 0u;
	for (; textRightmostToFill > 0; textRightmostToFill--) {
		if (isFullWidth(s[textRightmostToFill - 1])) fillCols += 2;
		else fillCols += 1;
		if (fillCols >= width) {textRightmostToFill--; break;}
	}
	if (fillCols > width) textRightmostToFill++;
	if ((textRightmostToFill > 0 ) && (isLeadingByte(s, textRightmostToFill - 1))) textRightmostToFill++;

	while (true) {
		const unsigned trueCursorPos = cursorPos + ((imStat.getGCSCursorPos() < 0) ? 0 : imStat.getGCSCursorPos());
		if (trueCursorPos > (scrollRBound(imStat) - paragraphLength)) {
			try {
				if (isLeadingByte(s, scrollPos)) ++scrollPos;
				++scrollPos;
			} catch (std::out_of_range&) {}
		} else if (trueCursorPos < scrollPos) {
			try {
				--scrollPos;
				if (isLeadingByte(s, scrollPos - 1)) --scrollPos;
			} catch (std::out_of_range&) {}
		} else if (scrollPos > textRightmostToFill) {
			scrollPos = textRightmostToFill;
			break;
		} else break;
	}
}

void EditBox::Render() {
	/* Initialize */
	int X, Y; unsigned width; std::tie(X, Y, width) = myRegion;
	unsigned TextID = 0u, cols = 0u; signed cursorcol = -1;
#ifdef _WIN32
	IMStat imStat(myHWnd);
#else /*_WIN32*/
	IMStat imStat(nullptr);
#endif /*_WIN32*/

	/* Textbox */
	renderFrame(X, Y, width);

	/* Text */
	scroll(imStat);
	renderNormalText(imStat, 0u, cursorPos, X, Y, TextID, cols, cursorcol);
	if (isActive) renderIMText(imStat, X, Y, TextID, cols, cursorcol);
	renderNormalText(imStat, cursorPos, myText.size(), X, Y, TextID, cols, cursorcol);
	if (cursorcol == -1) cursorcol = cols;

	/* Candidate words */
	if (isActive) {
		renderIMCandidates(imStat, X + cursorcol * halffontsz, Y + 20 * myScale, TextID);
		renderIMCandidates(imStat, X + cursorcol * halffontsz, Y + 20 * myScale, TextID);
	}

	/* Commit */
	for (unsigned i = TextID; i < maxStr; i++)
		myTextRenderer->DelText(i);
	maxStr = TextID;
	myTextRenderer->Render();

	/* Cursor */
	if (isActive)
		renderCursor(imStat, X, Y, cursorcol);
}

#ifdef _WIN32
void EditBox::KeyboardInput(WPARAM wParam, LPARAM lParam)
#else /*_WIN32*/
void EditBox::KeyboardInput(const XEvent* od)
#endif /*_WIN32*/
{
	if (!isActive) return;
#ifdef _WIN32
	if (wParam == CHARDAT_CURSOR_LEFT)
#else /*_WIN32*/
	if (od->type != KeyPress) return;
	if (od->xkey.keycode == DIK_LEFT)
#endif /*_WIN32*/
	{ // Cursor key
		if (cursorPos > 0) --cursorPos;
		try {
			if (isLeadingByte(myText, cursorPos - 1)) --cursorPos;
		} catch (std::out_of_range&) {}
	}
#ifdef _WIN32
	else if (wParam == CHARDAT_CURSOR_RIGHT)
#else /*_WIN32*/
	else if (od->xkey.keycode == DIK_RIGHT)
#endif /*_WIN32*/
	{ // Cursor key
		try {
			if (isLeadingByte(myText, cursorPos)) ++cursorPos;
			if (cursorPos < myText.size()) ++cursorPos;
		} catch (std::out_of_range&) {}
	}
#ifdef _WIN32
	else if (wParam == CHARDAT_CURSOR_ENTER)
#else /*_WIN32*/
	else if (od->xkey.keycode == DIK_RETURN)
#endif /*_WIN32*/
	{ // Enter key
		/* Do nothing */
	}
#ifdef _WIN32
	else if (wParam == CHARDAT_CURSOR_ESCAPE)
#else /*_WIN32*/
	else if (od->xkey.keycode == DIK_ESCAPE)
#endif /*_WIN32*/
	{ // Escape key
		/* Do nothing */
	} else {
#ifdef _WIN32
		WCHAR Letter[2] = {(WCHAR)wParam, 0};
#else /*_WIN32*/
		volatile char keyStr[256];
		memset(const_cast<char*>(keyStr), 0, 256);
		XLookupString(const_cast<XKeyEvent*>(reinterpret_cast<const XKeyEvent*>(od)),
			const_cast<char*>(keyStr), 256, nullptr, nullptr);
		std::wstring wst([](char* s) -> std::wstring {
			try {
				return CodeConv::UTF8toWIDE(s);
			} catch (...) {
				return std::wstring();
			}
		}(const_cast<char*>(keyStr)));
		wchar_t* Letter_ = new wchar_t[wst.size() + 1];
		wcsncpy(Letter_, wst.c_str(), wst.size() + 1);
		for (wchar_t* Letter = Letter_; *Letter != L'\0'; ++Letter) {
#endif /*_WIN32*/
		if ((*Letter >= L' ') && (*Letter != L'\x7f')) { // Ordinary
			CodeConv::tstring currentInput = CodeConv::EnsureTStr(std::wstring(Letter));
			myText = myText.substr(0, cursorPos) + currentInput + myText.substr(cursorPos, myText.size());
			++cursorPos;
		} else if ((*Letter == L'\b') && (!myText.empty())) { // Backspace
			if (cursorPos > 0) {
				try {
					myText = myText.substr(0, cursorPos - 1) + myText.substr(cursorPos, myText.size());
					--cursorPos;
					if (isLeadingByte(myText, cursorPos - 1)) {
						myText = myText.substr(0, cursorPos - 1) + myText.substr(cursorPos, myText.size());
						--cursorPos;
					}
				} catch (std::out_of_range&) {}
			}
		}
#ifndef _WIN32
		}
		delete[] Letter_;
#endif /*_WIN32*/
	}
}

#ifdef _WIN32
void EditBox::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	if (!isActive) return;
	if ((message == WM_IME_COMPOSITION) && (lParam & GCS_RESULTSTR)) {
		IMStat imStat(myHWnd);
		CodeConv::tstring resultStr(imStat.getGCSResultStr());
		myText = myText.substr(0, cursorPos) + resultStr + myText.substr(cursorPos, myText.size());
		cursorPos += resultStr.size();
	}
}
#else /*_WIN32*/
/* TODO: Linuxでは日本語入力が未実装 */
#endif /*_WIN32*/

void EditBox::setText(const CodeConv::tstring& newstr) {
	myText = newstr;
	cursorPos = myText.length();
	scrollPos = 0;
}

// -------------------------------------------------------------------------

#ifdef _WIN32
EditBox::IMStat::IMStat(HWND hWnd) {
	hwnd = hWnd;
	hIMC = ImmGetContext(hwnd);
}

EditBox::IMStat::~IMStat() {
	ImmReleaseContext(hwnd, hIMC);
}

bool EditBox::IMStat::isOpened() {
	return (bool)ImmGetOpenStatus(hIMC);
}

std::tuple<DWORD, DWORD> EditBox::IMStat::getConvStat() {
	DWORD fdwConversion, fdwSentence;
	ImmGetConversionStatus(hIMC, &fdwConversion, &fdwSentence);
	return std::make_tuple(fdwConversion, fdwSentence);
}

std::vector<BYTE> EditBox::IMStat::getCompositionString(DWORD InfoType) {
	std::vector<BYTE> data;
	DWORD bufSize = (DWORD)ImmGetCompositionString(hIMC, InfoType, nullptr, 0);
	if ((bufSize == IMM_ERROR_NODATA) || (bufSize == IMM_ERROR_GENERAL)) {
		return std::vector<BYTE>();
	} else if (bufSize) {
		data.resize(bufSize);
		ImmGetCompositionString(hIMC, InfoType, &data[0], bufSize);
	}
	return data;
}

CodeConv::tstring EditBox::IMStat::getGCSCompStr() {
	std::vector<BYTE> data(getCompositionString(GCS_COMPSTR));
	for (int i = 0; i < sizeof(TCHAR); i++)
		data.push_back(0);
	return CodeConv::tstring(reinterpret_cast<LPTSTR>(&(data[0])));
}

std::vector<BYTE> EditBox::IMStat::getGCSCompAttr() {
	return getCompositionString(GCS_COMPATTR);
}

CodeConv::tstring EditBox::IMStat::getGCSResultStr() {
	std::vector<BYTE> data(getCompositionString(GCS_RESULTSTR));
	for (int i = 0; i < sizeof(TCHAR); i++)
		data.push_back(0);
	return CodeConv::tstring(reinterpret_cast<LPTSTR>(&(data[0])));
}

int EditBox::IMStat::getGCSCursorPos() {
	return ImmGetCompositionString(hIMC, GCS_CURSORPOS, nullptr, 0);
}

std::tuple<unsigned, std::vector<CodeConv::tstring>, unsigned, unsigned> EditBox::IMStat::getCandidateList() {
	DWORD BufSize = ImmGetCandidateList(hIMC, 0, nullptr, 0);
	BYTE* buf = new BYTE[BufSize];
	if (ImmGetCandidateList(hIMC, 0, reinterpret_cast<LPCANDIDATELIST>(buf), BufSize)) {
		unsigned selection = reinterpret_cast<LPCANDIDATELIST>(buf)->dwSelection;
		unsigned count = reinterpret_cast<LPCANDIDATELIST>(buf)->dwCount;
		unsigned start = reinterpret_cast<LPCANDIDATELIST>(buf)->dwPageStart;
		unsigned pagesize = reinterpret_cast<LPCANDIDATELIST>(buf)->dwPageSize;
		std::vector<CodeConv::tstring> candidates; candidates.resize(count);
		for (unsigned i = 0; i < count; i++)
			candidates[i] = CodeConv::tstring((LPTSTR)(buf + (reinterpret_cast<LPCANDIDATELIST>(buf)->dwOffset[i])));
		delete[] buf;
		return std::make_tuple(selection, candidates, start, pagesize);
	} else {
		delete[] buf;
		return std::make_tuple(0, std::vector<CodeConv::tstring>(), 0, 0);
	}
}
#endif /*_WIN32*/

}
