//=================================
//
// �^�C�g������ [title.cpp]
// Author: Asuma Nishio
//
//=================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "Title.h"
#include "input.h"

//***************************
// �O���[�o���ϐ��錾
//***************************
Title g_Title;											// �\���̕ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleback = NULL;		// �^�C�g���w�i
LPDIRECT3DTEXTURE9 g_pTextureTitleback = NULL;			// �^�C�g���w�i�e�N�X�`���̃|�C���^

//===========================
// �^�C�g������������
//===========================
void InitTitle(void)
{
	// �f�o�C�X�ւ̃|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;				

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title.png",
		&g_pTextureTitleback);
	
	// ���_�o�b�t�@�̐��� �w�i
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleback,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleback->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �A�����b�N
	g_pVtxBuffTitleback->Unlock();
	
}
//===================
// �^�C�g���̏I������
//===================
void UninitTitle(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureTitleback != NULL)
	{
		g_pTextureTitleback->Release();
		g_pTextureTitleback = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleback != NULL)
	{
		g_pVtxBuffTitleback->Release();
		g_pVtxBuffTitleback = NULL;
	}

}
//=======================
// �^�C�g���̍X�V����
//=======================
void UpdateTitle(void)
{
#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F1))
	{// F1�L�[�ŕҏW����
		// ��ʑJ��
		SetFade(MODE_EDIT);
	}
#endif
	if (KeyboardTrigger(DIK_RETURN))
	{// Enter�L�[�ŉ�ʑJ��
		// �Q�[�����
		SetFade(MODE_GAME);	}
}
//===================
// �^�C�g���̕`�揈��
//===================
void DrawTitle(void)
{
	// �f�o�C�X�ւ̃|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleback, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleback);

	// �^�C�g���|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
