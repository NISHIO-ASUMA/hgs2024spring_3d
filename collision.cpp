//===========================================
//
// 当たり判定関連の処理 [ collision.h ]
// Author : Asuma Nishio
//
//===========================================

//*************************************
// インクルードファイル宣言
//*************************************
#include "collision.h"
#include "meshfield.h"
#include "mouse.h"
#include "camera.h"

//*************************************
// マクロ定義
//*************************************
#define HALF (0.5f) // 半分のサイズ

//*************************************
//グローバル変数宣言
//*************************************
D3DXVECTOR3 g_OutPos(0.0f, 0.0f, 0.0f);

//======================================================
// ブロックとプレイヤーの当たり判定処理
//======================================================
void CollisionBlockToPlayer3D(PLAYER * pPlayer,D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize,BlockModel * pBlockModel,BlockInfo * pInfoBlock)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!pInfoBlock[nCnt].bUse)
		{//	 使用していないとき
			continue;
		}

		// タイプを取得
		int nType = pInfoBlock[nCnt].aBlock.nType;

		if (pInfoBlock[nCnt].aBlock.bCollision)
		{// 判定が有効の時のみ
			if (pPos->y < pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y) * (pInfoBlock[nCnt].aBlock.Scal.y) &&
				pPos->y + pSize->y > pInfoBlock[nCnt].aBlock.pos.y - (pBlockModel[nType].size.y) * pInfoBlock[nCnt].aBlock.Scal.y)
			{
				// 左右の当たり判定----------------------
				if (pPosOld->z - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
					pPos->z + pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z - ((pBlockModel[nType].size.z) * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
				{
					// 左からめり込む
					if (pPosOld->x + pSize->x * HALF < pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
						pPos->x + pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
					{
						pPos->x = pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) - pSize->x * HALF - 0.1f;

					}
					// 右からめり込む
					else if (pPosOld->x - pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
						pPos->x - pSize->x * HALF < pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
					{
						pPos->x = pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) + pSize->x * HALF + 0.1f;
					}
				}

				// 前後の当たり判定------------------------------
				if (pPosOld->x - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
				{
					// 手前からめり込む
					if (pPosOld->z + pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
					{
						pPos->z = pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) - pSize->z * HALF - 0.3f;

					}
					// 奥からめり込む
					else if (pPosOld->z - pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
						pPos->z - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
					{
						pPos->z = pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) + pSize->z * HALF + 0.3f;
					}
				}

				// 縦の当たり判定--------------------------
				if (pPos->x - pSize->x * HALF < pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
				{
					if (pPosOld->z - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
					{
						// 上からめり込んだ時
						if (pPosOld->y >= pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y) &&
							pPos->y < pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y))
						{
							pPlayer->bLanding = true;	// プレイヤーのジャンプを可能にする
							pPos->y = (pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y));
							pMove->y = 0.0f;

						}
						// 下からめり込んだ時
						else if (pPosOld->y + pSize->y <= pInfoBlock[nCnt].aBlock.pos.y - (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y) &&
							pPos->y + pSize->y > pInfoBlock[nCnt].aBlock.pos.y - (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y))
						{
							pPos->y = (pInfoBlock[nCnt].aBlock.pos.y - (pBlockModel[nType].size.y) * (pInfoBlock[nCnt].aBlock.Scal.y)) - pSize->y * HALF;
							pMove->y = 0.0f;
						}
					}
				}
			}
		}
	}
}
