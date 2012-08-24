#include "functbl.h"

const char* aiscript::table::functable::tblname = "mihajong"; // table name

void aiscript::table::functable::inittable(lua_State* const L) {
	lua_newtable(L); // table 'mihajong'
	discardTileCode(L); // subtable 'DiscardType'
	meldCallCode(L); // subtable 'Call'
	meldTypeCode(L); // subtable 'MeldType'
	tileCode(L); // subtable 'Tile'
	lockTable(L); // mark as read-only
	lua_setglobal(L, tblname); // global table
}

int aiscript::table::functable::tableLockedErr(lua_State* const L) { // ロックされたテーブルを書き換えようとした時
	return luaL_error(L, "Attempt to modify a read-only table");
}

void aiscript::table::functable::lockTable(lua_State* const L) { // テーブルをロックする
	lua_newtable(L); // メタテーブルの用意
	lua_pushcfunction(L, tableLockedErr); lua_setfield(L, -2, "__newindex"); // 書き換え禁止用
	lua_insert(L, -2); lua_setfield(L, -2, "__index"); // 本来のテーブル
	lua_newtable(L); lua_insert(L, -2); // プロキシテーブル
	lua_setmetatable(L, -2); // メタテーブル設定
}

/* 捨牌番号の付帯情報 */
inline void aiscript::table::functable::discardTileCode(lua_State* const L) {
	lua_newtable(L);
	TableAdd(L, "Normal", (int)DiscardTileNum::Normal);
	TableAdd(L, "Ankan", (int)DiscardTileNum::Ankan);
	TableAdd(L, "Kakan", (int)DiscardTileNum::Kakan);
	TableAdd(L, "Riichi", (int)DiscardTileNum::Riichi);
	TableAdd(L, "Flower", (int)DiscardTileNum::Flower);
	TableAdd(L, "OpenRiichi", (int)DiscardTileNum::OpenRiichi);
	TableAdd(L, "Agari", (int)DiscardTileNum::Agari);
	TableAdd(L, "Kyuushu", (int)DiscardTileNum::Kyuushu);
	lockTable(L); lua_setfield(L, -2, "DiscardType");
}

/* 鳴きの種別コード */
inline void aiscript::table::functable::meldCallCode(lua_State* const L) {
	lua_newtable(L);
	TableAdd(L, "None", meldNone);
	TableAdd(L, "Ron", meldRon);
	TableAdd(L, "Kan", meldKan);
	TableAdd(L, "Pon", meldPon);
	lua_newtable(L);
	TableAdd(L, "Lower", meldChiiLower);
	TableAdd(L, "Middle", meldChiiMiddle);
	TableAdd(L, "Upper", meldChiiUpper);
	lockTable(L); lua_setfield(L, -2, "Chii");
	lockTable(L); lua_setfield(L, -2, "DiscardType");
}

/* 鳴きの種別コード */
inline void aiscript::table::functable::meldTypeCode(lua_State* const L) {
	lua_newtable(L); // MeldType
	lua_newtable(L); // Sequence
	TableAdd(L, "Lower", (int)meldSequenceExposedLower);
	TableAdd(L, "Middle", (int)meldSequenceExposedMiddle);
	TableAdd(L, "Upper", (int)meldSequenceExposedUpper);
	lockTable(L); lua_setfield(L, -2, "Sequence");
	lua_newtable(L); // Triplet
	TableAdd(L, "Kamicha", (int)meldTripletExposedLeft);
	TableAdd(L, "Toimen", (int)meldTripletExposedCenter);
	TableAdd(L, "Shimocha", (int)meldTripletExposedRight);
	lockTable(L); lua_setfield(L, -2, "Triplet");
	lua_newtable(L); // Quad
	TableAdd(L, "Concealed", (int)meldQuadConcealed);
	lua_newtable(L); // Exposed
	TableAdd(L, "Kamicha", (int)meldQuadExposedLeft);
	TableAdd(L, "Toimen", (int)meldQuadExposedCenter);
	TableAdd(L, "Shimocha", (int)meldQuadExposedRight);
	lockTable(L); lua_setfield(L, -2, "Exposed");
	lua_newtable(L); // Added
	TableAdd(L, "Kamicha", (int)meldQuadAddedLeft);
	TableAdd(L, "Toimen", (int)meldQuadAddedCenter);
	TableAdd(L, "Shimocha", (int)meldQuadAddedRight);
	lockTable(L); lua_setfield(L, -2, "Added");
	lockTable(L); lua_setfield(L, -2, "Quad");
	lockTable(L); lua_setfield(L, -2, "MeldType");
}

