#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* チートイ */
	auto isQiDui =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("七対子"), get_yaku_han("seven_pairs"), /* 1翻50符のルールと2翻25符のルールがある。符はここでは設定できないです…… */
		isQiDui
	));

	auto chktiles = // 判定関数オブジェクト
		[isQiDui](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetDuiz, int numOfDuiz) -> bool {
			if (isQiDui(analysis))
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetDuiz, numOfDuiz)
					== (numOfDuiz > (NumOfTilesInHand / 2) ? (NumOfTilesInHand / 2) : numOfDuiz));
			else return false;
		};
	// ---------------------------------------------------------------------

	/* 七福星 */
	if (RuleData::chkRuleApplied("seven_honor_pairs") && !RuleData::chkRule("seven_honor_pairs", "yes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("七福星"), get_yaku_han("seven_honor_pairs"),
			_T("混老頭"), _T("混一色"), _T("三元七対子"), _T("四喜七対子"), _T("紅白七対"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (isQiDui(analysis))
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NumOfTilesInHand / 2);
				else return false;
			}
		));
	/* 三元七対子 */
	if (RuleData::chkRuleApplied("dragon_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三元七対子"), get_yaku_han("dragon_pairs"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[GreenDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* 四喜七対子 */
	if (RuleData::chkRuleApplied("wind_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四喜七対子"), get_yaku_han("wind_pairs"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[EastWind] >= 2) &&
					(analysis->TileCount[SouthWind] >= 2) &&
					(analysis->TileCount[WestWind] >= 2) &&
					(analysis->TileCount[NorthWind] >= 2));
			}
		));
	/* 奇数七対子 */
	if (RuleData::chkRuleApplied("odd_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("奇数七対子"), get_yaku_han("odd_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[15] = {
					CharacterOne, CharacterThree, CharacterFive, CharacterSeven, CharacterNine,
					CircleOne, CircleThree, CircleFive, CircleSeven, CircleNine,
					BambooOne, BambooThree, BambooFive, BambooSeven, BambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 15);
			}
		));
	/* 偶数七対子 */
	if (RuleData::chkRuleApplied("even_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("偶数七対子"), get_yaku_han("even_pairs"),
			_T("断幺九"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[12] = {
					CharacterTwo, CharacterFour, CharacterSix, CharacterEight,
					CircleTwo, CircleFour, CircleSix, CircleEight,
					BambooTwo, BambooFour, BambooSix, BambooEight,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 12);
			}
		));

	// ---------------------------------------------------------------------

	/* 紅白七対 */
	if (RuleData::chkRuleApplied("kouhaku_chiitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅白七対"), get_yaku_han("kouhaku_chiitoi"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* 北の大地 */
	if (RuleData::chkRuleApplied("kita_no_daichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北の大地"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
				switch (analysis->TsumoHai->tile) { /* 何で和了ったかで分岐 */
				case NorthWind: /* 北で和了るとダブル役満 */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman);
				case WhiteDragon: /* 白で和了るとトリプル役満 */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman);
				default:
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
				}
			}),
			_T("混一色"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[7] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					NorthWind, WhiteDragon,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));
	/* 世界一 */
	if (RuleData::chkRuleApplied("sekaiichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("世界一"), get_yaku_han("sekaiichi"),
			_T("四喜七対子"), _T("混老頭"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[7] = {
					CharacterOne, CircleOne, BambooOne,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));
	/* ニコニコ七対子 */
	if (RuleData::chkRuleApplied("nikoniko_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ニコニコ七対子"), get_yaku_han("nikoniko_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[6] = {
					CharacterTwo, CharacterFive,
					CircleTwo, CircleFive, BambooTwo, BambooFive,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 6);
			}
		));
	/* 一休さん七対子 */
	if (RuleData::chkRuleApplied("ikkyuusan_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一休さん七対子"), get_yaku_han("ikkyuusan_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[9] = {
					CharacterOne, CharacterThree, CharacterNine,
					CircleOne, CircleThree, CircleNine,
					BambooOne, BambooThree, BambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 9);
			}
		));

	// ---------------------------------------------------------------------

	/* 三色同対 */
	if (RuleData::chkRuleApplied("sanshoku_doutoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色同対"), get_yaku_han("sanshoku_doutoi"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->TileCount[TileSuitCharacters + i] >= 2) &&
						(analysis->TileCount[TileSuitCircles + i] >= 2) &&
						(analysis->TileCount[TileSuitBamboos + i] >= 2))
						yakuFlag = true;
				return isQiDui(analysis) && yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	{
		auto gyakusharin =
			[isQiDui](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
				std::array<TileCode, 7> honors = {
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon, RedDragon,
				}; int count = 0;
				for (auto k = honors.begin(); k != honors.end(); k++)
					if (analysis->TileCount[*k] >= 2) ++count;
				return isQiDui(analysis) &&
					(analysis->TileCount[suit + 1] >= 2) &&
					(analysis->TileCount[suit + 9] >= 2) &&
					(count == 5);
			};
		/* 逆車輪 */
		if (RuleData::chkRuleApplied("gyakusharin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("逆車輪"), get_yaku_han("gyakusharin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TileSuitCircles);
				}
			));
		/* 逆竹林 */
		if (RuleData::chkRuleApplied("gyakuchikurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("逆竹林"), get_yaku_han("gyakuchikurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TileSuitBamboos);
				}
			));
		/* 逆数隣 */
		if (RuleData::chkRuleApplied("gyakusuurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("逆数隣"), get_yaku_han("gyakusuurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TileSuitCharacters);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* 国士 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("国士無双"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("十三不塔"), _T("五門斎"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (RuleData::chkRuleApplied("double_yakuman")) /* 国士13面待ち（ダブル役満） */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("国士無双十三面"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("国士無双"), _T("十三不塔"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // 国士無双になっていて、
					(!analysis->MachiInfo.FuritenFlag) && // フリテンではなくて、
					((analysis->MachiInfo.MachiMen == 13)||(analysis->GameStat->TianHuFlag))); // 13面待ちか天和になっている
			}
		));
	/* 七星無靠 */
	if (RuleData::chkRuleApplied("stellar_uushii"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("七星無靠"), get_yaku_han("stellar_uushii"),
			_T("全不靠"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenStellar] == -1);
			}
		));
	/* 南北戦争 */
	if (RuleData::chkRuleApplied("civil_war"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南北戦争"), get_yaku_han("civil_war"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenCivilWar] == -1);
			}
		));
	/* 東北新幹線グリーン車 */
	if (RuleData::chkRuleApplied("tohoku_shinkansen_green"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東北新幹線グリーン車"), get_yaku_han("tohoku_shinkansen_green"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenTohokuGreen] == -1);
			}
		));
	/* 惑星直列 */
	if (RuleData::chkRuleApplied("syzygy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("惑星直列"), get_yaku_han("syzygy"),
			_T("清一色"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSyzygy] == -1);
			}
		));
	/* 全不靠 */
	if (RuleData::chkRuleApplied("quanbukao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("全不靠"), get_yaku_han("quanbukao"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenQuanbukao] == -1);
			}
		));
	/* セブンアップ */
	if (RuleData::chkRuleApplied("sevenup"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("セブンアップ"), get_yaku_han("sevenup"),
			_T("混一色"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSevenup] == -1);
			}
		));
	/* 組合龍 */
	if (RuleData::chkRuleApplied("zuhelong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("組合龍"), get_yaku_han("zuhelong"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenZuhelong] == -1);
			}
		));
	/* 仁和寺 */
	if (RuleData::chkRuleApplied("ninnaji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("仁和寺"), get_yaku_han("ninnaji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenNinnaji] == -1);
			}
		));
}
