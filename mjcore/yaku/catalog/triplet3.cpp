#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_3() {
	auto countTilesOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->TileCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->TileCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->TileCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countKangziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->KangziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->KangziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->KangziCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countKeziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->KeziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->KeziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->KeziCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countDuiziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->DuiziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->DuiziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->DuiziCount[TILE_SUIT_BAMBOOS + numeral];
		};

	// ---------------------------------------------------------------------

	/* イナバウアー */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"イナバウアー", get_yaku_han("inabauer"),
			"清一色", "対々和", "筋牌刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->DuiziCount[i + 1] >= 1) &&
						(analysis->DuiziCount[i + 7] >= 1) &&
						(analysis->DuiziCount[i + 8] >= 1) &&
						(analysis->DuiziCount[i + 5] >= 1) &&
						(analysis->DuiziCount[i + 2] >= 1))
						flag = true;
				return flag;
			}
		));
	/* 四月馬鹿 */
	if (RuleData::chkRuleApplied("april_fool")) {
		auto isApril1st =
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 4) && (nowTime.wDay == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四月馬鹿", get_yaku_han("april_fool"),
			[isApril1st](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (!isApril1st()) return false;
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 4] >= 1) &&
						(analysis->KeziCount[i + 1] >= 1))
						flag = true;
				return flag && (analysis->TileCount[CircleNine]);
			}
		));
	}
	/* 中東の笛 */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中東の笛", get_yaku_han("chuutounofue"),
			"役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* 三国志 */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三国志", get_yaku_han("sangokushi"),
			"対々和",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* 一休さん */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一休さん", get_yaku_han("ikkyuusan"),
			"対々和",
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 1) > 0) &&
					(countDuiziOf(analysis, 9) > 0) &&
					(countDuiziOf(analysis, 3) > 0) &&
					(countDuiziOf(analysis, 1) + countDuiziOf(analysis, 9) +
					countDuiziOf(analysis, 3) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* 大魔神 */
	if (RuleData::chkRuleApplied("daimajin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大魔神", get_yaku_han("daimajin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[WhiteDragon] +
					analysis->AnKeziCount[GreenDragon] +
					analysis->AnKeziCount[RedDragon]) >= 1) &&
					(analysis->TsumoHai->tile == BambooFive) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki);
			}
		));
	/* 松竹梅 */
	if (RuleData::chkRuleApplied("shouchikubai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"松竹梅", get_yaku_han("shouchikubai"),
			"役牌・發",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 2; i <= 9; ++i)
					if (analysis->KeziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						flag = true;
				return flag &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* パンダ */
	if (RuleData::chkRuleApplied("panda"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"パンダ", get_yaku_han("panda"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 黒旗軍 */
	if (RuleData::chkRuleApplied("kokkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"黒旗軍", get_yaku_han("kokkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1);
			}
		));
	/* 白旗軍 */
	if (RuleData::chkRuleApplied("hakkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白旗軍", get_yaku_han("hakkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* 紅旗軍 */
	if (RuleData::chkRuleApplied("kohkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅旗軍", get_yaku_han("kohkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* 藍旗軍 */
	if (RuleData::chkRuleApplied("rankigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"藍旗軍", get_yaku_han("rankigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 東風燕剪柳 */
	if (RuleData::chkRuleApplied("dongfeng_yan_jian_liu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東風燕剪柳", get_yaku_han("dongfeng_yan_jian_liu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* 橋本真也 */
	if (RuleData::chkRuleApplied("hashimoto_shin_ya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"橋本真也", get_yaku_han("hashimoto_shin_ya"),
			"対々和", "絶一門",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleSeven] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1);
			}
		));
	/* 大南西諸島 */
	if (RuleData::chkRuleApplied("dai_nansei_shotou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大南西諸島", get_yaku_han("dai_nansei_shotou"),
			"対々和", "三色同刻", "混老頭",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1);
			}
		));
	/* 白衣の天使 */
	if (RuleData::chkRuleApplied("hakuinotenshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白衣の天使", get_yaku_han("hakuinotenshi"),
			"役牌・白",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 7) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(countKangziOf(analysis, 5) >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* 七龍珠 */
	if (RuleData::chkRuleApplied("dragon_ball"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"七龍珠", get_yaku_han("dragon_ball"),
			"対々和", "大三元", "混一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] - analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1);
			}
		));
	/* 七五三 */
	if (RuleData::chkRuleApplied("shichigosan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"七五三", get_yaku_han("shichigosan"),
			"奇数対々和", "奇数七対子", "断幺九",
			[countTilesOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countTilesOf(analysis, 7) + countTilesOf(analysis, 5) + countTilesOf(analysis, 3) == NUM_OF_TILES_IN_HAND) &&
					(countTilesOf(analysis, 7) * countTilesOf(analysis, 5) * countTilesOf(analysis, 3));
			}
		));
	/* 御苦労さん */
	if (RuleData::chkRuleApplied("gokurousan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"御苦労さん", get_yaku_han("gokurousan"),
			"対々和", "筋牌刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 5] >= 1) && (analysis->KeziCount[i + 9] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) && (analysis->KeziCount[i + 3] >= 1))
						flag = true;
				return flag;
			}
		));
	/* ニコニコ役満 */
	if (RuleData::chkRuleApplied("nikoniko_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ニコニコ役満", get_yaku_han("nikoniko_yakuman"),
			"対々和", "断幺九",
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 2) + countDuiziOf(analysis, 5) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* 風雪宝双風 */
	if (RuleData::chkRuleApplied("feng_xue_bao_shangfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"風雪宝双風", get_yaku_han("feng_xue_bao_shangfeng"),
			"対々和", "混老頭",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (Wind2Tile(analysis->GameStat->GameRound / 4) != // ダブ風でなくて
					Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) &&
					(analysis->DuiziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))] >= 1) && // 自風があり
					(analysis->DuiziCount[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1) && // 場風があり
					(analysis->DuiziCount[CircleOne] >= 1) && (analysis->DuiziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1);
			}
		));
	/* 遥か彼方 */
	if (RuleData::chkRuleApplied("haruka_kanata"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"遥か彼方", get_yaku_han("haruka_kanata"),
			"役牌・中", "役牌・發", "三暗刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->AnKeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[RedDragon] >= 1) &&
					(analysis->AnKeziCount[WhiteDragon] >= 1);
			}
		));
	/* 激四暗刻 */
	if (RuleData::chkRuleApplied("geki_sianke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"激四暗刻", get_yaku_han("geki_sianke"),
			"四暗刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && // 四暗刻を
					(analysis->TsumoHai->tile == CharacterOne); // 一萬で和了る
			}
		));
	/* 激大三元 */
	if (RuleData::chkRuleApplied("geki_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"激大三元", get_yaku_han("geki_daisangen"),
			"大三元",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiPenchan);
			}
		));
	/* 激北 */
	if (RuleData::chkRuleApplied("geki_north"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"激北", get_yaku_han("geki_north"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* 大九和 */
	if (RuleData::chkRuleApplied("daikyuuhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大九和", get_yaku_han("daikyuuhoh"),
			"三色同刻", "対々和", "混老頭", "五門斉",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1);
			}
		));
	/* 大林間 */
	if (RuleData::chkRuleApplied("dairinkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大林間", get_yaku_han("dairinkan"),
			"役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* 藤子スペシャル */
	if (RuleData::chkRuleApplied("fujikospecial"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"藤子スペシャル", get_yaku_han("fujikospecial"),
			"役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 1; i < TILE_NONFLOWER_MAX; i++)
					if (analysis->GameStat->DoraFlag.Omote[i] && analysis->KeziCount[i])
						flag = true;
				return flag &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* フジテレビ */
	if (RuleData::chkRuleApplied("fujitv"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"フジテレビ", get_yaku_han("fujitv"),
			"三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1);
			}
		));
	/* 東アジア */
	if (RuleData::chkRuleApplied("eastasia"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東アジア", get_yaku_han("eastasia"),
			"役牌・中", "対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* バッハ */
	if (RuleData::chkRuleApplied("bach"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"バッハ", get_yaku_han("bach"),
			"二色同刻",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				unsigned count = 0;
				for (int i = CircleOne; i <= CircleSeven; i++) {
					count += analysis->DuiziCount[i];
					count += analysis->ShunziCount[i];
				}
				count += analysis->DuiziCount[CircleNine];
				return (countKeziOf(analysis, 8) >= 2) &&
					(count >= 3);
			}
		));
	/* 北島三郎 */
	if (RuleData::chkRuleApplied("kitajima_saburoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北島三郎", get_yaku_han("kitajima_saburoh"),
			"混一色", "四帰一",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool pairIsHonor = false; bool flag = false;
				for (int i = TILE_SUIT_HONORS; i < TILE_NONFLOWER_MAX; i++)
					if (analysis->MianziDat[0].tile == (tileCode)i)
						pairIsHonor = true;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					if ((analysis->KeziCount[i + 3] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) &&
						(analysis->KeziCount[NorthWind] >= 1) &&
						(pairIsHonor)) {
							if ((analysis->ShunziCount[i + 4] >= 1) &&
								(analysis->TsumoHai->tile == (tileCode)(i + 6)))
								flag = true;
							if ((analysis->ShunziCount[i + 3] >= 1) &&
								(analysis->TsumoHai->tile == (tileCode)(i + 3)))
								flag = true;
					}
				}
				return flag;
			}
		));
	/* 大鎌の死神 */
	if (RuleData::chkRuleApplied("reaper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大鎌の死神", get_yaku_han("reaper"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* 越谷 */
	if (RuleData::chkRuleApplied("koshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"越谷", get_yaku_han("koshigaya"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* 北越谷 */
	if (RuleData::chkRuleApplied("kitakoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北越谷", get_yaku_han("kitakoshigaya"),
			"越谷",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[NorthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* 南越谷 */
	if (RuleData::chkRuleApplied("minamikoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南越谷", get_yaku_han("minamikoshigaya"),
			"越谷",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* 和洋中役満 */
	if (RuleData::chkRuleApplied("wayouchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"和洋中役満", get_yaku_han("wayouchuu"),
			"対々和", "役牌・中",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* 閏年役満 */
	if (RuleData::chkRuleApplied("leap_year"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"閏年役満",
			yaku::yakuCalculator::Yaku::HANFUNC(
				[](const MENTSU_ANALYSIS* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
					SYSTEMTIME nowTime; GetLocalTime(&nowTime);
					if ((nowTime.wMonth == 2) && (nowTime.wDay == 29))
						return yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman;
					else return yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman;
				}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				bool isLeapYear = (nowTime.wYear % 400 == 0) || ((nowTime.wYear % 4 == 0) && (nowTime.wYear % 100 != 0));
				if (!isLeapYear) return false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - '0') * TILE_SUIT_STEP + 2] >= 1) &&
						(analysis->KeziCount[((*k)[1] - '0') * TILE_SUIT_STEP + 2] >= 1) &&
						(analysis->KeziCount[((*k)[1] - '0') * TILE_SUIT_STEP + 9] >= 1))
						return true;
				return false;
			}
		));
	/* Windows8 */
	if (RuleData::chkRuleApplied("windows8"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows8", get_yaku_han("windows8"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) &&
					(!*analysis->TsumoAgariFlag)) ? 1 : 0) >= 3);
			}
		));
	/* 侵略イカ娘 */
	if (RuleData::chkRuleApplied("ikamusume"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"侵略イカ娘", get_yaku_han("ikamusume"),
			"役牌・白", "役牌・發",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[EastWind] - analysis->AnKangziCount[EastWind] +
					analysis->AnKeziCount[SouthWind] - analysis->AnKangziCount[SouthWind] +
					analysis->AnKeziCount[WestWind] - analysis->AnKangziCount[WestWind] +
					analysis->AnKeziCount[NorthWind] - analysis->AnKangziCount[NorthWind]) >= 1) &&
					((analysis->AnKeziCount[WhiteDragon] - analysis->AnKangziCount[WhiteDragon]) >= 1) &&
					((analysis->AnKeziCount[GreenDragon] - analysis->AnKangziCount[GreenDragon]) >= 1);
			}
		));
	/* あずま寿し */
	if (RuleData::chkRuleApplied("azumazushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"あずま寿し", get_yaku_han("azumazushi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CharacterFour) ||
					(analysis->MianziDat[0].tile == CircleFour) ||
					(analysis->MianziDat[0].tile == BambooFour)) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* 西新井 */
	if (RuleData::chkRuleApplied("nishiarai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西新井", get_yaku_han("nishiarai"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					((analysis->TsumoHai->tile == CharacterOne) ||
					(analysis->TsumoHai->tile == CircleOne) ||
					(analysis->TsumoHai->tile == BambooOne) ||
					(analysis->TsumoHai->tile == CharacterTwo) ||
					(analysis->TsumoHai->tile == CircleTwo) ||
					(analysis->TsumoHai->tile == BambooTwo));
			}
		));
	/* 金剛組 */
	if (RuleData::chkRuleApplied("kongougumi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金剛組", get_yaku_han("kongougumi"),
			"対々和",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 2) &&
					(countKeziOf(analysis, 9) >= 1) &&
					(countKeziOf(analysis, 3) >= 1);
			}
		));
	/* 東日本復興 */
	if (RuleData::chkRuleApplied("eastjapan_fukkou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東日本復興", get_yaku_han("eastjapan_fukkou"),
			"対々和",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(countKeziOf(analysis, 2) >= 1) &&
					(countKeziOf(analysis, 5) >= 1);
			}
		));
	/* 正露丸 */
	if (RuleData::chkRuleApplied("seirogan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"正露丸", get_yaku_han("seirogan"),
			"役牌・發",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* 朱雀門 */
	if (RuleData::chkRuleApplied("suzakumon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"朱雀門", get_yaku_han("suzakumon"),
			"対々和", "混一色", "役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooFour] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == SouthWind);
			}
		));
	/* 全六刻 */
	if (RuleData::chkRuleApplied("quanliuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"全六刻", get_yaku_han("quanliuke"),
			"対々和", "役牌・發",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSix] >= 1) &&
					(analysis->KeziCount[CircleSix] >= 1) &&
					(analysis->KeziCount[BambooSix] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* 全玖刻 */
	if (RuleData::chkRuleApplied("quanjiuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"全玖刻", get_yaku_han("quanjiuke"),
			"対々和", "役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* 白鳥湖 */
	if (RuleData::chkRuleApplied("swanlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白鳥湖", get_yaku_han("swanlake"),
			"対々和", "役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));
	/* 黒鳥湖 */
	if (RuleData::chkRuleApplied("blackbirdlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"黒鳥湖", get_yaku_han("blackbirdlake"),
			"対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* 大日本史 */
	if (RuleData::chkRuleApplied("dainihonshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大日本史", get_yaku_han("dainihonshi"),
			"対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1) &&
					(analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[BambooFour] >= 1);
			}
		));
	/* 大化の改新 */
	if (RuleData::chkRuleApplied("taikanokaishin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大化の改新", get_yaku_han("taikanokaishin"),
			"対々和", "役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - '0') * TILE_SUIT_STEP + 6] >= 1) &&
						(analysis->KeziCount[((*k)[1] - '0') * TILE_SUIT_STEP + 4] >= 1) &&
						(analysis->KeziCount[((*k)[2] - '0') * TILE_SUIT_STEP + 5] >= 1))
						flag = true;
				return flag &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* 青山横北郭白水遶東城 */
	if (RuleData::chkRuleApplied("song_youren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青山横北郭白水遶東城", get_yaku_han("song_youren"),
			"対々和", "役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[BambooThree] - analysis->AnKeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[NorthWind] - analysis->AnKeziCount[NorthWind] >= 1);
			}
		));
}
