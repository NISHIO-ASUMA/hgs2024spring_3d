//=================================
//
// タイトル処理 [title.h]
// Author: Asuma Nishio
//
//=================================
#ifndef _TITLE_H_
#define _TITLE_H_

//****************************
// インクルードファイル宣言
//****************************
#include "main.h"
#include "fade.h"

//****************************
// タイトル構造体宣言
//****************************
typedef struct
{
	D3DXVECTOR3 pos;			// 座標
	int nType;					// 種類
	float fWidth;				// 横幅
	float fHeight;				// 高さ
	bool bUse;					// 使用状態
}Title;

//**********************
// プロトタイプ宣言
//**********************
void InitTitle(void);						// タイトルの初期化
void UninitTitle(void);						// タイトルの終了
void UpdateTitle(void);						// タイトルの更新
void DrawTitle(void);						// タイトルの描画
#endif