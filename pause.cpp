//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "pause.h"
#include "main.h"
#include "game.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTexturePause[MAX_PAUSE] = {};		//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_apTexturePauseBG = NULL;				//テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseBG = NULL;			//頂点バッファへのポインタ

PAUSE_MENU g_pauseMenu;										//ポーズメニュー

// ポーズ項目の拡大率を管理する配列
float pauseScales[MAX_PAUSE] = { PAUSE_MIN_SCALE, PAUSE_MIN_SCALE, PAUSE_MIN_SCALE };

//===============================================================
//ポーズの初期化処理
//===============================================================
void InitPause(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	int nCntPause;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\.png",
		&g_apTexturePauseBG);

	for (int i = 0; i < MAX_PAUSE; i++)
	{
		D3DXCreateTextureFromFile(pDevice,
			PAUSE_TEXTURE[i],
			&g_apTexturePause[i]);
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseBG,
		NULL);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);


	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPauseBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);//2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);//3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);//4つ目の頂点情報

	//rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロックする
	g_pVtxBuffPauseBG->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(500.0f, (150.0f + nCntPause * 150), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(800.0f, (150.0f + nCntPause * 150), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(500.0f, (250.0f + nCntPause * 150), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(800.0f, (250.0f + nCntPause * 150), 0.0f);


		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	g_pauseMenu = PAUSE_MENU_CONTINUE;

}
//===============================================================
//ポーズの終了処理
//===============================================================
void UninitPause(void)
{

	//StopSound(SOUND_LABEL_PAUSE);
	//StopSound(SOUND_LABEL_PAUSE2);

	//テクスチャの破棄
	if (g_apTexturePauseBG != NULL)
	{
		g_apTexturePauseBG->Release();
		g_apTexturePauseBG = NULL;
	}

	int nCnt;

	for (nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		//テクスチャの破棄
		if (g_apTexturePause[nCnt] != NULL)
		{
			g_apTexturePause[nCnt]->Release();
			g_apTexturePause[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPauseBG != NULL)
	{
		g_pVtxBuffPauseBG->Release();
		g_pVtxBuffPauseBG = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

}
//===============================================================
//ポーズの更新処理
//===============================================================
void UpdatePause(void)
{
	FADE g_fade = GetFade(); // 現在の状態

	VERTEX_2D* pVtx;

	if (KeyboardTrigger(DIK_UP) || JoypadTrigger(JOYKEY_UP))
	{
		//PlaySound(SOUND_LABEL_PAUSE2);
		g_pauseMenu = static_cast<PAUSE_MENU>((g_pauseMenu - 1 + MAX_PAUSE) % MAX_PAUSE);
	}
	else if (KeyboardTrigger(DIK_DOWN) || JoypadTrigger(JOYKEY_DOWN))
	{
		//PlaySound(SOUND_LABEL_PAUSE2);
		g_pauseMenu = static_cast<PAUSE_MENU>((g_pauseMenu + 1) % MAX_PAUSE);
	}

	// 拡大率の更新
	for (int p = 0; p < MAX_PAUSE; p++)
	{
		if (p == g_pauseMenu)
		{
			// 選択中の項目を拡大（スムーズに MAX_SCALE に近づく）
			if (pauseScales[p] < PAUSE_MAX_SCALE)
			{
				pauseScales[p] += PAUSE_SCALE_SPEED;
			}
		}
		else
		{
			// 非選択項目を縮小（スムーズに MIN_SCALE に近づく）
			if (pauseScales[p] > PAUSE_MIN_SCALE)
			{
				pauseScales[p] -= PAUSE_SCALE_SPEED;
			}
		}
	}

	//頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		float scale = pauseScales[nCntPause];
		float centerX = 650.0f;
		float centerY = 200.0f + nCntPause * 150.0f;

		// 頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(centerX - 150.0f * scale, centerY - 50.0f * scale, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(centerX + 150.0f * scale, centerY - 50.0f * scale, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(centerX - 150.0f * scale, centerY + 50.0f * scale, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(centerX + 150.0f * scale, centerY + 50.0f * scale, 0.0f);

		// カラー設定（選択中なら白、不選択なら薄い白）
		D3DXCOLOR color;

		if (nCntPause == g_pauseMenu)
		{
			color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		}

		for (int nCntPause2 = 0; nCntPause2 < 4; nCntPause2++)
		{
			pVtx[nCntPause2].col = color;
		}

		pVtx += 4;
	}

	//頂点バッファのアンロック
	g_pVtxBuffPause->Unlock();


	if (g_fade == FADE_NONE && (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true))
	{//決定(ENTER)キーが押された
		//メニューに合わせてモードの切り替え

		//PlaySound(SOUND_LABEL_PAUSE);

		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);

			break;
		case PAUSE_MENU_RETRY:
			SetFade(MODE_GAME);

			break;
		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);

			break;
		}

	}

}
//===============================================================
//ポーズの描画処理
//===============================================================
void DrawPause(void)
{
	int nCntPause;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPauseBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, /*g_apTexturePauseBG*/NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);

	}

}