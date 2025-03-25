//=============================
//
// ブロック処理 [block.cpp]
// Author: Asuma Nishio
// 
//=============================

//****************************
// インクルードファイル宣言
//****************************
#include "input.h"
#include "block.h"
#include <stdio.h>
#include "shadow.h"
#include "mapedit.h"
#include "meshfield.h"
#include "collision.h"

//*****************************
// マクロ定義
//*****************************
#define HALF (0.5f)				// 半分の値

//*************************************
// ファイル名列挙型宣言
//*************************************
typedef enum
{
	BLOCKPASS_0 = 0, // 初期ファイルパス
	BLOCKPASS_1,	 // 2番目
	BLOCKPASS_2,	 // 3番目
	BLOCKPASS_MAX
}BLOCKPASS;

//*************************************
// テキストファイルパスを設定
//*************************************
const char* BLOCK_TEXTFILENAME[BLOCKPASS_MAX] =
{
	"data\\txt\\stage000.txt",	// 初期状態
	"data\\txt\\stage001.txt",	// 2番目
	"data\\txt\\stage002.txt",	// 3番目
};

//*****************************
// グローバル変数宣言
//*****************************
BlockInfo g_Block[MAX_BLOCK];		// 構造体変数
BlockModel g_pBlock[MAX_TEXTURE];   // ブロック情報を持つ配列
int g_BlockPass;					// バイナリパスを保存する変数
int g_textPass;						//  テキストファイルパスを保存する変数
bool isFrame;						// 当たり判定の表示フレーム

//*****************************
// プロトタイプ宣言
//*****************************
void DrawCollisionBox(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& scale, const D3DXVECTOR3& rot, D3DXMATRIX& mtxWorld);

//=========================
//　ブロック初期化処理
//=========================
void InitBlock(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_Block[nCnt].bUse = false;		// 未使用判定
		g_Block[nCnt].aBlock.nType = 0; // 種類
		g_Block[nCnt].aBlock.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 座標
		g_Block[nCnt].aBlock.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 角度
		g_Block[nCnt].aBlock.Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);  // 拡大率
		g_Block[nCnt].aBlock.bCollision = true; // 初期状態は判定を有効に
	}

	// グローバル変数の初期化
	g_BlockPass = 0;
	g_textPass = 0;
	isFrame = false;
}
//=========================
//　ブロック終了処理
//=========================
void UninitBlock(void)
{
	// 無し
}
//=========================
//　ブロック更新処理
//=========================
void UpdateBlock(void)
{
	if (KeyboardTrigger(DIK_F6))
	{// キーを押した
		// 判定を有効
		isFrame = true;
	}
	else if (KeyboardTrigger(DIK_F5))
	{
		// 判定を無効
		isFrame = false;
	}

}
//=========================
//　ブロック描画処理
//=========================
void DrawBlock(void)
{
#if 1
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	// 現在のマテリアルを保存
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!g_Block[nCnt].bUse)
		{// 未使用状態なら
			continue;
		}

		// タイプを保存
		int nType = g_Block[nCnt].aBlock.nType;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Block[nCnt].aBlock.mtxWorld);

		// 大きさを反映
		D3DXMatrixScaling(&mtxScal, g_Block[nCnt].aBlock.Scal.x, g_Block[nCnt].aBlock.Scal.y, g_Block[nCnt].aBlock.Scal.z);
		D3DXMatrixMultiply(&g_Block[nCnt].aBlock.mtxWorld, &g_Block[nCnt].aBlock.mtxWorld, &mtxScal);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCnt].aBlock.rot.y, g_Block[nCnt].aBlock.rot.x, g_Block[nCnt].aBlock.rot.z);
		D3DXMatrixMultiply(&g_Block[nCnt].aBlock.mtxWorld, &g_Block[nCnt].aBlock.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Block[nCnt].aBlock.pos.x, g_Block[nCnt].aBlock.pos.y, g_Block[nCnt].aBlock.pos.z);
		D3DXMatrixMultiply(&g_Block[nCnt].aBlock.mtxWorld, &g_Block[nCnt].aBlock.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCnt].aBlock.mtxWorld);

		// 現在のマトリックスの取得
		pDevice->GetMaterial(&matDef);
		
		// マテリアル数だけ回す
		for (int nCntMat = 0; nCntMat < (int)GetBlockInfo(nType)->dwNumMat; nCntMat++)
		{
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)GetBlockInfo(nType)->pBuffMat->GetBufferPointer();

			// マテリアル設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			
			// テクスチャ設定
			pDevice->SetTexture(0, GetBlockInfo(nType)->pTexture[nCntMat]);

			// ブロック(パーツ)の描画
			GetBlockInfo(nType)->pMesh->DrawSubset(nCntMat);
		}
		
		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);

		if (isFrame)
		{
			// 当たり判定のバウンディングボックス表示
			DrawCollisionBox(g_Block[nCnt].aBlock.pos, g_pBlock[nType].size, g_Block[nCnt].aBlock.Scal, g_Block[nCnt].aBlock.rot, g_Block[nCnt].aBlock.mtxWorld);
		}

	}