/* 牌の番号 */
inline void aiscript::table::functable::tileCode(lua_State* const L) {
	const char suitname[3][16] = {"Character","Circle","Bamboo",};
	const char numeral[9][8] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine",};
	lua_newtable(L);
	for (int suit = 0; suit < TILE_SUIT_HONORS; suit += TILE_SUIT_STEP) { // 数牌(ループ)
		lua_newtable(L);
		for (int num = 1; num <= 9; num++) {
			TableAdd(L, numeral[num - 1], suit + num);
			TableAdd(L, num, suit + num);
		}
		lockTable(L); lua_setfield(L, -2, suitname[suit / TILE_SUIT_HONORS]);
	}
	lua_newtable(L);
	TableAdd(L, "East", (int)EastWind); TableAdd(L, "South", (int)SouthWind);
	TableAdd(L, "West", (int)WestWind); TableAdd(L, "North", (int)NorthWind);
	lockTable(L); lua_setfield(L, -2, "Wind");
	lua_newtable(L);
	TableAdd(L, "White", (int)WhiteDragon); TableAdd(L, "Green", (int)GreenDragon);
	TableAdd(L, "Red", (int)RedDragon);
	lockTable(L); lua_setfield(L, -2, "Dragon");
	lua_newtable(L);
	TableAdd(L, "Spring", (int)Spring); TableAdd(L, "Summer", (int)Summer);
	TableAdd(L, "Autumn", (int)Autumn); TableAdd(L, "Winter", (int)Winter);
	TableAdd(L, "Plum", (int)Plum); TableAdd(L, "Orchid", (int)Orchid);
	TableAdd(L, "Chrysanthemum", (int)Chrysanthemum); TableAdd(L, "Bamboo", (int)Bamboo);
	TableAdd(L, "Fall", (int)Autumn); TableAdd(L, "Chrys", (int)Chrysanthemum);
	TableAdd(L, "Flower", (int)Flower);
	lockTable(L); lua_setfield(L, -2, "Flower");
	lockTable(L); lua_setfield(L, -2, "Tile");
}

