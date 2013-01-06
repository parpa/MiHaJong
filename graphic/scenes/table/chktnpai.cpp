#include "chktnpai.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

TableSubsceneCheckTenpai::TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device) : TableSubsceneCallFadeProto(device) {
	txtRenderer = new TextRenderer(device);
	tileRenderer = new ShowTile(device);
}

TableSubsceneCheckTenpai::~TableSubsceneCheckTenpai() {
	delete tileRenderer;
	delete txtRenderer;
}

void TableSubsceneCheckTenpai::ShowTenpaiFlag(PLAYER_ID player, int x, int y) {
	bool tenpaiflag = utils::isTenpai(GameStatus::gameStat(), player);
	MachihaiInfo machiInfo = utils::chkFuriten(GameStatus::gameStat(), player);
	if (tenpaiflag) { // 聴牌
		ShowCallMsg(player, calltext::Tenpai, x, y - 40);
		int tile = 0;
		for (tileCode k = CharacterOne; k <= RedDragon; k = (tileCode)(k + 1)) {
			if (machiInfo.Machihai.val[k].MachihaiFlag) // 待ち牌になっている場合
				tileRenderer->NewTile(player * 9 + tile, k, Normal,
				x - 20 * (machiInfo.MachiMen - 1) + 40 * (tile++),
				y + 20, Portrait, Obverse); // その牌を表示
		}
		if (machiInfo.FuritenFlag)
			txtRenderer->NewText(player, _T("(フリテン)"), x - 90, y + 40);
		else
			txtRenderer->NewText(player, _T("待ち"), x - 36, y + 40);
	}
	else if (GameStatus::gameStat()->Player.val[player].RichiFlag.RichiFlag)
		ShowCallMsg(player, calltext::Chonbo, x, y); // ノーテンリーチしてた場合は錯和と表示
	else ShowCallMsg(player, calltext::Noten, x, y); // 不聴
}

void TableSubsceneCheckTenpai::Render() {
	tileRenderer->Render();
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
	txtRenderer->Render();
}

}
