//===========================================
//
// 当たり判定関連の処理 [ collision.h ]
// Author : Asuma Nishio
//
//===========================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*************************************
// インクルードファイル宣言
//*************************************
#include "main.h"
#include "block.h"
#include "player.h"

//*************************************
// プロトタイプ宣言
//*************************************
void CollisionBlockToPlayer3D(PLAYER * pPlayer,D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, BlockModel* pBlockModel, BlockInfo* pInfoBlock); // ブロックとプレイヤーの当たり判定

#endif // !_COLLISION_H_