inline void aiscript::table::functable::gametbl::makeprototype(lua_State* const L) {
	lua_newtable(L);
	lua_pushlightuserdata(L, NULL); lua_setfield(L, -2, "addr"); // pointer to C++ struct
	/* ここにメソッドを書く */
	lua_pushcfunction(L, luafunc::getactiveplayer); lua_setfield(L, -2, "getactiveplayer");
	lua_pushcfunction(L, luafunc::getchip); lua_setfield(L, -2, "getchip");
	lua_pushcfunction(L, luafunc::getdeckleft); lua_setfield(L, -2, "getdeckleft");
	lua_pushcfunction(L, luafunc::getdeposit); lua_setfield(L, -2, "getdeposit");
	lua_pushcfunction(L, luafunc::getdiscard); lua_setfield(L, -2, "getdiscard");
	lua_pushcfunction(L, luafunc::getdorainfo); lua_setfield(L, -2, "getdorainfo");
	lua_pushcfunction(L, luafunc::getdoukasen); lua_setfield(L, -2, "getdoukasen");
	lua_pushcfunction(L, luafunc::gethand); lua_setfield(L, -2, "gethand");
	lua_pushcfunction(L, luafunc::getopenwait); lua_setfield(L, -2, "getopenwait");
	lua_pushcfunction(L, luafunc::getpreviousdiscard); lua_setfield(L, -2, "getpreviousdiscard");
	lua_pushcfunction(L, luafunc::getrank); lua_setfield(L, -2, "getrank");
	lua_pushcfunction(L, luafunc::getround); lua_setfield(L, -2, "getround");
	lua_pushcfunction(L, luafunc::getrule); lua_setfield(L, -2, "getrule");
	lua_pushcfunction(L, luafunc::getscore); lua_setfield(L, -2, "getscore");
	lua_pushcfunction(L, luafunc::getseentiles); lua_setfield(L, -2, "getseentiles");
	lua_pushcfunction(L, luafunc::getshanten); lua_setfield(L, -2, "getshanten");
	lua_pushcfunction(L, luafunc::gettilecontext); lua_setfield(L, -2, "gettilecontext");
	lua_pushcfunction(L, luafunc::gettilesinhand); lua_setfield(L, -2, "gettilesinhand");
	lua_pushcfunction(L, luafunc::gettsumibou); lua_setfield(L, -2, "gettsumibou");
	lua_pushcfunction(L, luafunc::getwareme); lua_setfield(L, -2, "getwareme");
	lua_pushcfunction(L, luafunc::isfinalround); lua_setfield(L, -2, "isfinalround");
	lua_pushcfunction(L, luafunc::isfirstdraw); lua_setfield(L, -2, "isfirstdraw");
	lua_pushcfunction(L, luafunc::isippatsu); lua_setfield(L, -2, "isippatsu");
	lua_pushcfunction(L, luafunc::iskyuushu); lua_setfield(L, -2, "iskyuushu");
	lua_pushcfunction(L, luafunc::ismenzen); lua_setfield(L, -2, "ismenzen");
	lua_pushcfunction(L, luafunc::isopenriichideclared); lua_setfield(L, -2, "isopenriichideclared");
	lua_pushcfunction(L, luafunc::ispenultimateround); lua_setfield(L, -2, "ispenultimateround");
	lua_pushcfunction(L, luafunc::isriichideclared); lua_setfield(L, -2, "isriichideclared");
	lua_pushcfunction(L, luafunc::isshisanbuda); lua_setfield(L, -2, "isshisanbuda");
	lua_pushcfunction(L, luafunc::isshisibuda); lua_setfield(L, -2, "isshisibuda");
	lua_pushcfunction(L, luafunc::issumaroallowed); lua_setfield(L, -2, "issumaroallowed");
	lua_pushcfunction(L, luafunc::isyakitori); lua_setfield(L, -2, "isyakitori");
	/* メソッド定義ここまで */
	lockTable(L); lua_setfield(L, -2, "gametbl");
}

/* gameStatのアドレスを取得（暗黙の引数） */
GameTable* aiscript::table::functable::gametbl::getGameStatAddr(lua_State* const L) {
	lua_getfield(L, 1, "addr"); GameTable* addr = (GameTable*)lua_touserdata(L, -1); lua_pop(L, 1);
	return addr;
}

/* プレイヤー番号を取得 */
PLAYER_ID aiscript::table::functable::gametbl::getPlayerID(lua_State* const L, int index) {
	PLAYER_ID player; int n = lua_gettop(L);
	if ((index != 0)&&(n >= index)&&(!lua_isnil(L, index))) player = lua_tointeger(L, index);
	else {lua_getfield(L, 1, "playerid"); player = lua_tointeger(L, -1); lua_pop(L, 1);}
	return player - 1;
}

