//===========================================
//
// �����蔻��֘A�̏��� [ collision.h ]
// Author : Asuma Nishio
//
//===========================================

//*************************************
// �C���N���[�h�t�@�C���錾
//*************************************
#include "collision.h"
#include "meshfield.h"
#include "mouse.h"
#include "camera.h"

//*************************************
// �}�N����`
//*************************************
#define HALF (0.5f) // �����̃T�C�Y

//*************************************
//�O���[�o���ϐ��錾
//*************************************
D3DXVECTOR3 g_OutPos(0.0f, 0.0f, 0.0f);

//======================================================
// �u���b�N�ƃv���C���[�̓����蔻�菈��
//======================================================
void CollisionBlockToPlayer3D(PLAYER * pPlayer,D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize,BlockModel * pBlockModel,BlockInfo * pInfoBlock)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!pInfoBlock[nCnt].bUse)
		{//	 �g�p���Ă��Ȃ��Ƃ�
			continue;
		}

		// �^�C�v���擾
		int nType = pInfoBlock[nCnt].aBlock.nType;

		if (pInfoBlock[nCnt].aBlock.bCollision)
		{// ���肪�L���̎��̂�
			if (pPos->y < pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y) * (pInfoBlock[nCnt].aBlock.Scal.y) &&
				pPos->y + pSize->y > pInfoBlock[nCnt].aBlock.pos.y - (pBlockModel[nType].size.y) * pInfoBlock[nCnt].aBlock.Scal.y)
			{
				// ���E�̓����蔻��----------------------
				if (pPosOld->z - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
					pPos->z + pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z - ((pBlockModel[nType].size.z) * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
				{
					// ������߂荞��
					if (pPosOld->x + pSize->x * HALF < pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
						pPos->x + pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
					{
						pPos->x = pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) - pSize->x * HALF - 0.1f;

					}
					// �E����߂荞��
					else if (pPosOld->x - pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
						pPos->x - pSize->x * HALF < pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
					{
						pPos->x = pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) + pSize->x * HALF + 0.1f;
					}
				}

				// �O��̓����蔻��------------------------------
				if (pPosOld->x - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
				{
					// ��O����߂荞��
					if (pPosOld->z + pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
					{
						pPos->z = pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) - pSize->z * HALF - 0.3f;

					}
					// ������߂荞��
					else if (pPosOld->z - pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
						pPos->z - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
					{
						pPos->z = pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) + pSize->z * HALF + 0.3f;
					}
				}

				// �c�̓����蔻��--------------------------
				if (pPos->x - pSize->x * HALF < pInfoBlock[nCnt].aBlock.pos.x + (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > pInfoBlock[nCnt].aBlock.pos.x - (pBlockModel[nType].size.x * HALF) * (pInfoBlock[nCnt].aBlock.Scal.x))
				{
					if (pPosOld->z - pSize->z * HALF < pInfoBlock[nCnt].aBlock.pos.z + (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > pInfoBlock[nCnt].aBlock.pos.z - (pBlockModel[nType].size.z * HALF) * (pInfoBlock[nCnt].aBlock.Scal.z))
					{
						// �ォ��߂荞�񂾎�
						if (pPosOld->y >= pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y) &&
							pPos->y < pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y))
						{
							pPlayer->bLanding = true;	// �v���C���[�̃W�����v���\�ɂ���
							pPos->y = (pInfoBlock[nCnt].aBlock.pos.y + (pBlockModel[nType].size.y * HALF) * (pInfoBlock[nCnt].aBlock.Scal.y));
							pMove->y = 0.0f;

						}
						// ������߂荞�񂾎�
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
