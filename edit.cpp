//=======================================
//
// �c�[����ʏ��� [ edit.cpp ]
// Author:Asuma Nishio
//
//=======================================

//*************************************
// �C���N���[�h�t�@�C���錾
//*************************************
#include "edit.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include "mapedit.h"
#include "block.h"
#include "input.h"
// #include "player.h"
#include "shadow.h"
#include "fade.h"

//*************************************
// �O���[�o���ϐ��錾
//*************************************
bool isEdit;
MODE g_GameMode = MODE_EDIT; // �O���[�o���ϐ��Ń��[�h���Ǘ�

//=====================================
// ����������
//=====================================
void InitEdit()
{
	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// �e�̏�����
	InitShadow();

	// �}�b�v�G�f�B�^�[�̏�����
	InitMapEdit();

	// �u���b�N�̏�����
	InitBlock();

	// �O���[�o���ϐ��̏�����
	g_GameMode = MODE_EDIT; // �������[�h
}
//=====================================
// �I������
//=====================================
void UninitEdit()
{
	// �J�����̏I��
	UninitCamera();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// �G�f�B�^�[�̏I��
	UninitMapEdit();

	// �e�̏I��
	UninitShadow();

	// �u���b�N�̏I��
	UninitBlock();
}
//=====================================
// �X�V����
//=====================================
void UpdateEdit()
{
	// �J�����̍X�V
	UpdateCamera();

	// ���݂̃��[�h�ɉ����čX�V�����𕪊�
	if (g_GameMode == MODE_EDIT)
	{
		// �G�f�B�^�[�̍X�V
		UpdateMapEdit();
	}

	if (KeyboardTrigger(DIK_F1))
	{// F1�L�[��������
		// �Q�[���ɑJ��
		SetFade(MODE_GAME);
	}
}
//=====================================
// �`�揈��
//=====================================
void DrawEdit()
{
	// �J�������Z�b�g
	SetCamera();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField();

	//----------------------------
	// �f�o�b�O�t�H���g
	//----------------------------
	DrawEditkey();
	DebugEditModelInfo();
	DrawCameraPos();
	DrawNumBlock();

	if (g_GameMode == MODE_EDIT)
	{
		// �G�f�B�^�[�̕`��
		DrawMapEdit();

		// �e�̕`��
		DrawShadow();

	}
}
//===============================
// �ҏW���[�h����MODE��؂�ւ���
//===============================
MODE GetNowMode()
{
	return g_GameMode;
}