/* 牌の種類ごとの表をスタックに積む */
const std::array<tileCode, 35> aiscript::table::functable::gametbl::validTiles = {
	CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
	CharacterSix, CharacterSeven, CharacterEight, CharacterNine,
	CircleOne, CircleTwo, CircleThree, CircleFour, CircleFive,
	CircleSix, CircleSeven, CircleEight, CircleNine,
	BambooOne, BambooTwo, BambooThree, BambooFour, BambooFive,
	BambooSix, BambooSeven, BambooEight, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon,
	Flower,
};
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, Int8ByTile& tptr) {
	lua_newtable(L); // テーブル
	for (auto k = validTiles.begin(); k != validTiles.end(); k++)
		TableAdd(L, (int)*k, (int)(tptr[*k]));
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, UInt8ByTile& tptr) {
	lua_newtable(L); // テーブル
	for (auto k = validTiles.begin(); k != validTiles.end(); k++)
		TableAdd(L, (int)*k, (int)(tptr[*k]));
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, FlagByTile& tptr) {
	lua_newtable(L); // テーブル
	for (auto k = validTiles.begin(); k != validTiles.end(); k++)
		TableAdd(L, (int)*k, tptr[*k]);
}

/* 手牌を取得 */
void aiscript::table::functable::gametbl::setHand(lua_State* const L, GameTable* const tmpGameStat, int index) {
	int n = lua_gettop(L);
	PLAYER_ID player = getPlayerID(L, 0);
	if ((index != 0)&&(n >= index)&&(!lua_isnil(L, index))) { // 牌姿を指定した場合
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			lua_pushnumber(L, i + 1); lua_gettable(L, n);
			if (lua_isnil(L, -1)) { // そこに牌はなかった
				tmpGameStat->Player[player].Hand[i].tile = NoTile;
				tmpGameStat->Player[player].Hand[i].red = Normal;
			} else if (lua_istable(L, -1)) { // 牌があった
				lua_pushstring(L, "tile"); lua_gettable(L, -2);
				tmpGameStat->Player[player].Hand[i].tile = (::tileCode)lua_tointeger(L, -1);
				lua_pop(L, -1);
				lua_pushstring(L, "red"); lua_gettable(L, -2);
				tmpGameStat->Player[player].Hand[i].red = (doraCol)lua_tointeger(L, -1);
				lua_pop(L, -1);
			} // 変なことになっていたら無視
			lua_pop(L, 1);
		}
	}
}

/* ツモ番のプレイヤー番号 */
int aiscript::table::functable::gametbl::luafunc::getactiveplayer(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushinteger(L, (int)getGameStatAddr(L)->CurrentPlayer.Active + 1);
	return 1;
}

/* チップ取得 */
int aiscript::table::functable::gametbl::luafunc::getchip(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("chip")) lua_pushnil(L); // チップ無しルールならnil
	else lua_pushinteger(L, gameStat->Player[player].playerChip); // チップの収支をスタックに積む
	return 1;
}

/* 山牌の残り枚数 */
int aiscript::table::functable::gametbl::luafunc::getdeckleft(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushinteger(L, tilesLeft(getGameStatAddr(L)));
	return 1;
}

/* 供託点棒の数 */
int aiscript::table::functable::gametbl::luafunc::getdeposit(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushinteger(L, getGameStatAddr(L)->Deposit);
	return 1;
}

/* 捨牌テーブル */
int aiscript::table::functable::gametbl::luafunc::getdiscard(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (uint8_t i = 1; i <= gameStat->Player[player].DiscardPointer; i++) {
		lua_pushnumber(L, i);
		lua_newtable(L);
		TableAdd(L, "tile", (int)gameStat->Player[player].Discard[i].tcode.tile);
		TableAdd(L, "red", (int)gameStat->Player[player].Discard[i].tcode.red);
		TableAdd(L, "through", gameStat->Player[player].Discard[i].isDiscardThrough);
		TableAdd(L, "riichi", (gameStat->Player[player].Discard[i].dstat == discardRiichi) ||
			(gameStat->Player[player].Discard[i].dstat == discardRiichiTaken));
		TableAdd(L, "taken", (gameStat->Player[player].Discard[i].dstat == discardTaken) ||
			(gameStat->Player[player].Discard[i].dstat == discardRiichiTaken));
		lua_settable(L, -3);
	}
	return 1;
}

