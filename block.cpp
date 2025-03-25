//=============================
//
// �u���b�N���� [block.cpp]
// Author: Asuma Nishio
// 
//=============================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "input.h"
#include "block.h"
#include <stdio.h>
#include "shadow.h"
#include "mapedit.h"
#include "meshfield.h"
#include "collision.h"

//*****************************
// �}�N����`
//*****************************
#define HALF (0.5f)				// �����̒l

//*************************************
// �t�@�C�����񋓌^�錾
//*************************************
typedef enum
{
	BLOCKPASS_0 = 0, // �����t�@�C���p�X
	BLOCKPASS_1,	 // 2�Ԗ�
	BLOCKPASS_2,	 // 3�Ԗ�
	BLOCKPASS_MAX
}BLOCKPASS;

//*************************************
// �e�L�X�g�t�@�C���p�X��ݒ�
//*************************************
const char* BLOCK_TEXTFILENAME[BLOCKPASS_MAX] =
{
	"data\\txt\\stage000.txt",	// �������
	"data\\txt\\stage001.txt",	// 2�Ԗ�
	"data\\txt\\stage002.txt",	// 3�Ԗ�
};

//*****************************
// �O���[�o���ϐ��錾
//*****************************
BlockInfo g_Block[MAX_BLOCK];		// �\���̕ϐ�
BlockModel g_pBlock[MAX_TEXTURE];   // �u���b�N�������z��
int g_BlockPass;					// �o�C�i���p�X��ۑ�����ϐ�
int g_textPass;						//  �e�L�X�g�t�@�C���p�X��ۑ�����ϐ�
bool isFrame;						// �����蔻��̕\���t���[��

//*****************************
// �v���g�^�C�v�錾
//*****************************
void DrawCollisionBox(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& scale, const D3DXVECTOR3& rot, D3DXMATRIX& mtxWorld);

//=========================
//�@�u���b�N����������
//=========================
void InitBlock(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_Block[nCnt].bUse = false;		// ���g�p����
		g_Block[nCnt].aBlock.nType = 0; // ���
		g_Block[nCnt].aBlock.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W
		g_Block[nCnt].aBlock.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �p�x
		g_Block[nCnt].aBlock.Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);  // �g�嗦
		g_Block[nCnt].aBlock.bCollision = true; // ������Ԃ͔����L����
	}

	// �O���[�o���ϐ��̏�����
	g_BlockPass = 0;
	g_textPass = 0;
	isFrame = false;
}
//=========================
//�@�u���b�N�I������
//=========================
void UninitBlock(void)
{
	// ����
}
//=========================
//�@�u���b�N�X�V����
//=========================
void UpdateBlock(void)
{
	if (KeyboardTrigger(DIK_F6))
	{// �L�[��������
		// �����L��
		isFrame = true;
	}
	else if (KeyboardTrigger(DIK_F5))
	{
		// ����𖳌�
		isFrame = false;
	}

}
//=========================
//�@�u���b�N�`�揈��
//=========================
void DrawBlock(void)
{
#if 1
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	// ���݂̃}�e���A����ۑ�
	D3DMATERIAL9 matDef;

	// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL* pMat;

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!g_Block[nCnt].bUse)
		{// ���g�p��ԂȂ�
			continue;
		}

		// �^�C�v��ۑ�
		int nType = g_Block[nCnt].aBlock.nType;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Block[nCnt].aBlock.mtxWorld);

		// �傫���𔽉f
		D3DXMatrixScaling(&mtxScal, g_Block[nCnt].aBlock.Scal.x, g_Block[nCnt].aBlock.Scal.y, g_Block[nCnt].aBlock.Scal.z);
		D3DXMatrixMultiply(&g_Block[nCnt].aBlock.mtxWorld, &g_Block[nCnt].aBlock.mtxWorld, &mtxScal);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCnt].aBlock.rot.y, g_Block[nCnt].aBlock.rot.x, g_Block[nCnt].aBlock.rot.z);
		D3DXMatrixMultiply(&g_Block[nCnt].aBlock.mtxWorld, &g_Block[nCnt].aBlock.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Block[nCnt].aBlock.pos.x, g_Block[nCnt].aBlock.pos.y, g_Block[nCnt].aBlock.pos.z);
		D3DXMatrixMultiply(&g_Block[nCnt].aBlock.mtxWorld, &g_Block[nCnt].aBlock.mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCnt].aBlock.mtxWorld);

		// ���݂̃}�g���b�N�X�̎擾
		pDevice->GetMaterial(&matDef);
		
		// �}�e���A����������
		for (int nCntMat = 0; nCntMat < (int)GetBlockInfo(nType)->dwNumMat; nCntMat++)
		{
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)GetBlockInfo(nType)->pBuffMat->GetBufferPointer();

			// �}�e���A���ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, GetBlockInfo(nType)->pTexture[nCntMat]);

			// �u���b�N(�p�[�c)�̕`��
			GetBlockInfo(nType)->pMesh->DrawSubset(nCntMat);
		}
		
		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		if (isFrame)
		{
			// �����蔻��̃o�E���f�B���O�{�b�N�X�\��
			DrawCollisionBox(g_Block[nCnt].aBlock.pos, g_pBlock[nType].size, g_Block[nCnt].aBlock.Scal, g_Block[nCnt].aBlock.rot, g_Block[nCnt].aBlock.mtxWorld);
		}

	}
