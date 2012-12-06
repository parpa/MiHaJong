#include "ruleconf.h"
#include <cstring>
#include "../scrmanip.h"
#include "../geometry.h"
#include "../../sound/sound.h"
#include "../../mjcore/bgmid.h"
#include "../../mihajong/version.h"
#include "../event.h"
#include <iomanip>

namespace mihajong_graphic {

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	for (unsigned short i = 0; i < RULESIZE; i++)
		rulestat[i] = rules::getRule(i);
	menuCursor = 0;
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];
	rules::exportRule(RuleConfPtr);
	redrawItems();
}

RuleConfigScene::~RuleConfigScene() {
}

void RuleConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// 項目を表示
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // アス比×0.75(横幅調整用)
	unsigned itmNameCols = strwidth(prmName); // 桁数(日本語は2桁)
	D3DCOLOR baseColor = ((prmContent == _T("Ｎ／Ａ")) || (prmContent.empty()) || (rules::reqFailed(menuCursor / RULES_IN_PAGE * RULES_IN_PAGE + prmID, rulestat))) ? 0x00bfbfbf : 0x00ffffff;
	D3DCOLOR menuColor = (menuCursor % RULES_IN_PAGE == prmID) ? 0xff000000 : 0x7f000000;
	myTextRenderer->NewText(prmID * 3, prmName,
		(prmID / 20 * 720 + 50) * WidthRate, 135 + (prmID % 20) * 40, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 1, _T(":"),
		(prmID / 20 * 720 + 50 + 144) * WidthRate, 135 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 2, prmContent,
		(prmID / 20 * 720 + 50 + 162) * WidthRate, 135 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	if (regions.size() <= prmID) {
		Region nullRegion = {0, 0, -1, -1};
		regions.resize(prmID + 1, Region(nullRegion));
	}
	regions[prmID].Left = (prmID / 20 * 720 + 50); regions[prmID].Top = 135 + (prmID % 20) * 40; 
	regions[prmID].Right = (prmID / 20 * 720 + 670); regions[prmID].Bottom = regions[prmID].Top + 35; 
}

void RuleConfigScene::redrawItems() {
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // アス比×0.75(横幅調整用)
	for (int i = 0; i < RULES_IN_PAGE; i++) {
		const unsigned ItemNum = (menuCursor / RULES_IN_PAGE * RULES_IN_PAGE) + i;
		TCHAR menuitem[128]; rules::getRuleName(menuitem, 128, ItemNum);
		TCHAR itmfield[128]; rules::getRuleTxt(itmfield, 128, ItemNum, rulestat[ItemNum]);
		itemText(i, CodeConv::tstring(menuitem), CodeConv::tstring(itmfield));
	}
}

void RuleConfigScene::Render() {
	clearWithGameTypeColor(); // バッファクリア
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // アス比×0.75(横幅調整用)
	{
		myTextRenderer->NewText(123, _T("ルール設定"), 540 * WidthRate, 25, 2.0f, WidthRate, 0xffffffff);
	}
	{
		uint64_t t = elapsed();
		CodeConv::tstring caption = _T("");
		switch ((t / 50000000u) % 6) {
		case 0:
			TCHAR menuitem[128]; rules::getRuleDescription(menuitem, 128, menuCursor);
			caption = CodeConv::tstring(menuitem);
			if (caption.empty()) {
				CodeConv::tostringstream o; SYSTEMTIME Zeit; GetLocalTime(&Zeit);
				o << _T("MiHaJong version ") _T(MIHAJONG_VER) _T(" / 現在日時 ") <<
					std::setw(4) << Zeit.wYear << _T("年") <<
					std::setw(2) << Zeit.wMonth << _T("月") <<
					std::setw(2) << Zeit.wDay << _T("日 ") <<
					((Zeit.wHour / 12 == 0) ? _T("午前") : _T("午後")) <<
					std::setw(2) << (Zeit.wHour % 12) << _T("時") <<
					std::setw(2) << std::setfill(_T('0')) << Zeit.wMinute << _T("分");
				caption = o.str();
			}
			break;
		case 1:
			caption = CodeConv::tstring(_T("キーボード操作  ↑/↓:カーソル移動  ←/→:選択中の項目を変更"));
			break;
		case 2:
			caption = CodeConv::tstring(_T("キーボード操作  Home/End:左右カラム間の移動  PageUp/PageDown:ページ間の移動"));
			break;
		case 3:
			caption = CodeConv::tstring(_T("マウス操作  項目上で左クリック/ホイール回転:選択中の項目を変更"));
			break;
		case 4:
			caption = CodeConv::tstring(_T("マウス操作  右上見出し上でホイール回転:ページ間の移動"));
			break;
		case 5:
			caption = CodeConv::tstring(_T("通信対戦時のルール設定はホスト側の設定が適用されます"));
			break;
		}
		unsigned captionCols = strwidth(caption); // 桁数(日本語は2桁)
		myTextRenderer->NewText(120, caption,
			(720 - 9 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 955, 1.0f,
			(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate,
			((t % 50000000u) < 5000000u) ? (55u + ((t % 50000000u) / 25000u)) << 24 | 0x00ffffff : 0xffffffff);
	}
	{
		CodeConv::tostringstream o; o << _T("Page ") << std::setw(2) << (menuCursor / RULES_IN_PAGE + 1) << _T("/") << RULE_PAGES;
		CodeConv::tstring pagecaption(o.str());
		unsigned captionCols = strwidth(pagecaption); // 桁数(日本語は2桁)
		myTextRenderer->NewText(121, pagecaption,
			(1400 - 15 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 45, 0.833333f,
			(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate, 0xffffffff);
	}
	{
		TCHAR pagecap[128]; rules::getPageCaption(pagecap, 128, menuCursor / RULES_IN_PAGE);
		CodeConv::tstring pagecaption(pagecap);
		unsigned captionCols = strwidth(pagecaption); // 桁数(日本語は2桁)
		myTextRenderer->NewText(122, pagecaption,
			(1400 - 15 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 75, 0.833333f,
			(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate, 0xffffffff);
		if (regions.size() <= 40) {
			Region nullRegion = {0, 0, -1, -1};
			regions.resize(40 + 1, Region(nullRegion));
		}
		regions[40].Left = (1400 - 15 * ((captionCols > 76) ? 76 : captionCols));
		regions[40].Top = 45; regions[40].Right = 1400; regions[40].Bottom = 104; 
	}
	myTextRenderer->Render();
}

void RuleConfigScene::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	switch (od->dwOfs) {
	case DIK_UP: // 前の項目
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if (--menuCursor < 0) menuCursor = 0;
			skipto(0); redrawItems();
		}
		break;
	case DIK_DOWN: // 次の項目
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if (++menuCursor >= RULESIZE) menuCursor = RULESIZE - 1;
			skipto(0); redrawItems();
		}
		break;
	case DIK_LEFT: // 前の選択肢
		if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			while (true) {
				if ((--rulestat[menuCursor]) < 0) rulestat[menuCursor] = rules::getRuleSize(menuCursor) - 1;
				TCHAR menuitem[128]; rules::getRuleTxt(menuitem, 128, menuCursor, rulestat[menuCursor]);
				if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
			}
			redrawItems();
		}
		break;
	case DIK_RIGHT: // 次の選択肢
		if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			while (true) {
				if ((++rulestat[menuCursor]) >= rules::getRuleSize(menuCursor)) rulestat[menuCursor] = 0;
				TCHAR menuitem[128]; rules::getRuleTxt(menuitem, 128, menuCursor, rulestat[menuCursor]);
				if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
			}
			redrawItems();
		}
		break;
	case DIK_HOME: // 前のカラム
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor -= 20) < 0) menuCursor = 0;
			skipto(0); redrawItems();
		}
		break;
	case DIK_END: // 次のカラム
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor += 20) >= RULESIZE) menuCursor = RULESIZE - 1;
			skipto(0); redrawItems();
		}
		break;
	case DIK_PRIOR: // 前のページ
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor -= RULES_IN_PAGE) < 0) menuCursor = 0;
			skipto(0); redrawItems();
		}
		break;
	case DIK_NEXT: // 次のページ
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor += RULES_IN_PAGE) >= RULESIZE) menuCursor = RULESIZE - 1;
			skipto(0); redrawItems();
		}
		break;
	}
}