/* ドラ情報の配列 */
int aiscript::table::functable::gametbl::luafunc::getdorainfo(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, gameStat->DoraFlag.Omote);
	return 1;
}

/* 導火線 */
int aiscript::table::functable::gametbl::luafunc::getdoukasen(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (getGameStatAddr(L)->DoukasenPlayer == -1)
		lua_pushnil(L); // 導火線なしの時はnil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->DoukasenPlayer + 1);
	return 1;
}

/* 純手牌テーブル */
int aiscript::table::functable::gametbl::luafunc::gethand(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
			/* 牌があったらサプテーブルに変換、なかったらnilのまま放置 */
			lua_pushnumber(L, i + 1);
			lua_newtable(L);
			TableAdd(L, "tile", (int)gameStat->Player[player].Hand[i].tile);
			TableAdd(L, "red", (int)gameStat->Player[player].Hand[i].red);
			lua_settable(L, -3);
		}
	}
	return 1;
}

/* 副露面子 */
/*inline void aiscript::table::playertable::pltable::PlayerMeld(lua_State* const L, const MELD_BUF* const plMeld, uint8_t MeldPointer) {
	lua_newtable(L);
	for (uint8_t i = 1; i <= MeldPointer; i++) {
		std::ostringstream o; o << (int)i;
		TableAdd(L, o.str().c_str(), plMeld[i]);
	}
	lua_setfield(L, -2, "Meld");
}*/

/* オープンリーチの待ち牌情報 */
int aiscript::table::functable::gametbl::luafunc::getopenwait(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, gameStat->OpenRichiWait);
	return 1;
}

/* 直前の捨牌情報(食い変え防止用) */
int aiscript::table::functable::gametbl::luafunc::getpreviousdiscard(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	if (gameStat->PreviousMeld.Discard != NoTile) lua_pushinteger(L, gameStat->PreviousMeld.Discard); else lua_pushnil(L);
	if (gameStat->PreviousMeld.Stepped != NoTile) lua_pushinteger(L, gameStat->PreviousMeld.Stepped); else lua_pushnil(L);
	return 2;
}

/* 順位取得 */
int aiscript::table::functable::gametbl::luafunc::getrank(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushinteger(L, calcRank(gameStat)[player]); // 順位をスタックに積む
	return 1;
}

/* 現在の局番号 */
int aiscript::table::functable::gametbl::luafunc::getround(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game"))
		// 東場しかないルール
		lua_pushinteger(L, gameStat->LoopRound * 4 + gameStat->GameRound + 1);
	else // 普通のルール
		lua_pushinteger(L, gameStat->LoopRound * roundLoopRate() + gameStat->GameRound + 1);
	return 1;
}

/* ルール設定取得 */
int aiscript::table::functable::gametbl::luafunc::getrule(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 2) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	const char* fieldname = lua_tostring(L, 2);
	lua_pushstring(L, RuleData::chkRule(fieldname).c_str());
	return 1;
}

/* 持ち点取得 */
int aiscript::table::functable::gametbl::luafunc::getscore(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushnumber(L, gameStat->Player[player].PlayerScore.bignumtodbl()); // 持ち点をdoubleにしてスタックに積む
	return 1;
}

/* 見えている牌の数の配列 */
int aiscript::table::functable::gametbl::luafunc::getseentiles(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, countseentiles(gameStat));
	return 1;
}

/* 向聴数を取得 */
int aiscript::table::functable::gametbl::luafunc::getshanten(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	PLAYER_ID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 2);
	lua_pushinteger(L, ShantenAnalyzer::calcShanten(&tmpGameStat, player, ShantenAnalyzer::shantenAll));
	return 1;
}

