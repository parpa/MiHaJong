#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_suit() {
	/* チンイツ */
#include "isshoku.h" /* やぶれかぶれ。行儀の悪いinclude */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"清一色", (getRule(RULE_CHINIISOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_4han_kuisagari : yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
		"混一色", "純一気通貫全帯幺九",
		[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, true);
		}
	));

	// ---------------------------------------------------------------------

	/* 和了ったら死ぬ */
	auto chkHaishiki =
		[](const MENTSU_ANALYSIS* const analysis, const char* const haishiki) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
				if ((analysis->TileCount[i + 1] >= (int)(haishiki[0] - '0')) &&
					(analysis->TileCount[i + 2] >= (int)(haishiki[1] - '0')) &&
					(analysis->TileCount[i + 3] >= (int)(haishiki[2] - '0')) &&
					(analysis->TileCount[i + 4] >= (int)(haishiki[3] - '0')) &&
					(analysis->TileCount[i + 5] >= (int)(haishiki[4] - '0')) &&
					(analysis->TileCount[i + 6] >= (int)(haishiki[5] - '0')) &&
					(analysis->TileCount[i + 7] >= (int)(haishiki[6] - '0')) &&
					(analysis->TileCount[i + 8] >= (int)(haishiki[7] - '0')) &&
					(analysis->TileCount[i + 9] >= (int)(haishiki[8] - '0')) ) yakuFlag = true;
			return yakuFlag;
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"九蓮宝燈", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"清一色",
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkHaishiki(analysis, "311111113");
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純正九蓮宝燈", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"九蓮宝燈", "清一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const tileCode* tsumoTile = &(analysis->TsumoHai->tile);
				if ((*tsumoTile % 10 == 1) || (*tsumoTile % 10 == 9)) {
					if (analysis->TileCount[*tsumoTile] >= 4) yakuFlag = true;
				} else {
					if (analysis->TileCount[*tsumoTile] >= 2) yakuFlag = true;
				}
				return (chkHaishiki(analysis, "311111113")) && (!analysis->MachiInfo.FuritenFlag);
			}
		));
	/* 某青い子 */
	if (getRule(RULE_DAISHARIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大車輪", (getRule(RULE_DAISHARIN) == 2) ?
			yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口", "七対子", "平和", "断幺九",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CircleFive] >= 2));
			}
		));
	/* 大竹林 */
	if (getRule(RULE_DAICHIKURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大竹林", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口", "七対子", "平和", "断幺九",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[BambooFive] >= 2));
			}
		));
	/* 大数隣 */
	if (getRule(RULE_DAISUURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大数隣", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口", "七対子", "平和", "断幺九", "橙一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CharacterFive] >= 2));
			}
		));
	/* 小車輪 */
	if (getRule(RULE_SHOSHARIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小車輪", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口", "七対子", "平和",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CircleFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* 小竹林 */
	if (getRule(RULE_SHOCHIKURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小竹林", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口", "七対子", "平和",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[BambooFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* 小数隣 */
	if (getRule(RULE_SHOSUURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小数隣", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口", "七対子", "平和",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CharacterFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* 円周率和 (3.)14159265358979 */
	if (getRule(RULE_PI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"円周率和", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "211131113");
			}
		));

	// ---------------------------------------------------------------------

	/* √２和 1.4142135623730 */
	if (getRule(RULE_SQUARE_ROOT_TWO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"√２和", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "323211100");
			}
		));
	/* 自然対数の底 2.7182818284590 */
	if (getRule(RULE_NATURAL_LOGARITHM) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"自然対数の底", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "230110141");
			}
		));
	/* 虚数和 (0.)207879576350761 */
	if (getRule(RULE_IMAGINARY_NUMBER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"虚数和", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "111022411");
			}
		));
	/* √１０和 3.16227766016837 */
	if (getRule(RULE_SQUARE_ROOT_TEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"√１０和", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "222004310");
			}
		));

	// ---------------------------------------------------------------------

	/* 清海湖 */
	if (getRule(RULE_QINGHAIHU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"清海湖", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "純全帯幺九", "二盃口", "平和",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((chkHaishiki(analysis, "422000222") || chkHaishiki(analysis, "222000224")));
			}
		));
	/* 中車輪 */
	auto chuusharin =
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
			return ((chkHaishiki(analysis, "202222220") || chkHaishiki(analysis, "222022220") ||
				chkHaishiki(analysis, "222202220") || chkHaishiki(analysis, "222222020") ||
				chkHaishiki(analysis, "022222202") || chkHaishiki(analysis, "022220222") ||
				chkHaishiki(analysis, "022202222") || chkHaishiki(analysis, "020222222")) &&
				((analysis->TileCount[suit + 4] >= 2) || (analysis->TileCount[suit + 5] >= 2))
				);
		};
	if (getRule(RULE_CHUUSHARIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中車輪", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口",
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_CIRCLES);
			}
		));
	/* 中竹林 */
	if (getRule(RULE_CHUUCHIKURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中竹林", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口",
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_BAMBOOS);
			}
		));
	/* 中数隣 */
	if (getRule(RULE_CHUUSUURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中数隣", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"清一色", "二盃口",
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_CHARACTERS);
			}
		));

	// ---------------------------------------------------------------------

	/* 百万石 */
	auto chiffre =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return (analysis->shanten[shantenRegular] == -1) ?
				yaku::countingFacility::countMentzNumerals(analysis->MianziDat) :
				yaku::countingFacility::countTileNumerals(analysis->TileCount);
		};
	if (getRule(RULE_KAGA_MILLION) != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"百万石", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) > 100) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"加賀百万石", (getRule(RULE_KAGA_MILLION) == 2) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"清一色",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 100) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
	}
	/* 筒子百万石 */
	if (getRule(RULE_PINZU_MILLION) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"筒子百万石", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) >= 100) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CIRCLES / TILE_SUIT_STEP)));
			}
		));
	/* 紀州五十五万石 */
	if (getRule(RULE_KISHU550K) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紀州五十五万石", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 55) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
	/* 水戸三十五万石 */
	if (getRule(RULE_MITO350K) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"水戸三十五万石", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) <= 35) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
	/* フルムーン */
	if (getRule(RULE_FULLMOON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"フルムーン", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 88));
			}
		));

	// ---------------------------------------------------------------------

	/* 星光燦爛 */
	if (getRule(RULE_STARLIGHT) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"星光燦爛", yaku::yakuCalculator::Yaku::yval_1han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (analysis->KeziCount[CircleSeven]));
			}
		));
	/* 橙一色 */
	if (getRule(RULE_CHENIISOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"橙一色", yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han),
			"清一色", "断幺九",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if (analysis->shanten[shantenRegular] == -1) {
					int yakuFlagCount = 0;
					const tileCode targetKezi[] = {CharacterOne, CharacterNine,};
					const tileCode targetShunzi[] = {CharacterOne, CharacterSeven,};
					if (yaku::countingFacility::countSpecMentz(analysis->MianziDat, targetKezi, 2, targetShunzi, 2, false) == 0)
						yakuFlag = true;
				} else if (analysis->shanten[shantenPairs] == -1) {
					int yakuFlagCount = 0;
					const tileCode targetDuizi[] = {CharacterOne, CharacterNine,};
					if (yaku::countingFacility::countPairs(analysis->TileCount, targetDuizi, 2) == 0)
						yakuFlag = true;
				}
				return (isshoku(analysis, true) && (yakuFlag) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));

	// ---------------------------------------------------------------------

	/* ホンイツ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"混一色", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		"一気通貫全帯幺九",
		[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, false);
		}
	));

	// ---------------------------------------------------------------------

	/* 三麻の萬子ホンイツ */
	if ((getRule(RULE_CHARACTERS_MAHJONG) != 0) && chkGameType(&GameStat, SanmaT))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"萬和", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混一色", "混老頭",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					((analysis->TileCount[CharacterOne] >= 1)||(analysis->TileCount[CharacterNine] >= 1)));
			}
		));
	/* 東京オリンピック(ホンイツ) */
	if (getRule(RULE_TOKYO_OLYMPIC_MONOCHROME) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東京オリンピック", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混一色",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					((analysis->TileCount[CircleFive] >= 1)||(analysis->TileCount[EastWind] >= 1)));
			}
		));
	/* 西郷南州 */
	if (getRule(RULE_SAIGOU_NANSHUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西郷南州", yaku::yakuCalculator::Yaku::yval_2han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1));
			}
		));
	/* 鏡音リンレン */
	if (getRule(RULE_KAGAMINE_RINLEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鏡音リンレン", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混一色", "鏡音リン", "鏡音レン", "三連刻", "対々和",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i + 1] >= 1) && (analysis->KeziCount[i + 2] >= 1))
						yakuFlag = true;
				return (isshoku(analysis, false) && yakuFlag &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP)));
			}
		));
	/* 鏡音リン */
	if (getRule(RULE_KAGAMINE_RIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鏡音リン", yaku::yakuCalculator::Yaku::yval_2han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				int PinMian = 0;
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP))
						++PinMian;
				return (isshoku(analysis, false) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* 翻満乃歩手地 */
	if (getRule(RULE_HONMANOPOTECHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"翻満乃歩手地", yaku::yakuCalculator::Yaku::yval_1han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				int PinMian = 0;
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP))
						++PinMian;
				return (isshoku(analysis, false) &&
					(PinMian == SIZE_OF_MELD_BUFFER - 1) &&
					(analysis->DuiziCount[GreenDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 絶一門 */
	auto chueiimen =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool flag[TILE_SUIT_HONORS / TILE_SUIT_STEP] = {false};
			for (int k = 1; k < TILE_SUIT_HONORS; k++)
				if (analysis->TileCount[k] > 0) flag[k] = true;
			return (
				(flag[TILE_SUIT_CHARACTERS / TILE_SUIT_STEP] && flag[TILE_SUIT_CIRCLES / TILE_SUIT_STEP] && !flag[TILE_SUIT_BAMBOOS / TILE_SUIT_STEP]) ||
				(flag[TILE_SUIT_CHARACTERS / TILE_SUIT_STEP] && !flag[TILE_SUIT_CIRCLES / TILE_SUIT_STEP] && flag[TILE_SUIT_BAMBOOS / TILE_SUIT_STEP]) ||
				(!flag[TILE_SUIT_CHARACTERS / TILE_SUIT_STEP] && flag[TILE_SUIT_CIRCLES / TILE_SUIT_STEP] && flag[TILE_SUIT_BAMBOOS / TILE_SUIT_STEP])
				);
		};
	std::function<bool(const MENTSU_ANALYSIS* const analysis)> chueiimen1 =
		[chueiimen](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chueiimen(analysis);
		};
	std::function<bool(const MENTSU_ANALYSIS* const analysis)> chueiimen2 =
		[chueiimen](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chueiimen(analysis) &&
				(analysis->TileCount[EastWind] + analysis->TileCount[SouthWind] + analysis->TileCount[WestWind] +
				analysis->TileCount[NorthWind] + analysis->TileCount[WhiteDragon] + analysis->TileCount[GreenDragon] +
				analysis->TileCount[RedDragon] == 0));
		};
	if (getRule(RULE_CHUEIIMEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"絶一門", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			(getRule(RULE_CHUEIIMEN) != 2) ? chueiimen1 : chueiimen2
		));
	/* 四面楚歌 */
	if (getRule(RULE_SIMIAN_CHUGE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四面楚歌", yaku::yakuCalculator::Yaku::yval_4han,
			"絶一門",
			[chueiimen2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chueiimen2(analysis) &&
					(analysis->shanten[shantenRegular] == -1) &&
					((analysis->MianziDat[1].tile / TILE_SUIT_STEP) ==
					(analysis->MianziDat[2].tile / TILE_SUIT_STEP) ==
					(analysis->MianziDat[3].tile / TILE_SUIT_STEP) ==
					(analysis->MianziDat[4].tile / TILE_SUIT_STEP)) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki)
					);
			}
		));
	/* 五門斎 */
	auto uumenchii =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int mnzCount[TILE_SUIT_HONORS / TILE_SUIT_STEP + 2] = {0};
			bool yakuFlag = true;
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				if (analysis->MianziDat[i].tile < TILE_SUIT_HONORS)
					++mnzCount[analysis->MianziDat[i].tile / TILE_SUIT_STEP];
				else switch (analysis->MianziDat[i].tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind:
					++mnzCount[TILE_SUIT_HONORS / TILE_SUIT_STEP]; break;
				case WhiteDragon: case GreenDragon: case RedDragon:
					++mnzCount[TILE_SUIT_HONORS / TILE_SUIT_STEP + 1]; break;
				}
			}
			for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP + 2); i++)
				if (mnzCount[i] != 1) yakuFlag = false;
			return yakuFlag;
		};
	if (getRule(RULE_UUMENCHII) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"五門斎", yaku::yakuCalculator::Yaku::yval_2han,
			uumenchii
		));
	/* 東京オリンピック */
	if (getRule(RULE_TOKYO_OLYMPIC_POLYCHROME) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東京オリンピック ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"五門斎",
			[uumenchii](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (uumenchii(analysis) &&
					(analysis->GameStat->GameRound / 4 == sEast) &&
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))
					);
			}
		));
	/* 未成年 */
	if (getRule(RULE_MINORAGE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"未成年", (getRule(RULE_MINORAGE) == 2) ? 
			yaku::yakuCalculator::Yaku::yval_6han : yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if (analysis->MianziDat[i].tile >= TILE_SUIT_HONORS)
						return false;
				return ((analysis->shanten[shantenRegular] == -1) &&
					(yaku::countingFacility::countMentzNumerals(analysis->MianziDat) < 20));
			}
		));
	/* 成人式 */
	if (getRule(RULE_COMINGOFAGE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"成人式", (getRule(RULE_COMINGOFAGE) == 2) ? 
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if (analysis->MianziDat[i].tile >= TILE_SUIT_HONORS)
						return false;
				return ((analysis->shanten[shantenRegular] == -1) &&
					(yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 20));
			}
		));
}
