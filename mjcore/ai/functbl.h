#pragma once

#include <array>
#include "class.h"
#include "../tileutil.h"

enum MeldCallID {
	meldNone, meldRon, meldKan, meldPon, meldChiiLower, meldChiiMiddle, meldChiiUpper
};

class aiscript::table::functable {
private:
	static const char* tblname;
	static int tableLockedErr(lua_State* const L);
	static void lockTable(lua_State* const L);
	static int random(lua_State* const L);
	static int say(lua_State* const L);
	static PlayerID getPlayerID(lua_State* const L);
	static inline void discardTileCode(lua_State* const L);
	static inline void doraColorCode(lua_State* const L);
	static inline void meldCallCode(lua_State* const L);
	static inline void meldTypeCode(lua_State* const L);
	static inline void tileCode(lua_State* const L);
	static inline void agariTypeCode(lua_State* const L);
	static inline void gametype(lua_State* const L);
	static inline void version(lua_State* const L);
	static int version_tostring(lua_State* const L);
	static int log(lua_State* const L);
	static int chkargnum(lua_State* const L, int argmin, int argmax);
	class gametbl;
public:
	static inline const char* getGTblName();
	static inline const char* getTblName() {return tblname;}
	static void inittable(lua_State* const L, int playerID);
public: /* Monostate class: cannot be instantiated */
	functable() = delete;
	functable(const functable&) = delete;
	functable& operator= (const functable&) = delete;
	~functable() = delete;
};

class aiscript::table::functable::gametbl {
private:
	static const char* tblname;
	static const std::array< ::TileCode, 35> validTiles;
	static GameTable* getGameStatAddr(lua_State* const L);
	static PlayerID getPlayerID(lua_State* const L, int index);
	static void pushTileTable(lua_State* const L, Int8ByTile& tptr);
	static void pushTileTable(lua_State* const L, UInt8ByTile& tptr);
	static void pushTileTable(lua_State* const L, FlagByTile& tptr);
	static void pushTileTable(lua_State* const L, InfoByTile<MachihaiTileInfo>& tptr);
	static void setHand(lua_State* const L, GameTable* const tmpGameStat, int index);
	class luafunc;
public:
	static inline const char* getTblName() {return tblname;}
	static inline void makeprototype(lua_State* const L, int playerID);
public: /* Monostate class: cannot be instantiated */
	gametbl() = delete;
	gametbl(const gametbl&) = delete;
	gametbl& operator= (const gametbl&) = delete;
	~gametbl() = delete;
};

inline const char* aiscript::table::functable::getGTblName() {return gametbl::getTblName();}

class aiscript::table::functable::gametbl::luafunc {
private:
	class riskchk;
public:
	static int evaluate(lua_State* const L);
	static int getactiveplayer(lua_State* const L);
	static int getbakaze(lua_State* const L);
	static int getbasepoint(lua_State* const L);
	static int getchip(lua_State* const L);
	static int getcurrentdiscard(lua_State* const L);
	static int getdeckleft(lua_State* const L);
	static int getdeposit(lua_State* const L);
	static int getdiscard(lua_State* const L);
	static int getdorainfo(lua_State* const L);
	static int getdoukasen(lua_State* const L);
	static int getflower(lua_State* const L);
	static int gethand(lua_State* const L);
	static int getjikaze(lua_State* const L);
	static int getmeld(lua_State* const L);
	static int getopenwait(lua_State* const L);
	static int getpreviousdiscard(lua_State* const L);
	static int getrank(lua_State* const L);
	static int getround(lua_State* const L);
	static int getrule(lua_State* const L);
	static int getscore(lua_State* const L);
	static int getseentiles(lua_State* const L);
	static int getshanten(lua_State* const L);
	static int gettenpaistat(lua_State* const L);
	static int gettilecontext(lua_State* const L);
	static int gettilerisk(lua_State* const L);
	static int gettilesinhand(lua_State* const L);
	static int gettsumibou(lua_State* const L);
	static int getwareme(lua_State* const L);
	static int getyakuhaiwind(lua_State* const L);
	static int isabovebase(lua_State* const L);
	static int isankanallowed(lua_State* const L);
	static int isdoujunfuriten(lua_State* const L);
	static int isfinalround(lua_State* const L);
	static int isfirstdraw(lua_State* const L);
	static int isippatsu(lua_State* const L);
	static int iskansanjunqualified(lua_State* const L);
	static int iskyuushu(lua_State* const L);
	static int ismenzen(lua_State* const L);
	static int isopenriichideclared(lua_State* const L);
	static int ispenultimateround(lua_State* const L);
	static int isrenpaitenhohqualified(lua_State* const L);
	static int isriichideclared(lua_State* const L);
	static int isshisanbuda(lua_State* const L);
	static int isshisibuda(lua_State* const L);
	static int isshokanqualified(lua_State* const L);
	static int issumaroallowed(lua_State* const L);
	static int isyakitori(lua_State* const L);
public: /* Monostate class: cannot be instantiated */
	luafunc() = delete;
	luafunc(const luafunc&) = delete;
	luafunc& operator= (const luafunc&) = delete;
	~luafunc() = delete;
};

class aiscript::table::functable::gametbl::luafunc::riskchk {
public:
	static bool issameasprevious(const GameTable* const gameStat, PlayerID player, int index);
	static bool isdora(const GameTable* const gameStat, PlayerID player, int index);
	static bool isdorasuji(const GameTable* const gameStat, PlayerID player, int index);
	static bool isdorasoba(const GameTable* const gameStat, PlayerID player, int index);
	static bool isnochance(const GameTable* const gameStat, PlayerID player, int index);
	static bool isonechance(const GameTable* const gameStat, PlayerID player, int index);
	static bool isneverdiscarded(const GameTable* const gameStat, PlayerID player, int index);
	static bool isseenfour(const GameTable* const gameStat, PlayerID player, int index);
	static bool isgembutsu(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool issuji(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool isurasuji(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool isaida4ken(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool ismatagisuji(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool issenkisuji(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool isnamakurasuji(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
	static bool isnakasuji(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index);
public: /* Monostate class: cannot be instantiated */
	riskchk() = delete;
	riskchk(const riskchk&) = delete;
	riskchk& operator= (const riskchk&) = delete;
	~riskchk() = delete;
};
