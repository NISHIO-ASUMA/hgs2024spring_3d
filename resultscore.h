//=============================================
//
// リザルトスコア処理 [ resultscore.h ]
// Author:Asuma Nishio
//
//=============================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

//*********************************************
// インクルードファイル宣言
//*********************************************
#include "main.h"

//*********************************************
// プロトタイプ宣言
//*********************************************
void InitResultScore(void);			// リザルト画面のスコアの初期化
void UninitResultScore(void);		// リザルト画面のスコアの終了
void UpdateResultScore(void);		// リザルト画面のスコアの更新
void DrawResultScore(void);			// リザルト画面のスコアの描画
void SetResultScore(void);			// リザルトスコアの設定
void LoadScore(void);				// リザルトスコアの読み込み

#endif