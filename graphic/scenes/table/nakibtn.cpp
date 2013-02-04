#include "nakibtn.h"

#include "../../geometry.h"
#include "../../utils.h"
#include "../../gametbl.h"
#include "../../rule.h"

namespace mihajong_graphic {

const GameTableScreen::ButtonReconst::BtnData
	GameTableScreen::ButtonReconst::buttonDat[2][GameTableScreen::ButtonReconst::btnMAXIMUM] = {
		{
			{_T("左チー"),   5 + 117 * 0, Geometry::BaseSize - 40, 0xffccff66},
			{_T("嵌チー"),   5 + 117 * 1, Geometry::BaseSize - 40, 0xff99ff99},
			{_T("右チー"),   5 + 117 * 2, Geometry::BaseSize - 40, 0xff66ff99},
			{_T("ポン"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("パス"),     5 + 117 * 5, Geometry::BaseSize - 40, 0xffcccccc},
			{_T("ロン"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffcc66},
		}, {
			{_T(""),         5 + 117 * 0, Geometry::BaseSize - 40, 0xff666666},
			{_T(""),         5 + 117 * 1, Geometry::BaseSize - 40, 0xff666666},
			{_T("九種九牌"), 5 + 117 * 2, Geometry::BaseSize - 40, 0xff99ffff},
			{_T("花牌"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("リーチ"),   5 + 117 * 5, Geometry::BaseSize - 40, 0xffff66ff},
			{_T("ツモ"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffff66},
		},
};

void GameTableScreen::ButtonReconst::Render() {
	buttons->Render();
	buttons->Render();
}

void GameTableScreen::ButtonReconst::reconstruct(ButtonID buttonID) {
	union Color { // とりあえずここに仮置き
		D3DCOLOR rgbaAsOneValue;
		/* 当面はスモールエンディアンのみ対応 */
		struct {unsigned b:8, g:8, r:8, a:8;} rgbaAsStruct;
	};
	static_assert(sizeof (Color) == 4, "union Color is not 32-bit long");
	Color btnColor; btnColor.rgbaAsOneValue = buttonDat[currentButtonSet][buttonID].color;
	if (!buttonEnabled[buttonID]) { // 暗転処理
		btnColor.rgbaAsStruct.r /= 3;
		btnColor.rgbaAsStruct.g /= 3;
		btnColor.rgbaAsStruct.b /= 3;
	}
	buttons->setButton(buttonID, (cursor == buttonID) ? ButtonPic::raised : ButtonPic::clear,
		buttonDat[currentButtonSet][buttonID].x * Geometry::WindowScale(),
		buttonDat[currentButtonSet][buttonID].y * Geometry::WindowScale(),
		117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
		btnColor.rgbaAsOneValue, buttonDat[currentButtonSet][buttonID].label);
	const Region nullRegion = {0, 0, -1, -1};
	if (caller->regions.size() <= (buttonID + ButtonRegionNum))
		caller->regions.resize(ButtonRegionNum + 1 + buttonID, nullRegion);
	caller->regions[buttonID + ButtonRegionNum].Left   = buttonDat[currentButtonSet][buttonID].x;
	caller->regions[buttonID + ButtonRegionNum].Top    = buttonDat[currentButtonSet][buttonID].y + 117;
	caller->regions[buttonID + ButtonRegionNum].Right  = buttonDat[currentButtonSet][buttonID].x;
	caller->regions[buttonID + ButtonRegionNum].Bottom = buttonDat[currentButtonSet][buttonID].y + 36;
}
void GameTableScreen::ButtonReconst::reconstruct() {
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		reconstruct((ButtonID)i);
}

void GameTableScreen::ButtonReconst::ChangeButtonSet(ButtonSet btnSet) {
	currentButtonSet = btnSet;
	reconstruct();
}

void GameTableScreen::ButtonReconst::enable(ButtonID buttonID) {
	buttonEnabled[buttonID] = true; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::disable(ButtonID buttonID) {
	buttonEnabled[buttonID] = false; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::enable(const std::bitset<btnMAXIMUM>& flagset) {
	buttonEnabled = flagset;
	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForDahai() { // ツモ番の時用の
	currentButtonSet = btnSetTsumo; buttonEnabled.reset(); // 状態をリセット
	const GameTable* const gameStat = GameStatus::retrGameStat();
	const PLAYER_ID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerTable* const playerStat = &(gameStat->Player.val[ActivePlayer]);
	const SHANTEN Shanten = utils::calcShanten(gameStat, ActivePlayer, ShantenAnalyzer::shantenAll);
	if (utils::isRichiReqSatisfied(gameStat, ActivePlayer) && // 点棒要件を満たしている（点棒が足りている）
		(Shanten <= 0) && // テンパイしている
		(playerStat->MenzenFlag || (!rules::chkRule("riichi_shibari", "no"))) && // 門前であるか、リーチ縛りルールである
		(!playerStat->RichiFlag.RichiFlag) && // まだリーチしていない
		((gameStat->TilePointer + ((gameStat->gameType & AllSanma) ? 2 : 3)) < // 残りツモ牌が
		(gameStat->RinshanPointer - gameStat->DeadTiles - 1))) // 十分あるなら
		buttonEnabled[btnRiichi] = true; // リーチボタンを有効に

	const bool DaoPaiAbilityFlag = utils::chkdaopaiability(gameStat, ActivePlayer);
	if ((DaoPaiAbilityFlag) && (playerStat->FirstDrawFlag))
		buttonEnabled[btnKyuushu] = true; // 九種九牌ボタン

	const bool ShisanBuDa = utils::chkShisanBuDa(gameStat, ActivePlayer);
	const bool ShisiBuDa = utils::chkShisiBuDa(gameStat, ActivePlayer);
	if (((Shanten <= -1) && (playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile != NoTile)) || // 和了になっているか
		ShisanBuDa || ShisiBuDa) // 十三不塔の場合（十三不塔なしの場合この変数はfalseになる）
		buttonEnabled[btnTsumo] = true; // 和了ボタン

	reconstruct();
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	caller = parent;
	cursor = CursorDisabled;
	buttons = new ButtonPic(caller->caller->getDevice());
	ChangeButtonSet(btnSetNormal);
}

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
}

}
