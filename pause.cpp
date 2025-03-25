//============================
//
// ポーズ処理 [ pause.cpp ]
// Author:Asuma Nishio
//
//============================

//*****************************
// インクルードファイル宣言
//*****************************
#include "pause.h"
#include "fade.h"
#include "input.h"
// #include "game.h"
// #include "sound.h"

//*****************************
// グローバル変数宣言
//*****************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};	// テクスチャへのポインタ(3枚分)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// 頂点バッファのポインタ
															   
LPDIRECT3DTEXTURE9 g_apTexturearufa = NULL;					// テクスチャへのポインタ(透明)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffarufa = NULL;				// 頂点バッファのポインタ
int g_nSelect;				// 選択用の変数

//=========================
// ポーズの初期化処理
//=========================
void InitPause(void)
{
	// デバイスへのポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み(透明)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\arufa.png",
		&g_apTexturearufa);

	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{// メニュー用の選択肢
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			PAUSE_TEX[nCnt],
			&g_apTexturePause[nCnt]);
	}

	// 初期の選択モードを設定
	g_nSelect = PAUSE_MENU_CONTINUE;  

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	//=====================
	// 透明ポリゴンの設定
	//=====================

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffarufa,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffarufa->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // 1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);	 // 2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);	 // 3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);// 4つ目の頂点情報

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	g_pVtxBuffarufa->Unlock();

	//=========================
	// 3つのポリゴン設定
	//=========================

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		//頂点座標1の設定
		pVtx[0].pos = D3DXVECTOR3(470.0f,100.0f +(nCnt * 150.0f),0.0f);	// 1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(870.0f,100.0f +(nCnt * 150.0f),0.0f);	// 2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(470.0f,280.0f +(nCnt * 150.0f),0.0f);	// 3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(870.0f,280.0f +(nCnt * 150.0f),0.0f);	// 4つ目の頂点情報
		 
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

		// 頂点情報分進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffPause->Unlock();

}
//=========================
// ポーズの終了
//=========================
void UninitPause(void)
{
	// 音楽を止める
	// StopSound();

	//=========================
	// 透明ポリゴンの終了
	//=========================
	// テクスチャの終了
	if (g_apTexturearufa != NULL)
	{			   
		g_apTexturearufa->Release();
		g_apTexturearufa = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffarufa != NULL)
	{
		g_pVtxBuffarufa->Release();
		g_pVtxBuffarufa = NULL;
	}

	//=========================
	// 3つのポリゴンの終了
	//=========================
	// テクスチャの終了
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		if (g_apTexturePause[nCnt] != NULL)
		{
			g_apTexturePause[nCnt]->Release();
			g_apTexturePause[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=========================
// ポーズの更新
//=========================
void UpdatePause(void)
{
	// 現在のモードを取得
	MODE nMode = GetMode();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{
		// 頂点カラーを設定(白)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点情報を進める
		pVtx += 4;
	}

	// pVtxを戻す
	pVtx -= 4 * PAUSE_MENU_MAX;		
	
	// メニューの選択
	if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
	{// UPキー or パッドの上キー or Wキーが押された
		g_nSelect--;	// デクリメント
	
		// 音楽再生
		// PlaySound(SOUND_LABLE_PAUSEMENU);

		if (g_nSelect < PAUSE_MENU_CONTINUE)
		{// PAUSE_MENU_CONTINUEより小さかったら
			// 一番下を選択
			g_nSelect = PAUSE_MENU_QUIT;
		}
	}

	else if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true)
	{// DOWNキー or パッドの下キー or Sキーが押された
		//下押したとき
		g_nSelect++;	// インクリメント
		
		// 音楽再生
		// PlaySound(SOUND_LABLE_PAUSEMENU);

		if (g_nSelect >= PAUSE_MENU_MAX)
		{// PAUSE_MENU_MAXより大きかったら
			// 一番上を選択
			g_nSelect = PAUSE_MENU_CONTINUE;
		}
	}

	// 選択肢
	pVtx += 4 * g_nSelect;		

	// 頂点カラーを設定
	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);

	// アンロック
	g_pVtxBuffPause->Unlock();
#if 0
	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true) && nMode == MODE_GAME)
	{// エンターキーが押された or Aキーが押された
		
		//各画面に移動
		switch (g_nSelect)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);	// ゲーム継続
			break;

		case PAUSE_MENU_RETRY:		
			SetFade(MODE_GAME);		// ゲームを最初から
			break;

		case PAUSE_MENU_QUIT:		
			SetFade(MODE_TITLE);	// タイトル遷移
			break;

		}

		// 音楽を再生
		// PlaySound(SOUND_LABLE_PAUSESE);
	}
#endif
}
//=========================
// ポーズ画面の描画
//=========================
void DrawPause(void)
{
	// デバイスへのポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//=========================
	// 透明ポリゴンの描画
	//=========================
	
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffarufa, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTexturearufa);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	

	//=========================
	// 3つのポリゴンの描画
	//=========================

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = 0;	

	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}