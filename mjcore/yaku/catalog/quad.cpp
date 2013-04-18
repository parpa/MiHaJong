#include "../catalog.h"
#include "../../../astro/astro.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
	/* 超四喜 */
	if (RuleData::chkRuleApplied("chousixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("超四喜"), get_yaku_han("chousixi"),
			_T("大四喜"), _T("四槓子"), _T("対々和"), _T("声東撃西"), _T("走南闖北"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* 四槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("四槓子"), get_yaku_han("suukantsu"),
		_T("対々和"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
	/* 三暗槓 */
	if (RuleData::chkRuleApplied("sanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三暗槓"), get_yaku_han("sanankan"),
			_T("三槓子"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* 大峡谷 */
	if (RuleData::chkRuleApplied("grand_canyon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大峡谷"), get_yaku_han("grand_canyon"),
			_T("三槓子"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CharacterNine] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] + analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2));
			}
		));
	/* 雪化粧 */
	if (RuleData::chkRuleApplied("yukigeshou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雪化粧"), get_yaku_han("yukigeshou"),
			_T("役牌・白"), _T("三槓子"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* 三色同槓 */
	if (RuleData::chkRuleApplied("sanshoku_doukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色同槓"), get_yaku_han("sanshoku_doukan"),
			_T("三色同刻"), _T("三槓子"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if ((analysis->KangziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->KangziCount[TileSuitCircles + i] >= 1) &&
						(analysis->KangziCount[TileSuitBamboos + i] >= 1))
						return true;
				return false;
			}
		));
	/* 超三元 */
	if (RuleData::chkRuleApplied("chousangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("超三元"), get_yaku_han("chousangen"),
			_T("大三元"), _T("三槓子"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
	/* 三槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("三槓子"), get_yaku_han("sankantsu"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
	/* 二暗槓 */
	if (RuleData::chkRuleApplied("ryanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二暗槓"), get_yaku_han("ryanankan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* 声東撃西 */
	if (RuleData::chkRuleApplied("shengdong_jixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("声東撃西"), get_yaku_han("shengdong_jixi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* 走南闖北 */
	if (RuleData::chkRuleApplied("zaonan_chuangbei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("走南闖北"), get_yaku_han("zaonan_chuangbei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* 暗中模索 */
	if (RuleData::chkRuleApplied("anchumosaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("暗中模索"), get_yaku_han("anchumosaku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TileSuitStep == TileSuitBamboos / TileSuitStep) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* 戦車 */
	if (RuleData::chkRuleApplied("tank"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("戦車"), get_yaku_han("tank"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* 真田六文銭 */
	if (RuleData::chkRuleApplied("sanadacoin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("真田六文銭"), get_yaku_han("sanadacoin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* 三矢の誓い */
	if (RuleData::chkRuleApplied("three_arrows"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三矢の誓い"), get_yaku_han("three_arrows"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* 鬼は外 */
	if (RuleData::chkRuleApplied("setsubun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鬼は外"), get_yaku_han("setsubun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
#ifdef _WIN32
				TIME_ZONE_INFORMATION Zeitzone; SYSTEMTIME Zeit; // 宣言。こんな変数名付ける私は厨二病かもしれない
				GetTimeZoneInformation(&Zeitzone); // タイムゾーンを取得する
				GetLocalTime(&Zeit); // WINAPIを使ってローカル時刻を取得
				SYSTEMTIME HeuteMitternacht = {Zeit.wYear, Zeit.wMonth, Zeit.wDayOfWeek, Zeit.wDay, 0, 0, 0, 0}; // 当日0時のSYSTEMTIME
				double JulianischeDatum = systime_to_julian(&HeuteMitternacht) - ((double)Zeitzone.Bias / 1440.0); // ローカル時間で当日0時のユリウス日
				return /* 翌日が立春か判定する。立春とは太陽が黄経315度の子午線を通過する日である。 */
					(sun_ecliptic_longitude(JulianischeDatum + 1.0) <= 315.0) &&
					(sun_ecliptic_longitude(JulianischeDatum + 2.0) >= 315.0) &&
					(analysis->KangziCount[CircleNine] >= 1); /* 手牌自体は九筒の槓子だけが条件 */
#else /*_WIN32*/
				/* TODO: 未実装箇所 */
				return false;
#endif /*_WIN32*/
			}
		));
	/* 草加 */
	if (RuleData::chkRuleApplied("souka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("草加"), get_yaku_han("souka"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TileSuitBamboos + i] >= 1)
						return true;
				return false;
			}
		));
	/* 合格祈願 */
	if (RuleData::chkRuleApplied("goukaku_kigan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("合格祈願"), get_yaku_han("goukaku_kigan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if (analysis->KangziCount[i + 5] >= 1) flag1 = true;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if (analysis->KangziCount[i + 9] >= 1) flag2 = true;
				return flag1 && flag2;
			}
		));
	/* 新越谷 */
	if (RuleData::chkRuleApplied("shinkoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("新越谷"), get_yaku_han("shinkoshigaya"),
			_T("越谷"), /* 三槓子と複合 */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CharacterFive] + analysis->KangziCount[CircleFive] + analysis->KangziCount[BambooFive] >= 1) &&
					(analysis->KangziCount[CharacterFour] + analysis->KangziCount[CircleFour] + analysis->KangziCount[BambooFour] >= 1) &&
					(analysis->KangziCount[CharacterEight] + analysis->KangziCount[CircleEight] + analysis->KangziCount[BambooEight] >= 1);
			}
		));
}
