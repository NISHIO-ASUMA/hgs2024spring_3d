//===============================
//
// �X�R�A���� [ score.cpp ]
// Author:Asuma Nishio
//
//===============================

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include "score.h"
#include <cstdio>

//******************************************
// �X�R�A���̍\���̐錾
//******************************************
typedef struct
{
	bool bUse;	// �g�p���Ă��邩
}Score;

//******************************************
// �O���[�o���ϐ��錾
//******************************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		// �e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// ���_�o�b�t�@�̃|�C���^
Score g_aScore[MAX_NUM_SCORE];		// �X�R�A�^�̃O���[�o���ϐ�
int g_nScore;						// �X�R�A�̒l

//==========================
//�@�X�R�A�̏���������
//==========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score000.png",
		&g_pTextureScore);

	// �O���[�o���ϐ��̏�����
	g_nScore = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		// �g�p���
		g_aScore[nCntscore].bUse = true;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f ,			5.0f,  0.0f);
		pVtx[1].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f + 50.0f,	5.0f,  0.0f);
		pVtx[2].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f ,			35.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(850.0f + nCntscore * 50.0f + 50.0f,	35.0f, 0.0f);

		// rhw�̐ݒ�(1.0f�ŌŒ�)
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

		pVtx += 4;

	}

	// �A�����b�N
	g_pVtxBuffScore->Unlock();

}
//==========================
// �X�R�A�̏I������
//==========================
void UninitScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

}
//==========================
// �X�R�A�̍X�V����
//==========================
void UpdateScore(void)
{
	// ���߂��X�R�A�̌�����ۑ�
	int nData = ScoreCounter(g_nScore);

	for ( int nData1 = 0; nData1 < MAX_NUM_SCORE; nData1++)
	{
		if (nData1 >= MAX_NUM_SCORE - nData)
		{
			// MAX_NUM_SCORE - nData��0���\������Ă��鎞(nData1��茅���̒l���������Ƃ�)
			g_aScore[nData1].bUse = true;	// ��̏�����L��������
		}
		else
		{
			// ����ȊO�̏�Ԃ̎�
			g_aScore[nData1].bUse = false;
		}
	}
}
//==========================
// �X�R�A�̕`�揈��
//==========================
void DrawScore(void)
{	
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE;nCntscore++)
	{
		if (g_aScore[nCntscore].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore);

			// �|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntscore * 4, 2);
		}
	}

	// �A�����b�N
	g_pVtxBuffScore->Unlock();

}
//==========================
// �X�R�A�̐ݒ菈��
//==========================
void SetScore(int nScore)
{
	int aPosTexU[MAX_NUM_SCORE] = {};	// �������̐������i�[
	int aData = NUM_DIGITS_8;				// 8��
	int aData2 = NUM_DIGITS_7;				// 7��

	g_nScore = nScore;			// �X�R�A���擾

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;			

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if ( nCntscore == 0)	// 0�Ԗڂ̎�
		{
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			// 0�ԖڈȊO�̎�
			aPosTexU[nCntscore] = g_nScore % aData / aData2;
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
	g_pVtxBuffScore->Unlock();
}
//========================
// �X�R�A�̉��Z����
//========================
void AddScore(int nValue)
{
	// �e���̐������i�[
	int aPosTexU[MAX_NUM_SCORE] = {};		
	int aData = NUM_DIGITS_8;
	int aData2 = NUM_DIGITS_7;

	g_nScore += nValue;

	// �e���̐������i�[
	VERTEX_2D* pVtx;		

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (nCntscore == 0)		
		{// 0�Ԗڂ�������
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			// 0�ԖڈȊO�̎�
			aPosTexU[nCntscore] = g_nScore %   aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 1.0f);

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffScore->Unlock();
}
//=====================
// �X�R�A��0���J�E���g
//=====================
int  ScoreCounter(int nScore)
{
	int nNum = nScore;		// �X�R�A��0�̕������J�E���g
	int nKeepNum = 0;		// �X�R�A�̌���
							   
	while (nNum != 0)		// ����؂�Ȃ��Ȃ�܂ŌJ��Ԃ�(0�܂�)
	{						   
		nNum /= 10;			// nNum��10�Ŋ����Ă���
		nKeepNum++;			// ���������Z
	}
	if (nScore == 0)
	{
		// 1���ڂ�0�������Ă���Ƃ�
		nKeepNum = 1;
	}
	return nKeepNum;		// �X�R�A�̌�����Ԃ�
}
//=====================
// �X�R�A��ۑ�
//=====================
int GetScore(void)
{
	return g_nScore;		// �X�R�A�̒l��Ԃ�
}
//=====================
// �X�R�A�̏����o������
//=====================
void SaveScore(void)
{
	// �t�@�C���|�C���^
	FILE* pFile;

	// �t�@�C�����J��
	pFile = fopen("data\\Lastscore.txt", "w");

	// �t�@�C�����J������
	if (pFile != NULL)
	{
		// �z�u��������������
		fprintf(pFile, "%d\n", g_nScore);
	}
	else
	{
		// �t�@�C�����J���Ȃ�������
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�J���܂���", "�G���[", MB_OK);

		return;
	}
	// �t�@�C�������
	fclose(pFile);
}