//=======================================
//
// ツール画面処理 [ edit.cpp ]
// Author:Asuma Nishio
//
//=======================================

//*************************************
// インクルードファイル宣言
//*************************************
#include "edit.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include "mapedit.h"
#include "block.h"
#include "input.h"
// #include "player.h"
#include "shadow.h"
#include "fade.h"

//*************************************
// グローバル変数宣言
//*************************************
bool isEdit;
MODE g_GameMode = MODE_EDIT; // グローバル変数でモードを管理

//=====================================
// 初期化処理
//=====================================
void InitEdit()
{
	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// メッシュフィールドの初期化
	InitMeshField();

	// 影の初期化
	InitShadow();

	// マップエディターの初期化
	InitMapEdit();

	// ブロックの初期化
	InitBlock();

	// グローバル変数の初期化
	g_GameMode = MODE_EDIT; // 初期モード
}
//=====================================
// 終了処理
//=====================================
void UninitEdit()
{
	// カメラの終了
	UninitCamera();

	// メッシュフィールドの終了
	UninitMeshField();

	// エディターの終了
	UninitMapEdit();

	// 影の終了
	UninitShadow();

	// ブロックの終了
	UninitBlock();
}
//=====================================
// 更新処理
//=====================================
void UpdateEdit()
{
	// カメラの更新
	UpdateCamera();

	// 現在のモードに応じて更新処理を分岐
	if (g_GameMode == MODE_EDIT)
	{
		// エディターの更新
		UpdateMapEdit();
	}

	if (KeyboardTrigger(DIK_F1))
	{// F1キーを押した
		// ゲームに遷移
		SetFade(MODE_GAME);
	}
}
//=====================================
// 描画処理
//=====================================
void DrawEdit()
{
	// カメラをセット
	SetCamera();

	// メッシュフィールドの描画
	DrawMeshField();

	//----------------------------
	// デバッグフォント
	//----------------------------
	DrawEditkey();
	DebugEditModelInfo();
	DrawCameraPos();
	DrawNumBlock();

	if (g_GameMode == MODE_EDIT)
	{
		// エディターの描画
		DrawMapEdit();

		// 影の描画
		DrawShadow();

	}
}
//===============================
// 編集モード中にMODEを切り替える
//===============================
MODE GetNowMode()
{
	return g_GameMode;
}