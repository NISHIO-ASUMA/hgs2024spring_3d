//=================================
//
// タイトル処理 [title.cpp]
// Author: Asuma Nishio
//
//=================================

//***************************
// インクルードファイル宣言
//***************************
#include "Title.h"
#include "input.h"

//***************************
// グローバル変数宣言
//***************************
Title g_Title;											// 構造体変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleback = NULL;		// タイトル背景
LPDIRECT3DTEXTURE9 g_pTextureTitleback = NULL;			// タイトル背景テクスチャのポインタ

//===========================
// タイトル初期化処理
//===========================
void InitTitle(void)
{
	// デバイスへのポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;				

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title.png",
		&g_pTextureTitleback);
	
	// 頂点バッファの生成 背景
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleback,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleback->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);		// 2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);		// 3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);	// 4つ目の頂点情報

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	g_pVtxBuffTitleback->Unlock();
	
}
//===================
// タイトルの終了処理
//===================
void UninitTitle(void)
{
	// テクスチャの破棄
	if (g_pTextureTitleback != NULL)
	{
		g_pTextureTitleback->Release();
		g_pTextureTitleback = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleback != NULL)
	{
		g_pVtxBuffTitleback->Release();
		g_pVtxBuffTitleback = NULL;
	}

}
//=======================
// タイトルの更新処理
//=======================
void UpdateTitle(void)
{
#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F1))
	{// F1キーで編集げ面
		// 画面遷移
		SetFade(MODE_EDIT);
	}
#endif
	if (KeyboardTrigger(DIK_RETURN))
	{// Enterキーで画面遷移
		// ゲーム画面
		SetFade(MODE_GAME);	}
}
//===================
// タイトルの描画処理
//===================
void DrawTitle(void)
{
	// デバイスへのポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleback, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleback);

	// タイトルポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
