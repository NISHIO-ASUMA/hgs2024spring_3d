//===============================
//
// スコア処理 [ score.cpp ]
// Author:Asuma Nishio
//
//===============================

//******************************************
// インクルードファイル宣言
//******************************************
#include "score.h"
#include <cstdio>

//******************************************
// スコア情報の構造体宣言
//******************************************
typedef struct
{
	bool bUse;	// 使用しているか
}Score;

//******************************************
// グローバル変数宣言
//******************************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		// テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// 頂点バッファのポインタ
Score g_aScore[MAX_NUM_SCORE];		// スコア型のグローバル変数
int g_nScore;						// スコアの値

//==========================
//　スコアの初期化処理
//==========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score000.png",
		&g_pTextureScore);

	// グローバル変数の初期化
	g_nScore = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		// 使用状態
		g_aScore[nCntscore].bUse = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f ,			5.0f,  0.0f);
		pVtx[1].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f + 50.0f,	5.0f,  0.0f);
		pVtx[2].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f ,			35.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f + 50.0f,	35.0f, 0.0f);

		// rhwの設定(1.0fで固定)
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

		pVtx += 4;

	}

	// アンロック
	g_pVtxBuffScore->Unlock();

}
//==========================
// スコアの終了処理
//==========================
void UninitScore(void)
{
	// テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

}
//==========================
// スコアの更新処理
//==========================
void UpdateScore(void)
{
	// 求めたスコアの桁数を保存
	int nData = ScoreCounter(g_nScore);

	for ( int nData1 = 0; nData1 < MAX_NUM_SCORE; nData1++)
	{
		if (nData1 >= MAX_NUM_SCORE - nData)
		{
			// MAX_NUM_SCORE - nDataで0が表示されている時(nData1より桁数の値が小さいとき)
			g_aScore[nData1].bUse = true;	// 上の条件を有効化する
		}
		else
		{
			// それ以外の状態の時
			g_aScore[nData1].bUse = false;
		}
	}
}
//==========================
// スコアの描画処理
//==========================
void DrawScore(void)
{	
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE;nCntscore++)
	{
		if (g_aScore[nCntscore].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			// ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntscore * 4, 2);
		}
	}

	// アンロック
	g_pVtxBuffScore->Unlock();

}
//==========================
// スコアの設定処理
//==========================
void SetScore(int nScore)
{
	int aPosTexU[MAX_NUM_SCORE] = {};	// 桁数分の数字を格納
	int aData = NUM_DIGITS_8;				// 8桁
	int aData2 = NUM_DIGITS_7;				// 7桁

	g_nScore = nScore;			// スコアを取得

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;			

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if ( nCntscore == 0)	// 0番目の時
		{
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			// 0番目以外の時
			aPosTexU[nCntscore] = g_nScore % aData / aData2;
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
	g_pVtxBuffScore->Unlock();
}
//========================
// スコアの加算処理
//========================
void AddScore(int nValue)
{
	// 各桁の数字を格納
	int aPosTexU[MAX_NUM_SCORE] = {};		
	int aData = NUM_DIGITS_8;
	int aData2 = NUM_DIGITS_7;

	g_nScore += nValue;

	// 各桁の数字を格納
	VERTEX_2D* pVtx;		

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (nCntscore == 0)		
		{// 0番目だったら
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			// 0番目以外の時
			aPosTexU[nCntscore] = g_nScore %   aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 1.0f);

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffScore->Unlock();
}
//=====================
// スコアの0をカウント
//=====================
int  ScoreCounter(int nScore)
{
	int nNum = nScore;		// スコアの0の部分をカウント
	int nKeepNum = 0;		// スコアの桁数
							   
	while (nNum != 0)		// 割り切れなくなるまで繰り返す(0まで)
	{						   
		nNum /= 10;			// nNumを10で割っていく
		nKeepNum++;			// 桁数を加算
	}
	if (nScore == 0)
	{
		// 1桁目に0が入っているとき
		nKeepNum = 1;
	}
	return nKeepNum;		// スコアの桁数を返す
}
//=====================
// スコアを保存
//=====================
int GetScore(void)
{
	return g_nScore;		// スコアの値を返す
}
//=====================
// スコアの書き出し処理
//=====================
void SaveScore(void)
{
	// ファイルポインタ
	FILE* pFile;

	// ファイルを開く
	pFile = fopen("data\\Lastscore.txt", "w");

	// ファイルを開けたら
	if (pFile != NULL)
	{
		// 配置した数だけ周回
		fprintf(pFile, "%d\n", g_nScore);
	}
	else
	{
		// ファイルが開けなかった時
		// メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}
	// ファイルを閉じる
	fclose(pFile);
}