//===========================================
//
// �����蔻��֘A�̏��� [ collision.h ]
// Author : Asuma Nishio
//
//===========================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*************************************
// �C���N���[�h�t�@�C���錾
//*************************************
#include "main.h"
#include "block.h"
#include "player.h"

//*************************************
// �v���g�^�C�v�錾
//*************************************
void CollisionBlockToPlayer3D(PLAYER * pPlayer,D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, BlockModel* pBlockModel, BlockInfo* pInfoBlock); // �u���b�N�ƃv���C���[�̓����蔻��

#endif // !_COLLISION_H_
