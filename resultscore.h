//=============================================
//
// ���U���g�X�R�A���� [ resultscore.h ]
// Author:Asuma Nishio
//
//=============================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

//*********************************************
// �C���N���[�h�t�@�C���錾
//*********************************************
#include "main.h"

//*********************************************
// �v���g�^�C�v�錾
//*********************************************
void InitResultScore(void);			// ���U���g��ʂ̃X�R�A�̏�����
void UninitResultScore(void);		// ���U���g��ʂ̃X�R�A�̏I��
void UpdateResultScore(void);		// ���U���g��ʂ̃X�R�A�̍X�V
void DrawResultScore(void);			// ���U���g��ʂ̃X�R�A�̕`��
void SetResultScore(void);			// ���U���g�X�R�A�̐ݒ�
void LoadScore(void);				// ���U���g�X�R�A�̓ǂݍ���

#endif