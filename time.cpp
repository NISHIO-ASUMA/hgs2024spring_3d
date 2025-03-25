//=============================================================================
//
// タイム処理 [time.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "time.h"
#include "player.h"
#include "block.h"
//#include "enemy.h"
//#include "sound.h"
#include "game.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTimeMinute = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimeSecond = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureColon = NULL;				// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeMinute = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeSecond = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffColon = NULL;			// 頂点バッファへのポインタ

D3DXVECTOR3 g_posTime;									// タイムの位置
Time g_aTime[MAX_TIMEDIGIT];

int g_nMinutes;											// 分
int g_nSeconds;											// 秒

//=============================
//タイムの初期化処理
//=============================
void InitTime(void)
{

	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み(分)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time.png",
		&g_pTextureTimeMinute);

	//テクスチャの読み込み(秒)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time.png",
		&g_pTextureTimeSecond);

	//テクスチャの読み込み(コロン)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\colon.png",
		&g_pTextureColon);

	//初期化
	g_posTime = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置を初期化する(始まりの位置)
	g_nMinutes = 0;
	g_nSeconds = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMEDIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeMinute,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMEDIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeSecond,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffColon,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_TIMEDIGIT; nCnt++)
	{

		g_aTime[nCnt].bUse = true;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(600.0f + nCnt * 35.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(600.0f + nCnt * 35.0f + 40.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600.0f + nCnt * 35.0f, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600.0f + nCnt * 35.0f + 40.0f, 80.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimeMinute->Unlock();

	// コロンの頂点バッファをロック
	g_pVtxBuffColon->Lock(0, 0, (void**)&pVtx, 0);

	float colonX = 680.0f; // コロンのX座標
	float colonY = 20.0f;  // コロンのY座標

	// コロンの頂点座標設定
	pVtx[0].pos = D3DXVECTOR3(colonX, colonY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(colonX + 20.0f, colonY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(colonX, colonY + 60.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(colonX + 20.0f, colonY + 60.0f, 0.0f);

	for (int nCntColon = 0; nCntColon < 4; nCntColon++)
	{
		pVtx[nCntColon].rhw = 1.0f;
		pVtx[nCntColon].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// コロンのテクスチャ座標設定（1枚の画像の場合は固定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffColon->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimeSecond->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_TIMEDIGIT; nCnt++)
	{

		g_aTime[nCnt].bUse = true;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(700.0f + nCnt * 35.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(700.0f + nCnt * 35.0f + 40.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(700.0f + nCnt * 35.0f, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(700.0f + nCnt * 35.0f + 40.0f, 80.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimeSecond->Unlock();


}
//=============================
//タイムの終了処理
//=============================
void UninitTime(void)
{

	//テクスチャの破棄(分)
	if (g_pTextureTimeMinute != NULL)
	{
		g_pTextureTimeMinute->Release();
		g_pTextureTimeMinute = NULL;
	}

	//テクスチャの破棄(秒)
	if (g_pTextureTimeSecond != NULL)
	{
		g_pTextureTimeSecond->Release();
		g_pTextureTimeSecond = NULL;
	}

	//テクスチャの破棄(コロン)
	if (g_pTextureColon != NULL)
	{
		g_pTextureColon->Release();
		g_pTextureColon = NULL;
	}

	//頂点バッファの破棄(分)
	if (g_pVtxBuffTimeMinute != NULL)
	{
		g_pVtxBuffTimeMinute->Release();
		g_pVtxBuffTimeMinute = NULL;
	}

	//頂点バッファの破棄(秒)
	if (g_pVtxBuffTimeSecond != NULL)
	{
		g_pVtxBuffTimeSecond->Release();
		g_pVtxBuffTimeSecond = NULL;
	}

	//頂点バッファの破棄(コロン)
	if (g_pVtxBuffColon != NULL)
	{
		g_pVtxBuffColon->Release();
		g_pVtxBuffColon = NULL;
	}

}
//=============================
// タイムの更新処理
//=============================
void UpdateTime(void)
{
	//Block* pBlock = GetBlock();
	//bool bEnd = GetEnd();

	VERTEX_2D* pVtx;

	static int nCntTimeSecond = 0;

	nCntTimeSecond++;

	//if (pFlag->bExit == false && bEnd == false && pGame->bMap == false)
	//{
		if (nCntTimeSecond >= 60)			// 1秒ごとに処理
		{
			nCntTimeSecond = 0;

			if (g_nSeconds >= 59)
			{
				g_nMinutes++;				// 分を増やす
				g_nSeconds = 0;				// 秒をリセット
			}
			else if (g_nSeconds < 60)
			{
				g_nSeconds++;				// 秒を増やす
			}

		}
	//}

	int min10 = g_nMinutes / 10;	// 分の10の位
	int min1 = g_nMinutes % 10;		// 分の1の位
	int sec10 = g_nSeconds / 10;	// 秒の10の位
	int sec1 = g_nSeconds % 10;		// 秒の1の位

	float texOffset = 0.1f;			// 1桁分のテクスチャ範囲（10分割）

	//===============================
	// 分のテクスチャ設定
	//===============================

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	// 分の10の位 
	pVtx[0].tex = D3DXVECTOR2(texOffset * min10, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(texOffset * min10 + texOffset, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(texOffset * min10, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(texOffset * min10 + texOffset, 1.0f);

	// 分の1の位
	pVtx[4].tex = D3DXVECTOR2(texOffset * min1, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(texOffset * min1 + texOffset, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(texOffset * min1, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(texOffset * min1 + texOffset, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTimeMinute->Unlock();

	//===============================
	// 秒のテクスチャ設定
	//===============================

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimeSecond->Lock(0, 0, (void**)&pVtx, 0);

	// 秒の10の位
	pVtx[0].tex = D3DXVECTOR2(texOffset * sec10, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(texOffset * sec10 + texOffset, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(texOffset * sec10, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(texOffset * sec10 + texOffset, 1.0f);

	// 秒の1の位
	pVtx[4].tex = D3DXVECTOR2(texOffset * sec1, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(texOffset * sec1 + texOffset, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(texOffset * sec1, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(texOffset * sec1 + texOffset, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTimeSecond->Unlock();

}
//=============================
//タイムの描画処理
//=============================
void DrawTime(void)
{

	//必要桁数分の描画

	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//=================
	// 分
	//=================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeMinute, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < MAX_TIMEDIGIT; nCnt++)
	{
		if (g_aTime[nCnt].bUse == true)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTimeMinute);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

		}
	}


	//=================
	// コロン
	//=================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffColon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureColon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//=================
	// 秒
	//=================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeSecond, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < MAX_TIMEDIGIT; nCnt++)
	{
		if (g_aTime[nCnt].bUse == true)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTimeSecond);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

		}
	}

}
//=============================
//タイム(分)の取得
//=============================
int GetTimeMinutes(void)
{
	return g_nMinutes;
}
//=============================
//タイム(秒)の取得
//=============================
int GetTimeSeconds(void)
{
	return g_nSeconds;
}
