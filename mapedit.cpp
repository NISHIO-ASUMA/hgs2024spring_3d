//===================================
//
// マップエディター処理 [mapedit.cpp]
// Author: Asuma Nishio
//
//===================================

//****************************
// インクルードファイル宣言
//****************************
#include "mapedit.h"
#include "camera.h"
#include "meshfield.h"
#include "light.h"
#include "input.h"
#include <stdio.h>
#include "block.h"
#include "mouse.h"
#include "collision.h"

//*************************************
// ファイル名列挙型宣言
//*************************************
typedef enum
{
	FILEPASS_0 = 0,	// 初期ファイルパス
	FILEPASS_1,		// 2個目
	FILEPASS_2,		// 3個目
	FILEPASS_MAX
}FILEPASS;

//*************************************
// 書き出すテキストファイルパスを設定
//*************************************
const char* TEXT_FILENAME[FILEPASS_MAX] =
{
	"data\\txt\\stage000.txt",	// 初期状態
	"data\\txt\\stage001.txt",	// 2番目
	"data\\txt\\stage002.txt",	// 3番目
};

//****************************
// グローバル変数宣言
//****************************
EDITMODEL g_ModelInfo[MAX_TEXTURE];	 // ブロックの情報を保存
MAPMODELINFO g_MapEdit[MAX_EDITOBJ]; // 配置時の情報
int g_nTypeNum;						 // ブロックの種類数を保存
int g_Edit;							 // 配置数をカウント
float g_fspeed;						 // 移動量
float g_jump;						 // ジャンプ
int g_CurrentFilepass;				 // ファイルパスのインデックス
int g_SelectFilePass;				 // テキストファイル用
bool isLoad;						 // ロードしたかどうか

//****************************
// プロトタイプ宣言
//****************************
void InitEditinfo(); // 構造体初期化情報
void NorRot();		 // 角度正規化関数