#endif
}
#if 1
//=============================
// �u���b�N�̐ݒ菈��
//=============================
void SetBlock(D3DXVECTOR3 pos,int nType,D3DXVECTOR3 Scal)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!g_Block[nCnt].bUse)
		{
			// ���g�p�Ȃ�
			g_Block[nCnt].aBlock.pos = pos;		// ���W
			g_Block[nCnt].aBlock.nType = nType; // ���
			g_Block[nCnt].aBlock.Scal = Scal;	// �g�嗦
			g_Block[nCnt].bUse = true;			// �g�p����

			// �T�C�Y��������
			g_pBlock[nCnt].size.x = GetBlockInfo(nType)->Vtxmax.x - GetBlockInfo(nType)->Vtxmin.x;	// size��x
			g_pBlock[nCnt].size.y = GetBlockInfo(nType)->Vtxmax.y - GetBlockInfo(nType)->Vtxmin.y;	// size��y
			g_pBlock[nCnt].size.z = GetBlockInfo(nType)->Vtxmax.z - GetBlockInfo(nType)->Vtxmin.z;	// size��z

			break;
		}
	}
}
//=============================
// �u���b�N�̓����蔻�菈��
//=============================
void CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (!g_Block[nCnt].bUse)
		{//	 �g�p���Ă��Ȃ��Ƃ�
			continue;
		}

		// �v���C���[�̎擾
		// PLAYER* pPlayer = GetPlayer();

		// �^�C�v���擾
		int nType = g_Block[nCnt].aBlock.nType;

		if (g_Block[nCnt].aBlock.bCollision)
		{// ���肪�L���̎��̂�
			if (pPos->y < g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y) * (g_Block[nCnt].aBlock.Scal.y) &&
				pPos->y + pSize->y > g_Block[nCnt].aBlock.pos.y - (g_pBlock[nType].size.y) * g_Block[nCnt].aBlock.Scal.y)
			{
				// ���E�̓����蔻��----------------------
				if (pPosOld->z - pSize->z * HALF < g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
					pPos->z + pSize->z * HALF > g_Block[nCnt].aBlock.pos.z - ((g_pBlock[nType].size.z) * HALF) * (g_Block[nCnt].aBlock.Scal.z))
				{
					// ������߂荞��
					if (pPosOld->x + pSize->x * HALF < g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
						pPos->x + pSize->x * HALF > g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
					{
						pPos->x = g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) - pSize->x * HALF - 0.1f;

					}
					// �E����߂荞��
					else if (pPosOld->x - pSize->x * HALF > g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
						pPos->x - pSize->x * HALF < g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
					{
						pPos->x = g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) + pSize->x * HALF + 0.1f;
					}
				}

				// �O��̓����蔻��------------------------------
				if (pPosOld->x - pSize->z * HALF < g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
				{
					// ��O����߂荞��
					if (pPosOld->z + pSize->z * HALF < g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z))
					{
						pPos->z = g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) - pSize->z * HALF - 0.3f;

					}
					// ������߂荞��
					else if (pPosOld->z - pSize->z * HALF > g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
						pPos->z - pSize->z * HALF < g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z))
					{
						pPos->z = g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) + pSize->z * HALF + 0.3f;

					}
				}

				// �c�̓����蔻��--------------------------
				if (pPos->x - pSize->x * HALF < g_Block[nCnt].aBlock.pos.x + (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x) &&
					pPos->x + pSize->x * HALF > g_Block[nCnt].aBlock.pos.x - (g_pBlock[nType].size.x * HALF) * (g_Block[nCnt].aBlock.Scal.x))
				{
					if (pPosOld->z - pSize->z * HALF < g_Block[nCnt].aBlock.pos.z + (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z) &&
						pPos->z + pSize->z * HALF > g_Block[nCnt].aBlock.pos.z - (g_pBlock[nType].size.z * HALF) * (g_Block[nCnt].aBlock.Scal.z))
					{
						// �ォ��߂荞�񂾎�
						if (pPosOld->y >= g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y) &&
							pPos->y < g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y))
						{
							// pPlayer->bLanding = true;	// �W�����v
							pPos->y = (g_Block[nCnt].aBlock.pos.y + (g_pBlock[nType].size.y * HALF) * (g_Block[nCnt].aBlock.Scal.y));
							pMove->y = 0.0f;

						}
						// ������߂荞�񂾎�
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
//�@�u���b�N�̎擾����
//=============================
BlockInfo * GetBlock()
{
	return &g_Block[0];
}
//=============================
//�@�u���b�N�̃��f�����̎擾
//=============================
BlockModel* GetInfoModel()
{
	return &g_pBlock[0];
}
//==========================================
//�@�e�L�X�g�t�@�C���u���b�N�̓ǂݍ��ݏ���
//==========================================
void LoadtxtBlock()
{
	// �t�@�C���|�C���^��錾
	FILE* pFile;

	// �t�@�C���p�X����
	g_textPass = TextFilepass();

	// �t�@�C���J��
	pFile = fopen(BLOCK_TEXTFILENAME[g_textPass], "r");

	// �u���b�N�̏���������
	InitBlock();

	if (pFile != NULL)
	{// �t�@�C�����J������
		// �e�ϐ��ɑ������
		char aSt[256] = {};
		char aGomi[5] = {};
		int nData = 0;
		int nBlock = 0;
		int nType = 0;
		int nCnt = 0;

		D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f); // ���W
		D3DXVECTOR3 Scal(1.0f, 1.0f, 1.0f); // �g�嗦
		int nNumType = 0; // ��ޔԍ�

		while (1)
		{
			// nData�ɑ������
			nData = fscanf(pFile, "%s", &aSt[0]);

			if (strcmp(aSt, "NUM_BLOCK") == 0)
			{// NUM_BLOCK��ǂݎ������
				// �ǂݔ�΂�
				fscanf(pFile, "%s", &aGomi[0]);

				// �u���b�N�̐���ǂݍ���
				fscanf(pFile, "%d", &nBlock);

			}

			if (strcmp(aSt, "START_BLOCKSET") == 0)
			{// START_BLOCKSET��ǂݎ������

				while (nCnt < nBlock)
				{
					fscanf(pFile, "%s", &aSt[0]);

					if (strcmp(aSt, "POS") == 0)
					{// POS��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// ���W����
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);

					}

					if (strcmp(aSt, "SCAL") == 0)
					{// SCAL��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// �g�嗦����
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);

					}

					if (strcmp(aSt, "TYPE") == 0)
					{// TYPE��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// ���݂̃u���b�N�̎�ޔԍ�����
						fscanf(pFile, "%d", &nNumType);
					}

					if (strcmp(aSt, "END_BLOCKSET") == 0)
					{// END_BLOCKSET��ǂݎ������
						// ������
						SetBlock(pos, nNumType, Scal);

						// �C���N�������g���Ď��̃u���b�N����
						nCnt++;
					}
				}
				
			}

			if (nData == EOF)
			{// EOF��ǂݎ������
				// whike���𔲂���
				break;
			}
		}
	}
	else
	{// �t�@�C�����J���Ȃ������Ƃ�
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�J���܂���", "�G���[", MB_OK);
		pFile = NULL;
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//=============================
//�@�u���b�N�̔���\��
//=============================
void DrawCollisionBox(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& scale,const D3DXVECTOR3& rot,D3DXMATRIX& mtxWorld)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxTrans, mtxScal,mtxRot;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScal, size.x * scale.x, size.y * scale.y, size.z * scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScal);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���C���[�t���[�����[�h�ɐݒ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// �F��ݒ�
	D3DMATERIAL9 mat;

	// �[���������[�֐�
	ZeroMemory(&mat, sizeof(D3DMATERIAL9));

	// �}�e���A���ݒ�
	mat.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
	mat.Ambient = mat.Diffuse;

	// �}�e���A����߂�
	pDevice->SetMaterial(&mat);

	// ���z���b�V���쐬
	LPD3DXMESH pMesh = NULL;

	// �{�b�N�X�쐬
	D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &pMesh, NULL);

	// ���b�V���̔j��
	if (pMesh)
	{
		// �Z�b�g
		pMesh->DrawSubset(0);

		pMesh->Release();  // ���������
	}

	// ���̕`�惂�[�h�ɖ߂�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
#endif
