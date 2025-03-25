//============================
//
// �|�[�Y���� [ pause.cpp ]
// Author:Asuma Nishio
//
//============================

//*****************************
// �C���N���[�h�t�@�C���錾
//*****************************
#include "pause.h"
#include "fade.h"
#include "input.h"
// #include "game.h"
// #include "sound.h"

//*****************************
// �O���[�o���ϐ��錾
//*****************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};	// �e�N�X�`���ւ̃|�C���^(3����)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// ���_�o�b�t�@�̃|�C���^
															   
LPDIRECT3DTEXTURE9 g_apTexturearufa = NULL;					// �e�N�X�`���ւ̃|�C���^(����)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffarufa = NULL;				// ���_�o�b�t�@�̃|�C���^
int g_nSelect;				// �I��p�̕ϐ�

//=========================
// �|�[�Y�̏���������
//=========================
void InitPause(void)
{
	// �f�o�C�X�ւ̃|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���(����)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\arufa.png",
		&g_apTexturearufa);

	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{// ���j���[�p�̑I����
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			PAUSE_TEX[nCnt],
			&g_apTexturePause[nCnt]);
	}

	// �����̑I�����[�h��ݒ�
	g_nSelect = PAUSE_MENU_CONTINUE;  

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	//=====================
	// �����|���S���̐ݒ�
	//=====================

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffarufa,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffarufa->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // 1�ڂ̒��_���
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);	 // 2�ڂ̒��_���
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);	 // 3�ڂ̒��_���
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);// 4�ڂ̒��_���

	// rhw�̐ݒ�(1.0f�ŌŒ�)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �A�����b�N
	g_pVtxBuffarufa->Unlock();

	//=========================
	// 3�̃|���S���ݒ�
	//=========================

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		//���_���W1�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(470.0f,100.0f +(nCnt * 150.0f),0.0f);	// 1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(870.0f,100.0f +(nCnt * 150.0f),0.0f);	// 2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(470.0f,280.0f +(nCnt * 150.0f),0.0f);	// 3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(870.0f,280.0f +(nCnt * 150.0f),0.0f);	// 4�ڂ̒��_���
		 
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

		// ���_��񕪐i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffPause->Unlock();

}
//=========================
// �|�[�Y�̏I��
//=========================
void UninitPause(void)
{
	// ���y���~�߂�
	// StopSound();

	//=========================
	// �����|���S���̏I��
	//=========================
	// �e�N�X�`���̏I��
	if (g_apTexturearufa != NULL)
	{			   
		g_apTexturearufa->Release();
		g_apTexturearufa = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffarufa != NULL)
	{
		g_pVtxBuffarufa->Release();
		g_pVtxBuffarufa = NULL;
	}

	//=========================
	// 3�̃|���S���̏I��
	//=========================
	// �e�N�X�`���̏I��
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		if (g_apTexturePause[nCnt] != NULL)
		{
			g_apTexturePause[nCnt]->Release();
			g_apTexturePause[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=========================
// �|�[�Y�̍X�V
//=========================
void UpdatePause(void)
{
	// ���݂̃��[�h���擾
	MODE nMode = GetMode();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{
		// ���_�J���[��ݒ�(��)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_����i�߂�
		pVtx += 4;
	}

	// pVtx��߂�
	pVtx -= 4 * PAUSE_MENU_MAX;		
	
	// ���j���[�̑I��
	if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
	{// UP�L�[ or �p�b�h�̏�L�[ or W�L�[�������ꂽ
		g_nSelect--;	// �f�N�������g
	
		// ���y�Đ�
		// PlaySound(SOUND_LABLE_PAUSEMENU);

		if (g_nSelect < PAUSE_MENU_CONTINUE)
		{// PAUSE_MENU_CONTINUE��菬����������
			// ��ԉ���I��
			g_nSelect = PAUSE_MENU_QUIT;
		}
	}

	else if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true)
	{// DOWN�L�[ or �p�b�h�̉��L�[ or S�L�[�������ꂽ
		//���������Ƃ�
		g_nSelect++;	// �C���N�������g
		
		// ���y�Đ�
		// PlaySound(SOUND_LABLE_PAUSEMENU);

		if (g_nSelect >= PAUSE_MENU_MAX)
		{// PAUSE_MENU_MAX���傫��������
			// ��ԏ��I��
			g_nSelect = PAUSE_MENU_CONTINUE;
		}
	}

	// �I����
	pVtx += 4 * g_nSelect;		

	// ���_�J���[��ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);

	// �A�����b�N
	g_pVtxBuffPause->Unlock();
#if 0
	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true) && nMode == MODE_GAME)
	{// �G���^�[�L�[�������ꂽ or A�L�[�������ꂽ
		
		//�e��ʂɈړ�
		switch (g_nSelect)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);	// �Q�[���p��
			break;

		case PAUSE_MENU_RETRY:		
			SetFade(MODE_GAME);		// �Q�[�����ŏ�����
			break;

		case PAUSE_MENU_QUIT:		
			SetFade(MODE_TITLE);	// �^�C�g���J��
			break;

		}

		// ���y���Đ�
		// PlaySound(SOUND_LABLE_PAUSESE);
	}
#endif
}
//=========================
// �|�[�Y��ʂ̕`��
//=========================
void DrawPause(void)
{
	// �f�o�C�X�ւ̃|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//=========================
	// �����|���S���̕`��
	//=========================
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffarufa, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTexturearufa);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	

	//=========================
	// 3�̃|���S���̕`��
	//=========================

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_���̃|�C���^
	VERTEX_2D* pVtx = 0;	

	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCnt]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}