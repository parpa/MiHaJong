#include "rankval.h"

#include <cstring>
#include <cstdlib>
#include "decomp.h"
#include "reader/csv2arry.h"
#include "../common/strcode.h"

/* コンストラクタ(シングルトン) */
RankVal::RankVal() {
	Compressed::file_rankval_csv rankvalCsv; // 設定ファイル
	char* csvDatUtf8 = new char[rankvalCsv.getDataSize() + 4]; // データバッファ(UTF-8形式)
	memset(csvDatUtf8, 0, rankvalCsv.getDataSize() + 4);
	memcpy(csvDatUtf8, reinterpret_cast<const char*>(rankvalCsv.getData()), rankvalCsv.getDataSize()); // データ読み込み
	CSVReader::CsvVecVec parsedCsv; // パースされたCSVがここに入る
	CodeConv::tstring csvDat(CodeConv::fromUTF8(csvDatUtf8)); // 文字コード変換(このデータの場合あまり意味が無いが)
	delete[] csvDatUtf8;
	CSVReader::parsecsv(parsedCsv, csvDat.c_str()); // CSVをパース

	/* 数値に直して格納 */
	for (int i = 1 /* 最初の行は見出しなので飛ばす */; i < parsedCsv.size(); ++i) {
		RankValSet values;
		for (int j = 0; j < Players + 1; ++j)
			for (int k = 0; k < Players - 1; ++k)
				values[j][k] = _ttoi(parsedCsv[i][j * (Players - 1) + k + 1].c_str());
		const std::string key(CodeConv::toANSI(parsedCsv[i][0]));
		rankValueMap.insert(std::make_pair(key, values));
	}
}

/* シングルトン アクセサ */
RankVal* RankVal::Instantiate() {
	static RankVal myInstance;
	return &myInstance;
}

/* 値を取得 */
int RankVal::getRankVal(const GameTable* gameStat, const std::string& ruletag, unsigned playersAboveBase, unsigned rank) const {
	using namespace CodeConv;
	const std::string ruleTagVal =
		std::string(gameStat->chkGameType(SanmaT) ? "T:" : "Q:") + ruletag;
	if (rankValueMap.find(ruleTagVal) == rankValueMap.end()) { /* Index error */
		tstring msg =
			tstring(_T("getRankVal: 対応していないルールタグです [")) +
			EnsureTStr(ruletag) + tstring(_T("]"));
		warn(msg.c_str());
		return 0;
	} else if (playersAboveBase > Players) { /* Index error */
		tostringstream o; o << _T("getRankVal: playersAboveBase 引数が正しくありません [") << playersAboveBase << _T(']');
		warn(o.str().c_str());
		return 0;
	} else if ((rank <= 0) || (rank > Players)) { /* Index error */
		tostringstream o; o << _T("getRankVal: rank 引数が正しくありません [") << rank << _T(']');
		warn(o.str().c_str());
		return 0;
	} else if (rank == 1) { /* トップの場合 */
		return -1 * (
			rankValueMap.at(ruleTagVal)[playersAboveBase][0] +
			rankValueMap.at(ruleTagVal)[playersAboveBase][1] +
			rankValueMap.at(ruleTagVal)[playersAboveBase][2]);
	} else { /* トップ以外の場合 */
		//return rankValueMap[ruleTagVal][playersAboveBase][rank - 2]; // なぜかoperator[]だとエラーになる。意味不明。
		return rankValueMap.at(ruleTagVal)[playersAboveBase][rank - 2];
	}
}
