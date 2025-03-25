//========================================
//
// ゲーム処理 [ game.cpp ]
// Author:Asuma Nishio
//
//========================================

//***********************************************
// インクルードファイル宣言
//***********************************************
#include "game.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "time.h"
#include "meshfield.h"
#include "player.h"
#include "block.h"
#include "Player.h"
#include "fade.h"
#include "score.h"
#include "title.h"
#include "pause.h"

//***********************************************
// グローバル変数宣言
//***********************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲーム状態
int g_nCounterGameState = 0;				// 状態管理カウンター
bool g_bPause = false;						// ポーズ中かどうか

//=====================
// ゲームの初期化処理
//=====================
void InitGame(void)
{
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// メッシュフィールドの初期化
	InitMeshField();

	// モデル読み込み情報をセット
	LoadXFile();

	// ブロックの初期化
	InitBlock();			

	// ポーズの初期化
	InitPause();			

	// 配置したブロック情報の読み込み
	LoadtxtBlock();

#if 0
	InitBackground();		//背景

	InitBullet();			//弾

	InitPlayer();			//プレイヤー

	InitEnemy();			//敵

	InitExplosion();		//爆発

	InitScore();			//スコア

	InitItem();				//アイテム

	InitEffect();			//エフェクト

	InitParticle();			//パーティクル

	InitTimer();			//タイマー

	InitPlayerLifeBar();	//ライフバー

	InitBlock();					//ブロックの初期化

	InitBoss();						// ボスの初期化

	LoadEdit();						// 配置情報の読み込み

	PlaySound(SOUND_LABEL_GAME);
#endif

	g_gameState = GAMESTATE_NORMAL;	//　通常状態
	g_nCounterGameState = 0;		//	カウンターの初期化
	g_bPause = false;				//	ポーズ画面の初期化(ポーズ解除)
}
//===================
// ゲームの終了
//===================
void UninitGame(void)
{
	// ライトの終了
	UninitLight();

	// カメラの終了
	UninitCamera();

	// メッシュフィールドの終了
	UninitMeshField();

	// ブロックの終了処理
	UninitBlock();

	// ポーズの終了処理
	UninitPause();
#if 0
	StopSound();

	SaveScore();			//スコアを保存

	UninitBackground();		//背景

	UninitBullet();			//弾

	UninitPlayer();			//プレイヤー

	UninitEnemy();			//敵の終了

	UninitExplosion();		//爆発

	UninitScore();			//スコア終了

	UninitItem();			//アイテム

	UninitEffect();			//エフェクト

	UninitParticle();		//パーティクル

	UninitPause();			//ポーズ

	UninitTimer();			//タイマー

	UninitPlayerLifeBar();	//ライフバー

	UninitBlock();			//ブロック

	UninitBoss();			// ボスの終了
#endif
}
//===================
// ゲームの更新
//===================
void UpdateGame(void)
{
	// カメラの更新処理
	UpdateCamera();

#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F2))
	{// F2キーを押した
		// エディター起動
		SetFade(MODE_EDIT);
	}

	if (KeyboardTrigger(DIK_RETURN))
	{// ENTERキー
		// リザルトに遷移
		SetFade(MODE_RESULT);

	}


#endif // _DEBUG

	// ポーズ画面
	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START))
	{
		// pが押された or startボタン
		g_bPause = g_bPause ? false : true;
	}


	if (g_bPause == true)
	{// ポーズ中
		UpdatePause();		// ポーズの更新処理
	}
	else
	{
		UpdateBlock();		// ブロックの更新
	}

#if 0
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 敵の取得
	int nNum = GetNumEnemy();

	// タイマーを取得
	int nTime = GetTimer();

	// スイッチの取得
	bool bPush = GetPush();

	// 青スイッチの取得
	bool bPushSwitch = GetPushSwitch();

	// 出口の取得
	bool bExit = GetExit();

	// ボスの取得
	BOSS* pBoss = GetBoss();

	if (pBoss->bUse == false)
	{
		// 出口の位置
		SetExit(D3DXVECTOR3(4940.0f, 620.0f, 0.0f));
	}

	if ((pPlayer->bDisp == false || nTime <= 0 || bExit == true) && g_gameState != GAMESTATE_NONE)
	{

		g_gameState = GAMESTATE_END;  //終了状態

		PlaySound(SOUND_LABLE_EXIT);
	}

	switch(g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;

	case GAMESTATE_END:
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 30)
		{
			//カウンターを初期化
			g_nCounterGameState = 0;

			//1秒経過
			g_gameState = GAMESTATE_NONE;		//何もしていない状態

			//画面設定
			SetFade(MODE_RESULT);

			//読み込み
			ResetRanking();

			//ランキングの設定
			SetRanking(GetScore());
		}
		break;
	}


#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F4) == true)
	{
		// モード変更
		SetFade(MODE_EDIT);
	}
#endif

	else 
	{
		//ポーズ中で無ければ
		UpdateCamera(pPlayer->pos);

		UpdateBackground();			//背景

		UpdatePlayer();				//プレイヤー

		UpdateBullet();				//弾

		UpdateEnemy();				//敵

		UpdateExplosion();			//爆発

		UpdateScore();				//スコア更新

		UpdateItem();				//アイテム

		UpdateEffect();				//エフェクト

		UpdateParticle();			//パーティクル

		UpdateTimer();				//タイマー

		UpdatePlayerLifeBar();	//ライフバー

		UpdateBlock();				//　ブロック

		UpdateBoss();				// ボスの更新
	}
#endif
}
//===================
// ゲームの描画
//===================
void DrawGame(void)
{
	// カメラのセット
	SetCamera();

	// メッシュフィールドの描画
	DrawMeshField();
	
	// ブロックの描画
	DrawBlock();			

	if (g_bPause == true)
	{// ポーズ中
		// ポーズの描画
		DrawPause();		
	}

#if 0
	//背景
	DrawBackground();

	//ブロック
	DrawBlock();

	//アイテム
	DrawItem();

	//弾
	DrawBullet();

	// ボスの描画
	DrawBoss();

	//プレイヤー
	DrawPlayer();

	//敵
	DrawEnemy();

	//爆破
	DrawExplosion();

	//スコア
	//DrawScore();

	//エフェクト
	DrawEffect();

	//パーティクル
	DrawParticle();

	//タイマー
	DrawTimer();

	//ライフバー
	DrawPlayerLifeBar();

#endif
}
//=========================
// ポーズの有効無効設定
//=========================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//=========================
// ゲーム状態設定
//=========================
void SetGamestate(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}
//==========================
// 現在のゲーム状態を返す
//==========================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
