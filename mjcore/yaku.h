#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include <functional>
#include <list>
#include "largenum.h"
#include "gametbl.h"
#include "tileutil.h"
#include "ykclass.h"

namespace yaku {
	EXPORT_STRUCT YAKUSTAT {
		bool isValid; // 和了っているかどうか
		int BasePoints; // 符
		int CoreHan; // 翻
		int BonusHan; // 翻（縛りを満たさない）
		int DoraQuantity; // ドラの数
		int UraDoraQuantity; // 裏ドラの数
		int AkaDoraQuantity; // 赤ドラの数
		int AoDoraQuantity; // 青ドラの数
		int AliceDora; // アリスドラの数
		int FlowerQuantity; // 花牌の数
		LargeNum AgariPoints; // 合計和了点
		char yakuNameList[1024];
		char yakuValList[1024];
		char yakumanNameList[1024];
		char yakumanValList[1024];
		// -----------------------------------------------------------------
		MELD_BUF MianziDat; // 面子に分解したデータ
		// -----------------------------------------------------------------
		static int getSize(); // 構造体のサイズを返す
		static void Init(YAKUSTAT* const myInstance); // インスタンスを初期化する
		static void exportYakuPoint(const YAKUSTAT* const myInstance, int* const exportArray);
		static void setYakuInfo(YAKUSTAT* const myInstance, int index, int value);
		static void setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
		static void addYakuInfo(YAKUSTAT* const myInstance, int index, int value);
		static void addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
		static int getYakuInfo(const YAKUSTAT* const myInstance, int index);
		static void getYakuInfo(const YAKUSTAT* const myInstance, int index, char* const targetStr, int bufSize);
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif

	// ---------------------------------------------------------------------

	enum ParseOrder : uint8_t {
		Ke_Shun, Shun_Ke, Ke_Shun_Rev, Shun_Ke_Rev
	};
	struct ParseMode {
		tileCode AtamaCode;
		ParseOrder Order;
	};

#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<ParseMode>::value, "ParseMode is not POD");
#endif

	// ---------------------------------------------------------------------

	namespace mentsuParser { // 面子パーサ
		bool makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile); /* 順子の処理 */
		void makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile); /* 刻子の処理 */

		void makementsu(const GameTable* const gameStat,
			PLAYER_ID targetPlayer, ParseMode AtamaCode,
			int* const ProcessedMelds, MELD_BUF MianziDat); /* 面子に分解する */
		__declspec(dllexport) void makementsu(const GameTable* const gameStat, void *,
			int targetPlayer, int AtamaCode, int* const ProcessedMelds, int* const MianziDat);

		void inline ReadAgainMentsu(MELD_BUF target, const int* const source); /* 逆変換 */
	}

	// ---------------------------------------------------------------------

	namespace countingFacility {
		int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
		__declspec(dllexport) int countPairs(
			const int* const tileCount, const int* const targetTiles, int numOfTiles);
		int countTileNumerals(const Int8ByTile tileCount);
		__declspec(dllexport) int countTileNumerals(const int* const tileCount);

		Int8ByTile countByMelds(
			const MELD_BUF MianziDat, int* const hits,
			std::function<bool (meldStat)> f);

		Int8ByTile countKez(const MELD_BUF MianziDat, int* const Kezi);
		__declspec(dllexport) int countKez(int* const KezCount, const int* const MianziDat);
		Int8ByTile countAnKez(const MELD_BUF MianziDat, int* const Kezi);
		__declspec(dllexport) int countAnKez(int* const KezCount, const int* const MianziDat);
		Int8ByTile countDuiz(const MELD_BUF MianziDat);
		__declspec(dllexport) void countDuiz(int* const DuiCount, const int* const MianziDat);

		Int8ByTile countShunz(const MELD_BUF MianziDat);
		__declspec(dllexport) void countShunz(int* const ShunzCount, const int* const MianziDat);
		Int8ByTile countAnShunz(const MELD_BUF MianziDat);
		__declspec(dllexport) void countAnShunz(int* const ShunzCount, const int* const MianziDat);

		Int8ByTile countKangz(const MELD_BUF MianziDat, int* const Kangzi);
		__declspec(dllexport) int countKangz(int* const KangzCount, const int* const MianziDat);
		Int8ByTile countAnKangz(const MELD_BUF MianziDat, int* const Kangzi);
		__declspec(dllexport) int countAnKangz(int* const KangzCount, const int* const MianziDat);
		Int8ByTile countKaKangz(const MELD_BUF MianziDat, int* const Kangzi);
		__declspec(dllexport) int countKaKangz(int* const KangzCount, const int* const MianziDat);

		int countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
			const tileCode* const targetShunz, int numOfShunz, bool Mode);
		__declspec(dllexport) int countSpecMentz(const int* const MianziDat,
			int* const targetKez, int numOfKez, int* const targetShunz, int numOfShunz, int Mode);
		int countMentzNumerals(const MELD_BUF MianziDat);
		__declspec(dllexport) int countMentzNumerals(const int* const MianziDat);
	}

	// ---------------------------------------------------------------------

	namespace yakuCalculator {

		class YakuCatalog { // 役の一覧 [singleton]
		private:
			YakuCatalog() {}
			YakuCatalog(const YakuCatalog&);
			YakuCatalog& operator= (const YakuCatalog&);
		public:
			static YakuCatalog* Instantiate(); // Singleton instance accessor
			std::list<Yaku> catalog;
		};

		__declspec(dllexport) void init();

		struct MENTSU_ANALYSIS { // 面子解析結果
			MELD_BUF MianziDat; // 面子パース結果
			Int8ByTile KeziCount; // 刻子・槓子の数
			Int8ByTile AnKeziCount; // 暗刻・暗槓の数
			Int8ByTile DuiziCount; // 対子・刻子・槓子の数
			Int8ByTile ShunziCount; // 順子の数
			Int8ByTile AnShunziCount; // 鳴いていない順子の数
			Int8ByTile KangziCount; // 槓子の数
			Int8ByTile AnKangziCount; // 暗槓の数
			Int8ByTile KaKangziCount; // 加槓の数
		};
#ifdef MJCORE_EXPORTS
		static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif

		YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
		__declspec(dllexport) void countyaku(const GameTable* const gameStat,
			YAKUSTAT* const yakuInfo, int targetPlayer);
	}
}

#endif
