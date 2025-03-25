//=======================================
//
// リザルトスコア処理 [ resultscore.cpp ]
// Author:Asuma Nishio
//
//========================================

//******************************************
// インクルードファイル宣言
//******************************************
#include "resultscore.h"
#include "result.h"
#include "score.h"
#include <stdio.h>
#include "rankscore.h"

//******************************************
// リザルトスコアの構造体宣言
//******************************************
typedef struct
{
	bool bUse; // 使用しているか否か
}Result;

//******************************************
// グローバル変数宣言
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;		// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			// テクスチャのポインタ

Result g_aResult[MAX_NUM_SCORE];	// リザルトスコアの構造体
int g_nResultScore;					// リザルトスコア

//============================
// リザルトスコアの初期化処理
//============================
void InitResultScore(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number003.png",
		&g_pTextureResultScore);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	// グローバル変数の初期化
	g_nResultScore = 0;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUM_SCORE; nCnt++)
	{
		g_aResult[nCnt].bUse = true;		// 使用している状態

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f + 50.0f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f, 300.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f + 50.0f, 300.0f, 0.0f);

		//r hwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// 頂点情報のポインタを進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffResultScore->Unlock();

	 LoadScore();			// スコアを読み込む
	 SetResultScore();	// リザルトスコアの設定
}
//===========================
// リザルトスコアの終了処理
//===========================
void UninitResultScore(void)
{
	// テクスチャの破棄
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}

}
//===========================
// リザルトスコアの更新処理
//===========================
void UpdateResultScore(void)
{
	// ない
}
//===========================
// リザルトスコアの描画処理
//===========================
void DrawResultScore(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (g_aResult[nCntscore].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResultScore);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntscore * 4, 2);
		}
	}
}
//=============================
// リザルトスコアの設定
//=============================
void SetResultScore()
{
	int aPosTexU[MAX_NUM_SCORE] = {};		// 桁数分の数字を格納
	int aData = NUM_DIGITS_8;				// 8桁
	int aData2 = NUM_DIGITS_7;				// 7桁

	VERTEX_2D* pVtx;			// 頂点情報のポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (nCntscore == 0)		// 0番目の時
		{
			aPosTexU[0] = g_nResultScore / aData;
		}
		else
		{
			// 0番目以外の時
			aPosTexU[nCntscore] = g_nResultScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 1.0f);

		pVtx += 4;

	}

	// アンロック
	g_pVtxBuffResultScore->Unlock();
}
//================================
// スコアの読み込み
//================================
void LoadScore(void)
{
	// ファイルポインタを宣言
	FILE* pFile;

	// ファイル開く
	pFile = fopen("data\\Lastscore.txt", "r");

	if (pFile != NULL)
	{// ファイル開けたら

		// スコア情報を代入
		fscanf(pFile, "%d", &g_nResultScore);
		
	}
	else
	{
		// 開けなかった時
		// メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}