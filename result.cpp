//=======================================
//
// ���U���g���� [ result.cpp ]
// Author:Asuma Nishio
//
//========================================

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include "result.h"
//#include "fade.h"
//#include "player.h"
//#include "score.h"
//#include "sound.h"
#include "resultscore.h"
#include "fade.h"
#include "input.h"
//#include "Timer.h"
//#include "boss.h"
//#include "item.h"
//#include "exit.h"

//******************************************
// �O���[�o���ϐ��錾
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResult= NULL;				// �e�N�X�`���̃|�C���^
int g_aCount;											// �J�E���g

//========================
// ���U���g�̏���������
//========================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
#if 0
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �^�C�}�[�����擾
	int nTime = GetTimer();
	
	// �A�C�e���̔�����擾
	bool bGet = bItemGetting();

	bool bExit = GetExit();

	// ���U���g�ɉ�������������
	int nData = GetNumber();

	//�e�N�X�`���t�@�C���p�X
	const char* TextureName = NULL;

	if (pPlayer->bDisp == true && bExit == true)
	{
		PlaySound(SOUND_LABEL_RESULT1);

		if (pBoss->bUse == false)
		{
			// �A�C�e�����ɂ���ĕύX
			if ((pItem->bUse == true)
				&& pItem->nType == ITEMTYPE_CURRY)
			{
				TextureName = "data\\TEXTURE\\result05.png";
			}

			switch (nData)
			{
			case 0:
				TextureName = "data\\TEXTURE\\result05.png";
				break;
			case 1:
				TextureName = "data\\TEXTURE\\result01.png";
				break;
			case 2:
				TextureName = "data\\TEXTURE\\resulto3.png";
				break;
			case 3:
				TextureName = "data\\TEXTURE\\result06.png";
				break;
			case 4:
				TextureName = "data\\TEXTURE\\result04.png";
				break;
			default:
				break;
			}
		}
		else
		{
			PlaySound(SOUND_LABEL_RESULT1);

			switch (nData)
			{
			case 0:
				TextureName = "data\\TEXTURE\\blue.png";
				break;
			case 4:
				TextureName = "data\\TEXTURE\\result04.png";
				break;
			default:
				break;
			}
		}
	}
	else if (pPlayer->bDisp == false)
	{
		TextureName = "data\\TEXTURE\\result02.png";

		PlaySound(SOUND_LABEL_RESULT2);
	}
	else if (nTime <= 0)
	{
		TextureName = "data\\TEXTURE\\gameover.png";
		PlaySound(SOUND_LABEL_RESULT2);

	}
#endif
	
	// �Q�[���I�[�o�[
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\result.png",
		&g_pTextureResult);

	// ������
	g_aCount = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D* pVtx;		// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1�ڂ̒��_���
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);		// 2�ڂ̒��_���
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);		// 3�ڂ̒��_���
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);	// 4�ڂ̒��_���

	// rhw�̐ݒ�(1.0f�ŌŒ�)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �A�����b�N
	g_pVtxBuffResult->Unlock();

	// ���U���g�X�R�A�̏�����
	InitResultScore();
}
//========================
// ���U���g�̏I������
//========================
void UninitResult(void)
{
	// ���y���~
	// StopSound();

	// ���U���g�X�R�A�̏I��
	UninitResultScore();

	// �e�N�X�`���̔j��
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

}
//========================
// ���U���g�̍X�V����
//========================
void UpdateResult(void)
{
	//�J�E���g�����Z
	g_aCount++;

	if (g_aCount >= 380 || KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{// ENTER�L�[ or �p�b�h��A�{�^��
		// ���[�h�ݒ�(�����L���O�Ɉړ�)
		SetFade(MODE_RANKING);
	}

	// ���U���g�X�R�A�̍X�V
	// UpdateResultScore();
}
//========================
// ���U���g�̕`�揈��
//========================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult);

	// ���U���g�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���U���g�X�R�A�̕`��
	//DrawResultScore();

}
