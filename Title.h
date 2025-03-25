//=================================
//
// �^�C�g������ [title.h]
// Author: Asuma Nishio
//
//=================================
#ifndef _TITLE_H_
#define _TITLE_H_

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "main.h"
#include "fade.h"

//****************************
// �^�C�g���\���̐錾
//****************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���W
	int nType;					// ���
	float fWidth;				// ����
	float fHeight;				// ����
	bool bUse;					// �g�p���
}Title;

//**********************
// �v���g�^�C�v�錾
//**********************
void InitTitle(void);						// �^�C�g���̏�����
void UninitTitle(void);						// �^�C�g���̏I��
void UpdateTitle(void);						// �^�C�g���̍X�V
void DrawTitle(void);						// �^�C�g���̕`��
#endif