void RuleConfigScene::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
#if 0
	{
		CodeConv::tostringstream o;
		o << _T("(") << scaledX << _T(", ") << scaledY << _T(") ");
		if (region != -1) o << _T("Region ") << region;
		else o << _T("No Region");
		myTextRenderer->NewText(144, o.str(), 0, 1000);
	}
#endif
	auto setcursor = [&]() -> void {
		if ((region >= 0) && (region <= (RULES_IN_PAGE - 1))) {
			if (region != menuCursor % RULES_IN_PAGE) {
				sound::Play(sound::IDs::sndCursor);
				menuCursor = menuCursor / RULES_IN_PAGE * RULES_IN_PAGE + region;
				skipto(0); redrawItems();
			}
		}
	};
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
		setcursor();
		break;
	case DIMOFS_Z: // ホイールの操作
		if ((region >= 0) && (region <= (RULES_IN_PAGE - 1))) {
			setcursor();
			sound::Play(sound::IDs::sndClick);
			while (true) {
				if ((LONG)od->dwData > 0) {
					if ((--rulestat[menuCursor]) < 0) rulestat[menuCursor] = rules::getRuleSize(menuCursor) - 1;
				} else if ((LONG)od->dwData < 0) {
					if ((++rulestat[menuCursor]) >= rules::getRuleSize(menuCursor)) rulestat[menuCursor] = 0;
				}
#if 0
				{
					CodeConv::tostringstream o;
					o << _T("Wheel ") << (signed)od->dwData;
					myTextRenderer->NewText(144, o.str(), 0, 1000);
				}
#endif
				TCHAR menuitem[128]; rules::getRuleTxt(menuitem, 128, menuCursor, rulestat[menuCursor]);
				if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
			}
			redrawItems();
		} else if (region == 40) {
			sound::Play(sound::IDs::sndClick);
			if ((LONG)od->dwData > 0) {
				if ((menuCursor -= RULES_IN_PAGE) < 0) menuCursor += RULES_IN_PAGE;
			} else if ((LONG)od->dwData < 0) {
				if ((menuCursor += RULES_IN_PAGE) >= RULESIZE) menuCursor -= RULES_IN_PAGE;
			}
			skipto(0); redrawItems();
		}
		break;
	case DIMOFS_BUTTON0: // 左クリック
		if ((od->dwData) && (region >= 0) && (region <= (RULES_IN_PAGE - 1))) {
			setcursor();
			sound::Play(sound::IDs::sndClick);
			while (true) {
				if ((++rulestat[menuCursor]) >= rules::getRuleSize(menuCursor)) rulestat[menuCursor] = 0;
				TCHAR menuitem[128]; rules::getRuleTxt(menuitem, 128, menuCursor, rulestat[menuCursor]);
				if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
			}
			redrawItems();
		}
		break;
	}
}

}