#endif
}
#if 1
//=============================
// ブロックの設定処理
//=============================
void SetBlock(D3DXVECTOR3 pos,int nType,D3DXVECTOR3 Scal)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!g_Block[nCnt].bUse)
		{
			// 未使用なら
			g_Block[nCnt].aBlock.pos = pos;		// 座標
			g_Block[nCnt].aBlock.nType = nType; // 種類
			g_Block[nCnt].aBlock.Scal = Scal;	// 拡大率
			g_Block[nCnt].bUse = true;			// 使用判定

			// サイズを代入する
			g_pBlock[nCnt].size.x = GetBlockInfo(nType)->Vtxmax.x - GetBlockInfo(nType)->Vtxmin.x;	// sizeのx
			g_pBlock[nCnt].size.y = GetBlockInfo(nType)->Vtxmax.y - GetBlockInfo(nType)->Vtxmin.y;	// sizeのy
			g_pBlock[nCnt].size.z = GetBlockInfo(nType)->Vtxmax.z - GetBlockInfo(nType)->Vtxmin.z;	// sizeのz

			break;
		}
	}
}
//=============================
// ブロックの当たり判定処理
//=============================
void CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!g_Block[nCnt].bUse)
		{//	 使用していないとき
			continue;
		}

		// プレイヤーの取得
		// PLAYER* pPlayer = GetPlayer();

		// タイプを取得
		int nType = g_Block[nCnt].aBlock.nType;

		if (g_Block[nCnt].aBlock.bCollision)
		{// 判定が有効の時のみ
			if (pPos->y < g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y) * (g_Block[nCnt].aBlock.Scal.y) &&
				pPos->y + pSize->y > g_Block[nCnt].aBlock.pos.y - (g_pBlock[nType].size.y) * g_Block[nCnt].aBlock.Scal.y)
			{
				// 左右の当たり判定----------------------
				if (pPosOld->z - pSize->z * HALF < g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
					pPos->z + pSize->z * HALF > g_Block[nCnt].aBlock.pos.z - ((g_pBlock[nType].size.z) * HALF) * (g_Block[nCnt].aBlock.Scal.z))
				{
					// 左からめり込む
					if (pPosOld->x + pSize->x * HALF < g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
						pPos->x + pSize->x * HALF > g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
					{
						pPos->x = g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) - pSize->x * HALF - 0.1f;

					}
					// 右からめり込む
					else if (pPosOld->x - pSize->x * HALF > g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
						pPos->x - pSize->x * HALF < g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
					{
						pPos->x = g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) + pSize->x * HALF + 0.1f;
					}
				}

				// 前後の当たり判定------------------------------
				if (pPosOld->x - pSize->z * HALF < g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
				{
					// 手前からめり込む
					if (pPosOld->z + pSize->z * HALF < g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z))
					{
						pPos->z = g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) - pSize->z * HALF - 0.3f;

					}
					// 奥からめり込む
					else if (pPosOld->z - pSize->z * HALF > g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
						pPos->z - pSize->z * HALF < g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z))
					{
						pPos->z = g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) + pSize->z * HALF + 0.3f;

					}
				}

				// 縦の当たり判定--------------------------
				if (pPos->x - pSize->x * HALF < g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
				{
					if (pPosOld->z - pSize->z * HALF < g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z))
					{
						// 上からめり込んだ時
						if (pPosOld->y >= g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y) &&
							pPos->y < g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y))
						{
							// pPlayer->bLanding = true;	// ジャンプ
							pPos->y = (g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y));
							pMove->y = 0.0f;

						}
						// 下からめり込んだ時
						else if (pPosOld->y + pSize->y <= g_Block[nCnt].aBlock.pos.y - (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y) &&
							pPos->y + pSize->y > g_Block[nCnt].aBlock.pos.y - (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y))
						{
							pPos->y = (g_Block[nCnt].aBlock.pos.y - (g_pBlock[nType].size.y) * (g_Block[nCnt].aBlock.Scal.y)) - pSize->y * HALF;
							pMove->y = 0.0f;
						}
					}
				}
			}
		}
	}
}
//=============================
//　ブロックの取得処理
//=============================
BlockInfo * GetBlock()
{
	return &g_Block[0];
}
//=============================
//　ブロックのモデル情報の取得
//=============================
BlockModel* GetInfoModel()
{
	return &g_pBlock[0];
}
//==========================================
//　テキストファイルブロックの読み込み処理
//==========================================
void LoadtxtBlock()
{
	// ファイルポインタを宣言
	FILE* pFile;

	// ファイルパスを代入
	g_textPass = TextFilepass();

	// ファイル開く
	pFile = fopen(BLOCK_TEXTFILENAME[g_textPass], "r");

	// ブロックの初期化処理
	InitBlock();

	if (pFile != NULL)
	{// ファイルが開けたら
		// 各変数に代入する
		char aSt[256] = {};
		char aGomi[5] = {};
		int nData = 0;
		int nBlock = 0;
		int nType = 0;
		int nCnt = 0;

		D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f); // 座標
		D3DXVECTOR3 Scal(1.0f, 1.0f, 1.0f); // 拡大率
		int nNumType = 0; // 種類番号

		while (1)
		{
			// nDataに代入する
			nData = fscanf(pFile, "%s", &aSt[0]);

			if (strcmp(aSt, "NUM_BLOCK") == 0)
			{// NUM_BLOCKを読み取ったら
				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// ブロックの数を読み込む
				fscanf(pFile, "%d", &nBlock);

			}

			if (strcmp(aSt, "START_BLOCKSET") == 0)
			{// START_BLOCKSETを読み取ったら

				while (nCnt < nBlock)
				{
					fscanf(pFile, "%s", &aSt[0]);

					if (strcmp(aSt, "POS") == 0)
					{// POSを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 座標を代入
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);

					}

					if (strcmp(aSt, "SCAL") == 0)
					{// SCALを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 拡大率を代入
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);

					}

					if (strcmp(aSt, "TYPE") == 0)
					{// TYPEを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 現在のブロックの種類番号を代入
						fscanf(pFile, "%d", &nNumType);
					}

					if (strcmp(aSt, "END_BLOCKSET") == 0)
					{// END_BLOCKSETを読み取ったら
						// 情報を代入
						SetBlock(pos, nNumType, Scal);

						// インクリメントして次のブロック情報へ
						nCnt++;
					}
				}
				
			}

			if (nData == EOF)
			{// EOFを読み取ったら
				// whike文を抜ける
				break;
			}
		}
	}
	else
	{// ファイルが開けなかったとき
		// メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);
		pFile = NULL;
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//=============================
//　ブロックの判定表示
//=============================
void DrawCollisionBox(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& scale,const D3DXVECTOR3& rot,D3DXMATRIX& mtxWorld)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックス
	D3DXMATRIX mtxTrans, mtxScal,mtxRot;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, size.x * scale.x, size.y * scale.y, size.z * scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScal);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ワイヤーフレームモードに設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 色を設定
	D3DMATERIAL9 mat;

	// ゼロメモリー関数
	ZeroMemory(&mat, sizeof(D3DMATERIAL9));

	// マテリアル設定
	mat.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
	mat.Ambient = mat.Diffuse;

	// マテリアルを戻す
	pDevice->SetMaterial(&mat);

	// 仮想メッシュ作成
	LPD3DXMESH pMesh = NULL;

	// ボックス作成
	D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &pMesh, NULL);

	// メッシュの破棄
	if (pMesh)
	{
		// セット
		pMesh->DrawSubset(0);

		pMesh->Release();  // メモリ解放
	}

	// 元の描画モードに戻す
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
#endif