/* 牌についての情報 */
int aiscript::table::functable::gametbl::luafunc::gettilecontext(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		lua_pushnumber(L, i + 1);
		lua_newtable(L);
		TileStatus tileStat = gettilestatus(gameStat, player, i, false);
		TableAdd(L, "isexistent", tileStat.isExistent);
		if (tileStat.isExistent) {
			TableAdd(L, "formstriplet", tileStat.formsTriplet);
			TableAdd(L, "formssequence", tileStat.formsSequence);
			TableAdd(L, "canformquad", tileStat.canFormQuad);
			TableAdd(L, "formspair", tileStat.formsPair);
			TableAdd(L, "formsryanmen", tileStat.seqDoubleSideWait);
			TableAdd(L, "formskanchan", tileStat.seqMidWait);
			TableAdd(L, "formspenchan", tileStat.seqSingleSideWait);
		}
		lua_settable(L, -3);
	}
	return 1;
}

/* 手牌に持っている枚数の配列 */
int aiscript::table::functable::gametbl::luafunc::gettilesinhand(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	pushTileTable(L, countTilesInHand(gameStat, player));
	return 1;
}

/* 積み棒 */
int aiscript::table::functable::gametbl::luafunc::gettsumibou(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushinteger(L, getGameStatAddr(L)->Honba);
	return 1;
}

/* 割れ目 */
int aiscript::table::functable::gametbl::luafunc::getwareme(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (getGameStatAddr(L)->WaremePlayer == -1)
		lua_pushnil(L); // 割れ目なしの時はnil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->WaremePlayer + 1);
	return 1;
}

/* オーラスか？ */
int aiscript::table::functable::gametbl::luafunc::isfinalround(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	const GameTable* gameStat = getGameStatAddr(L);
	lua_pushboolean(L, (gameStat->LoopRound * 16 + gameStat->GameRound) >= gameStat->GameLength);
	return 1;
}

/* 第一自摸か？ */
int aiscript::table::functable::gametbl::luafunc::isfirstdraw(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].FirstDrawFlag);
	return 1;
}

/* 一発圏内？ */
int aiscript::table::functable::gametbl::luafunc::isippatsu(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("riichi_ippatsu")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].RichiFlag.IppatsuFlag);
	return 1;
}

/* 九種九牌？ */
int aiscript::table::functable::gametbl::luafunc::iskyuushu(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (RuleData::chkRuleApplied("nine_terminals")) lua_pushnil(L);
	else lua_pushboolean(L, chkdaopaiability(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* 門前？ */
int aiscript::table::functable::gametbl::luafunc::ismenzen(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].MenzenFlag);
	return 1;
}

/* オープン立直している？ */
int aiscript::table::functable::gametbl::luafunc::isopenriichideclared(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].RichiFlag.OpenFlag);
	return 1;
}

/* ラス前か？ */
int aiscript::table::functable::gametbl::luafunc::ispenultimateround(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	const GameTable* gameStat = getGameStatAddr(L);
	lua_pushboolean(L, (gameStat->LoopRound * 16 + gameStat->GameRound) == (gameStat->GameLength - 1));
	return 1;
}

/* 立直している？ */
int aiscript::table::functable::gametbl::luafunc::isriichideclared(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].RichiFlag.RichiFlag);
	return 1;
}

/* 十三不塔？ */
int aiscript::table::functable::gametbl::luafunc::isshisanbuda(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (RuleData::chkRuleApplied("shiisan_puutaa")) lua_pushnil(L);
	else lua_pushboolean(L, yaku::yakuCalculator::chkShisanBuDa(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* 十三無靠？ */
int aiscript::table::functable::gametbl::luafunc::isshisibuda(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (RuleData::chkRuleApplied("shiisan_uushii")) lua_pushnil(L);
	else lua_pushboolean(L, yaku::yakuCalculator::chkShisiBuDa(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* 四馬路解禁？ */
int aiscript::table::functable::gametbl::luafunc::issumaroallowed(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("sumaro")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].SumaroFlag);
	return 1;
}

/* 焼き鳥？ */
int aiscript::table::functable::gametbl::luafunc::isyakitori(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("yakitori")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].YakitoriFlag);
	return 1;
}
