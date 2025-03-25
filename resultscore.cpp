//=======================================
//
// ���U���g�X�R�A���� [ resultscore.cpp ]
// Author:Asuma Nishio
//
//========================================

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include "resultscore.h"
#include "result.h"
#include "score.h"
#include <stdio.h>
#include "rankscore.h"

//******************************************
// ���U���g�X�R�A�̍\���̐錾
//******************************************
typedef struct
{
	bool bUse; // �g�p���Ă��邩�ۂ�
}Result;

//******************************************
// �O���[�o���ϐ��錾
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;		// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			// �e�N�X�`���̃|�C���^

Result g_aResult[MAX_NUM_SCORE];	// ���U���g�X�R�A�̍\����
int g_nResultScore;					// ���U���g�X�R�A

//============================
// ���U���g�X�R�A�̏���������
//============================
void InitResultScore(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number003.png",
		&g_pTextureResultScore);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	// �O���[�o���ϐ��̏�����
	g_nResultScore = 0;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUM_SCORE; nCnt++)
	{
		g_aResult[nCnt].bUse = true;		// �g�p���Ă�����

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f + 50.0f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f, 300.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(545.0f + nCnt * 50.0f + 50.0f, 300.0f, 0.0f);

		//r hw�̐ݒ�(1.0f�ŌŒ�)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// ���_���̃|�C���^��i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffResultScore->Unlock();

	 LoadScore();			// �X�R�A��ǂݍ���
	 SetResultScore();	// ���U���g�X�R�A�̐ݒ�
}
//===========================
// ���U���g�X�R�A�̏I������
//===========================
void UninitResultScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}

}
//===========================
// ���U���g�X�R�A�̍X�V����
//===========================
void UpdateResultScore(void)
{
	// �Ȃ�
}
//===========================
// ���U���g�X�R�A�̕`�揈��
//===========================
void DrawResultScore(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (g_aResult[nCntscore].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResultScore);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntscore * 4, 2);
		}
	}
}
//=============================
// ���U���g�X�R�A�̐ݒ�
//=============================
void SetResultScore()
{
	int aPosTexU[MAX_NUM_SCORE] = {};		// �������̐������i�[
	int aData = NUM_DIGITS_8;				// 8��
	int aData2 = NUM_DIGITS_7;				// 7��

	VERTEX_2D* pVtx;			// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (nCntscore == 0)		// 0�Ԗڂ̎�
		{
			aPosTexU[0] = g_nResultScore / aData;
		}
		else
		{
			// 0�ԖڈȊO�̎�
			aPosTexU[nCntscore] = g_nResultScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 1.0f);

		pVtx += 4;

	}

	// �A�����b�N
	g_pVtxBuffResultScore->Unlock();
}
//================================
// �X�R�A�̓ǂݍ���
//================================
void LoadScore(void)
{
	// �t�@�C���|�C���^��錾
	FILE* pFile;

	// �t�@�C���J��
	pFile = fopen("data\\Lastscore.txt", "r");

	if (pFile != NULL)
	{// �t�@�C���J������

		// �X�R�A������
		fscanf(pFile, "%d", &g_nResultScore);
		
	}
	else
	{
		// �J���Ȃ�������
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�J���܂���", "�G���[", MB_OK);

		return;
	}

	// �t�@�C�������
	fclose(pFile);
}