//============================
// マップエディター初期化処理
//============================
void InitMapEdit()
{
	// ブロック情報の読み込み
	LoadXFile();

	// 構造体情報
	InitEditinfo();

	// グローバル変数の初期化
	g_Edit = 0; // 配置数のカウント
	g_MapEdit[0].bUse = true;	// 使用状態
	g_CurrentFilepass = FILEPASS_0;	// 初期ファイルパス
	g_SelectFilePass = FILEPASS_0; // 初期ファイルパス
	isLoad = false;	// ロード状態
}
//============================
// マップエディター終了処理
//============================
void UninitMapEdit()
{
	// テクスチャの破棄
	for (int i = 0; i < MAX_TEXTURE; i++)	
	{
		for (int nCntMat = 0; nCntMat < (int)g_ModelInfo[i].dwNumMat; nCntMat++)
		{
			if (g_ModelInfo[i].pTexture[nCntMat] != NULL)
			{
				g_ModelInfo[i].pTexture[nCntMat]->Release();
				g_ModelInfo[i].pTexture[nCntMat] = NULL;
			}
		}

		// メッシュの破棄
		if (g_ModelInfo[i].pMesh != NULL)
		{
			g_ModelInfo[i].pMesh->Release();
			g_ModelInfo[i].pMesh = NULL;
		}

		// バッファの破棄
		if (g_ModelInfo[i].pBuffMat != NULL)
		{
			g_ModelInfo[i].pBuffMat->Release();
			g_ModelInfo[i].pBuffMat = NULL;
		}
	}
}
//============================
// マップエディター更新処理
//============================
void UpdateMapEdit()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// カメラの角度情報を代入
	D3DXVECTOR3 rot = GetCamera()->rot;

	// 角度計算用の変数
	float fAngle = D3DX_PI * 0.5f;

	// モードを取得
	MODE nMode = GetMode();

	D3DXVECTOR3 OutPos;

	if (nMode == MODE_EDIT)
	{// MODEが編集モードなら

		// 移動処理
		if (GetKeyboardPress(DIK_A))
		{// Aキーを押した

			g_MapEdit[g_Edit].mapedit.move.x -= sinf(rot.y + (fAngle)) * g_fspeed;
			g_MapEdit[g_Edit].mapedit.move.z -= cosf(rot.y + (fAngle)) * g_fspeed;
		}
		else if (GetKeyboardPress(DIK_D))
		{// Dキーを押した
			// Dキーのみ押した
			g_MapEdit[g_Edit].mapedit.move.x += sinf(rot.y + (fAngle)) * g_fspeed;
			g_MapEdit[g_Edit].mapedit.move.z += cosf(rot.y + (fAngle)) * g_fspeed;
		}
		else if (GetKeyboardPress(DIK_W))
		{// Wキーを押した

			g_MapEdit[g_Edit].mapedit.move.x += sinf(rot.y) * g_fspeed;
			g_MapEdit[g_Edit].mapedit.move.z += cosf(rot.y) * g_fspeed;
		}
		else if (GetKeyboardPress(DIK_S))
		{// Sキーを押した

			g_MapEdit[g_Edit].mapedit.move.x -= sinf(rot.y) * g_fspeed;
			g_MapEdit[g_Edit].mapedit.move.z -= cosf(rot.y) * g_fspeed;
		}

		// 角度正規化関数
		NorRot();

		if (KeyboardTrigger(DIK_UP))
		{// 上キーを押した
			if (g_MapEdit[g_Edit].mapedit.nType < g_nTypeNum - 1)
			{
				// 最大数より小さいとき
				g_MapEdit[g_Edit].mapedit.nType++;
			}
		}
		else if (KeyboardTrigger(DIK_DOWN))
		{// 下キーを押した
			if (g_MapEdit[g_Edit].mapedit.nType > 0)
			{
				// 1より大きいとき
				g_MapEdit[g_Edit].mapedit.nType--;
			}
		}

		if (KeyboardTrigger(DIK_T))
		{// Tキーを押したら
			// 座標を更新
			g_MapEdit[g_Edit].mapedit.pos.y += 10.0f;
		}
		else if (KeyboardTrigger(DIK_G))
		{// Gキーを押したら
			// 座標を更新
			g_MapEdit[g_Edit].mapedit.pos.y -= 10.0f;
		}

		if (KeyboardTrigger(DIK_H))
		{// Hキーが押された
			// 拡大率の変更
			g_MapEdit[g_Edit].mapedit.Scal.x -= 0.1f;

			if (g_MapEdit[g_Edit].mapedit.Scal.x <= 0.1f)
			{// 下限を設定
				g_MapEdit[g_Edit].mapedit.Scal.x = 0.1f;
			}

		}
		else if (KeyboardTrigger(DIK_Y))
		{// Yキーが押された
			// 拡大率の変更
			g_MapEdit[g_Edit].mapedit.Scal.x += 0.1f;

			if (g_MapEdit[g_Edit].mapedit.Scal.x >= 2.0f)
			{// 上限を設定
				g_MapEdit[g_Edit].mapedit.Scal.x = 2.0f;
			}
		}

		if (KeyboardTrigger(DIK_J))
		{// Jキーが押された
			// 拡大率の変更
			g_MapEdit[g_Edit].mapedit.Scal.y -= 0.1f;

			if (g_MapEdit[g_Edit].mapedit.Scal.y <= 0.1f)
			{// 下限を設定
				g_MapEdit[g_Edit].mapedit.Scal.y = 0.1f;
			}

		}
		else if (KeyboardTrigger(DIK_U))
		{// Uキーが押された
			// 拡大率の変更
			g_MapEdit[g_Edit].mapedit.Scal.y += 0.1f;

			if (g_MapEdit[g_Edit].mapedit.Scal.y >= 2.0f)
			{// 上限を設定
				g_MapEdit[g_Edit].mapedit.Scal.y = 2.0f;
			}

		}

		if (KeyboardTrigger(DIK_I))
		{// Iキーが押された
			// 拡大率の変更
			g_MapEdit[g_Edit].mapedit.Scal.z += 0.1f;

			if (g_MapEdit[g_Edit].mapedit.Scal.z >= 2.0f)
			{// 上限を設定
				g_MapEdit[g_Edit].mapedit.Scal.z = 2.0f;
			}

		}
		else if (KeyboardTrigger(DIK_K))
		{// Kキーが押された
			// 拡大率の変更
			g_MapEdit[g_Edit].mapedit.Scal.z -= 0.1f;

			if (g_MapEdit[g_Edit].mapedit.Scal.z <= 0.1f)
			{// 下限を設定
				g_MapEdit[g_Edit].mapedit.Scal.z = 0.1f;
			}
		}

		if (KeyboardTrigger(DIK_RETURN))
		{// ENTERキーが押された
			// 次の敵の位置を配置した敵と同じにする
			g_MapEdit[g_Edit + 1].mapedit.pos = g_MapEdit[g_Edit].mapedit.pos;

			// タイプを同じにする
			g_MapEdit[g_Edit + 1].mapedit.nType = g_MapEdit[g_Edit].mapedit.nType;

			// 使用判定にする
			g_MapEdit[g_Edit + 1].bUse = true;

			// インクリメント
			g_Edit++;
		}

		if (g_Edit > 0)
		{// 0以上なら
			if (KeyboardTrigger(DIK_BACKSPACE))
			{// BACKSPACEキーが押された

				// 未使用判定にする
				g_MapEdit[g_Edit].bUse = false;

				// デクリメント
				g_Edit--;
			}
		}

		//	書き出し処理
		if (KeyboardTrigger(DIK_F7))
		{// F7キーを押した
			// 保存する
			SavetxtFile();
		}

		if (KeyboardTrigger(DIK_F8))
		{// F8キーを押した
			// ファイルパスを切り替え
			g_CurrentFilepass = (g_CurrentFilepass + 1) % FILEPASS_MAX;
		}

		if (KeyboardTrigger(DIK_F9))
		{// F9キーを押した
			// 再読み込み
			ReloadTextEdit();
			//isLoad = true;
		}

		if (KeyboardTrigger(DIK_C))
		{// 当たり判定切り替え
			g_MapEdit[g_Edit].mapedit.bCollision = false;
		}
		else if (KeyboardTrigger(DIK_Z))
		{// 当たり判定切り替え
			g_MapEdit[g_Edit].mapedit.bCollision = true;
		}

		if (KeyboardTrigger(DIK_V))
		{
			// テキストファイルパスを切り替え
			g_SelectFilePass = (g_SelectFilePass + 1) % FILEPASS_MAX;
		}

		// 移動量の減衰
		g_MapEdit[g_Edit].mapedit.move.x += (0.0f - g_MapEdit[g_Edit].mapedit.move.x) * 0.5f;
		g_MapEdit[g_Edit].mapedit.move.z += (0.0f - g_MapEdit[g_Edit].mapedit.move.z) * 0.5f;

		// 移動量の更新
		g_MapEdit[g_Edit].mapedit.pos += g_MapEdit[g_Edit].mapedit.move;
	}
}
//============================
// マップエディター描画処理
//============================
void DrawMapEdit()
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 現在のモードを取得
	MODE nMode = GetMode();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	// 現在のマテリアルを保存
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	for (int nCnt = 0; nCnt < MAX_EDITOBJ; nCnt++)
	{
		// タイプを保存
		int nType = g_MapEdit[nCnt].mapedit.nType;

		if (!g_MapEdit[nCnt].bUse)
		{// プレイモード かつ isputblockが未使用 または bUseが未使用
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_MapEdit[nCnt].mapedit.mtxWorld);

		// 大きさを反映
		D3DXMatrixScaling(&mtxScal, g_MapEdit[nCnt].mapedit.Scal.x, g_MapEdit[nCnt].mapedit.Scal.y, g_MapEdit[nCnt].mapedit.Scal.z);
		D3DXMatrixMultiply(&g_MapEdit[nCnt].mapedit.mtxWorld, &g_MapEdit[nCnt].mapedit.mtxWorld, &mtxScal);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MapEdit[nCnt].mapedit.rot.y, g_MapEdit[nCnt].mapedit.rot.x, g_MapEdit[nCnt].mapedit.rot.z);
		D3DXMatrixMultiply(&g_MapEdit[nCnt].mapedit.mtxWorld, &g_MapEdit[nCnt].mapedit.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MapEdit[nCnt].mapedit.pos.x, g_MapEdit[nCnt].mapedit.pos.y, g_MapEdit[nCnt].mapedit.pos.z);
		D3DXMatrixMultiply(&g_MapEdit[nCnt].mapedit.mtxWorld, &g_MapEdit[nCnt].mapedit.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_MapEdit[nCnt].mapedit.mtxWorld);

		// 現在のマトリックスの取得
		pDevice->GetMaterial(&matDef);

		// マテリアル数だけ回す
		for (int nCntMat = 0; nCntMat < (int)g_ModelInfo[nType].dwNumMat; nCntMat++)
		{
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_ModelInfo[nType].pBuffMat->GetBufferPointer();

			//選択中のブロックだったら
			if (nCnt == g_Edit)
			{
				// カラー変更用変数
				D3DXMATERIAL color;

				// カラーを代入
				color = pMat[nCntMat];

				// 半透明にする
				color.MatD3D.Diffuse.r = 1.0f;
				color.MatD3D.Diffuse.g = 1.0f;
				color.MatD3D.Diffuse.b = 1.0f;
				color.MatD3D.Diffuse.a = 0.5f;

				//マテリアルの設定
				pDevice->SetMaterial(&color.MatD3D);
			}
			else
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}

			// テクスチャ設定
			pDevice->SetTexture(0, g_ModelInfo[nType].pTexture[nCntMat]);

			// モデルの描画
			g_ModelInfo[nType].pMesh->DrawSubset(nCntMat);
		}

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}
//===============================
//テキストファイルに書き出し
//===============================
bool SavetxtFile()
{
	// 返すフラグ用変数
	bool isWrite = true;

	// ファイルポインタを宣言
	FILE* pFile;

	// 任意のファイルを開く
	pFile = fopen(TEXT_FILENAME[g_SelectFilePass], "w");

	if (pFile != NULL)
	{// ファイルが開けたら
		// 書き出し開始

		int nCnt = 0; // カウント用変数

		// タイトル関係
		fprintf(pFile, "//**************************************************\n");
		fprintf(pFile, "//                 モデル配置情報                   \n");
		fprintf(pFile, "//**************************************************\n\n");

		// 配置数,種類数
		fprintf(pFile, "NUM_BLOCK =  %d   # 総配置数\n\n", ReturnEdit());

		while (nCnt < g_Edit)
		{// nCntが下の分だけ書き出す

			// ブロックごとの配置情報
			fprintf(pFile, "//-------------------------------\n");
			fprintf(pFile, "//   [ %d ] 番目のブロック\n", nCnt);
			fprintf(pFile, "//-------------------------------\n");
			fprintf(pFile, "START_BLOCKSET\n");
			fprintf(pFile, "POS  = %.2f %.2f %.2f # 座標\n", g_MapEdit[nCnt].mapedit.pos.x, g_MapEdit[nCnt].mapedit.pos.y, g_MapEdit[nCnt].mapedit.pos.z);
			fprintf(pFile, "SCAL = %.2f %.2f %.2f # 拡大率\n", g_MapEdit[nCnt].mapedit.Scal.x, g_MapEdit[nCnt].mapedit.Scal.y, g_MapEdit[nCnt].mapedit.Scal.z);
			fprintf(pFile, "TYPE = %d        # モデルの種類番号\n", g_MapEdit[nCnt].mapedit.nType);
			fprintf(pFile, "END_BLOCKSET\n\n");

			// カウントを加算する
			nCnt++;
		}

	}
	else
	{// 書き出し失敗
		// メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);
		return !isWrite;
	}

	// ファイルを閉じる
	fclose(pFile);

	// 結果を返す
	return isWrite;
}
//=================================
// テキストファイル再読み込み処理
//=================================
bool ReloadTextEdit(void)
{
	// 初期状態をtrueにする
	bool isReload = true;

	// ファイルポインタを宣言
	FILE* pFile;

	// 任意のファイルを開く
	pFile = fopen(TEXT_FILENAME[g_SelectFilePass], "r");

	if (pFile != NULL)
	{// ファイルが開けたら

		// 構造体を初期化
		InitEditinfo();

		// ローカル変数---------------------------------
		int nData = 0; // ファイル読み込みの返り値
		int nCnt = 0; // カウント用変数
		char aString[256] = {}; // 文字列を読み込む
		char aGomi[5] = {}; // =を格納する文字列
		//-----------------------------------------------
		
		while (1)
		{
			// nDataに代入する
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "NUM_BLOCK") == 0)
			{// NUM_BLOCKを読み取ったら
				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// ブロックの配置された数を読み込む
				fscanf(pFile, "%d", &g_Edit);
			}

			if (strcmp(aString, "START_BLOCKSET") == 0)
			{// START_BLOCKSETを読み取ったら

				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "POS") == 0)
					{// POSを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 座標を代入
						fscanf(pFile, "%f", &g_MapEdit[nCnt].mapedit.pos.x);
						fscanf(pFile, "%f", &g_MapEdit[nCnt].mapedit.pos.y);
						fscanf(pFile, "%f", &g_MapEdit[nCnt].mapedit.pos.z);

					}

					if (strcmp(aString, "SCAL") == 0)
					{// SCALを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 拡大率を代入
						fscanf(pFile, "%f", &g_MapEdit[nCnt].mapedit.Scal.x);
						fscanf(pFile, "%f", &g_MapEdit[nCnt].mapedit.Scal.y);
						fscanf(pFile, "%f", &g_MapEdit[nCnt].mapedit.Scal.z);

					}

					if (strcmp(aString, "TYPE") == 0)
					{// TYPEを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 現在のブロックの種類番号を代入
						fscanf(pFile, "%d", &g_MapEdit[nCnt].mapedit.nType);
					}

					if (strcmp(aString, "END_BLOCKSET") == 0)
					{// END_BLOCKSETを読み取ったら
						// 使用判定にする
						g_MapEdit[nCnt].bUse = true;

						// インクリメントして次のブロック情報へ
						nCnt++;

						break;
					}
				}
			}

			if (nData == EOF)
			{// EOFを読み取ったら
				// 判定を初期状態に戻す
				g_MapEdit[g_Edit].bUse = true;

				break;
			}
		}
	}
	else 
	{
		// メッセージBOXの表示
		MessageBox(NULL, "再読み込み失敗(ReloadTextFile)", "エラー", MB_OK);
		pFile = NULL;
		return !isReload;
	}

	// ファイルを閉じる
	fclose(pFile);

	// 結果を返す
	return isReload;
}
//===============================
// Xファイルの読み込み処理
//===============================
void LoadXFile()
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 変数の初期化
	g_nTypeNum = 0; // 種類数
	g_fspeed = 0.0f;// 移動量
	g_jump = 0.0f;	// ジャンプ量

	// ファイルポインタを宣言
	FILE* pFile;

	// 開くファイル名
	pFile = fopen("data\\model.txt", "r");

	if (pFile != NULL)
	{// 開けたら

		// ローカル変数-------------------------------
		char aString[256] = {};	// 文字列
		char aGomi[3] = {};		// イコールを代入する
		int nCnt = 0;
		int nData = 0;
		//---------------------------------------------

		while (1)
		{
			// nDataに代入する
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0],"BLOCKTYPE") == 0)
			{// BLOCKTYPEを読み取った
				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				//種類の数を読み込む
				fscanf(pFile, "%d", &g_nTypeNum);

			}

			else if (strcmp(&aString[0], "MOVE") == 0)
			{// MOVEを読む

				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// 種類の数を読み込む
				fscanf(pFile, "%f", &g_fspeed);
			}

			else if (strcmp(&aString[0], "JUMP") == 0)
			{// JUMPを読む

				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// ジャンプ量の数を読み込む
				fscanf(pFile, "%f", &g_jump);
			}

			else if (strcmp(&aString[0],"MODEL_FILENAME") == 0)
			{// MODEL_FILENAMEを読み取ったら

				// =を飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// 文字列を読み込む
				fscanf(pFile, "%s", &aString[0]);

				// モデルファイル名格納用の変数
				char* filename[256] = {};	

				// 読み込んだ文字列を保存する
				filename[0] = aString;

				// ファイル名をコピー
				strcpy(&g_ModelInfo[nCnt].FileName[0], filename[0]);

				// Xファイルの読み込み
				D3DXLoadMeshFromX(filename[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_ModelInfo[nCnt].pBuffMat,
					NULL,
					&g_ModelInfo[nCnt].dwNumMat,
					&g_ModelInfo[nCnt].pMesh);
				
				// マテリアルデータへのポインタ
				D3DXMATERIAL* pMat;

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_ModelInfo[nCnt].pBuffMat->GetBufferPointer();

				// マテリアル数だけ回す
				for (int nCntMat = 0; nCntMat < (int)g_ModelInfo[nCnt].dwNumMat; nCntMat++)
				{
					if (pMat[nCntMat].pTextureFilename != NULL)
					{
						// テクスチャの読み込み
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntMat].pTextureFilename,
							&g_ModelInfo[nCnt].pTexture[nCntMat]);
					}
				}

				// ローカル変数宣言-----------------------------
				int nNumVtx;		// 頂点数
				DWORD sizeFVF;		// 頂点フォーマットのサイズ
				BYTE* pVtxBuff;		// 頂点バッファのポインタ
				//----------------------------------------------

				// 頂点数の取得
				nNumVtx = g_ModelInfo[nCnt].pMesh->GetNumVertices();

				// 頂点のサイズを取得
				sizeFVF = D3DXGetFVFVertexSize(g_ModelInfo[nCnt].pMesh->GetFVF());

				// 頂点バッファをロック
				g_ModelInfo[nCnt].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				// 頂点数分回す
				for (int nCntEnemy = 0; nCntEnemy < nNumVtx; nCntEnemy++)
				{
					// 頂点座標の代入
					D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

					// 頂点座標の比較
					if (Vtx.x > g_ModelInfo[nCnt].Vtxmax.x)
					{// xが最大値より大きかったら
						// 代入
						g_ModelInfo[nCnt].Vtxmax.x = Vtx.x;
					}
					if (Vtx.y > g_ModelInfo[nCnt].Vtxmax.y)
					{// yが最大値より大きかったら
						// 代入
						g_ModelInfo[nCnt].Vtxmax.y = Vtx.y;
					}
					if (Vtx.z > g_ModelInfo[nCnt].Vtxmax.z)
					{// zが最大値より大きかったら
						// 代入
						g_ModelInfo[nCnt].Vtxmax.z = Vtx.z;
					}

					if (Vtx.x < g_ModelInfo[nCnt].Vtxmin.x)
					{// xが最小値より小さかったら
						// 代入
						g_ModelInfo[nCnt].Vtxmin.x = Vtx.x;
					}
					if (Vtx.y < g_ModelInfo[nCnt].Vtxmin.y)
					{// yが最小値より小さかったら
						// 代入
						g_ModelInfo[nCnt].Vtxmin.y = Vtx.y;
					}
					if (Vtx.z < g_ModelInfo[nCnt].Vtxmin.z)
					{// zが最小値より小さかったら
						// 代入
						g_ModelInfo[nCnt].Vtxmin.z = Vtx.z;
					}

					// 頂点フォーマットのサイズ分進める
					pVtxBuff += sizeFVF;
				}

				// サイズを代入
				g_ModelInfo[nCnt].size.x = g_ModelInfo[nCnt].Vtxmax.x - g_ModelInfo[nCnt].Vtxmin.x;
				g_ModelInfo[nCnt].size.y = g_ModelInfo[nCnt].Vtxmax.y - g_ModelInfo[nCnt].Vtxmin.y;
				g_ModelInfo[nCnt].size.z = g_ModelInfo[nCnt].Vtxmax.z - g_ModelInfo[nCnt].Vtxmin.z;

				// アンロック
				g_ModelInfo[nCnt].pMesh->UnlockVertexBuffer();

				// nCntをインクリメント
				nCnt++;

			}

			if (nData == EOF)
			{// EOFを読み取る
				break;
			}
		}
	}
	else
	{// 開けなかったら
		return;
	}

	// ファイルを閉じる
	fclose(pFile);

}
//===============================
// エディターモデル配置時の情報
//===============================
MAPMODELINFO* MapInfo()
{
	return &g_MapEdit[0];
}
//===============================
// エディターモデル情報の取得
//===============================
EDITMODEL* GetBlockInfo(int type)
{
	return &g_ModelInfo[type];
}
//===============================
// エディター情報の取得
//===============================
MapEdit* getMap()
{
	return &g_MapEdit[0].mapedit;
}
//================================
// 配置数情報の取得
//================================
int ReturnEdit()
{
	return g_Edit;
}
//================================
// 配置時移動量の取得
//================================
float ReturnSpeed()
{
	return g_fspeed;
}
//================================
// 設定した最大種類数の取得
//================================
int ReturnType()
{
	return g_nTypeNum;
}
//================================
// 設定したジャンプ量の取得
//================================
float ReturnJump()
{
	return g_jump;
}
//======================================
// バイナリファイルパスナンバーを取得
//======================================
int Filepass()
{
	return g_CurrentFilepass;
}
//=====================================
// テキストファイルパスナンバーを取得
//=====================================
int TextFilepass()
{
	return g_SelectFilePass;
}
//================================
// 判定を取得
//================================
bool IsGetLoad()
{
	return isLoad;
}
//================================
// 構造体初期化処理
//================================
void InitEditinfo()
{
	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_EDITOBJ; nCnt++)
	{
		g_MapEdit[nCnt].mapedit.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
		g_MapEdit[nCnt].mapedit.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_MapEdit[nCnt].bUse = false;						          // 未使用状態
		g_MapEdit[nCnt].mapedit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 角度
		g_MapEdit[nCnt].mapedit.nType = 0;							  // 種類
		g_MapEdit[nCnt].mapedit.Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f); // 拡大率
		g_MapEdit[nCnt].mapedit.bCollision = true;					  // 初期状態を有効判定にする
	}
}
//================================
// 角度正規化関数
//================================
void NorRot()
{
		// 角度の正規化
	if (g_MapEdit[g_Edit].mapedit.rot.y > D3DX_PI)
	{// D3DX_PIより大きくなったら
		g_MapEdit[g_Edit].mapedit.rot.y -= D3DX_PI * 2.0f;
	}
	// 角度を正規化
	if (g_MapEdit[g_Edit].mapedit.rot.y < -D3DX_PI)
	{// D3DX_PIより小さくなったら
		g_MapEdit[g_Edit].mapedit.rot.y += D3DX_PI * 2.0f;
	}
}
