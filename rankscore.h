//============================================
//
// �����L���O�X�R�A���� [ rankscore.h ]
// Author:NISHIO ASUMA
//
//============================================
#ifndef _RANKSCORE_H_
#define _RANKSCORE_H_

//***************************************
// �C���N���[�h�t�@�C���錾
//***************************************
#include "main.h"

//***************************************
// �v���g�^�C�v�錾
//***************************************
void InitRankingScore(void);			// �����L���O�X�R�A�̏�����
void UninitRankingScore(void);			// �����L���O�X�R�A�̏I��
void UpdateRankingScore(void);			// �����L���O�X�R�A�̍X�V
void DrawRankingScore(void);			// �����L���O�X�R�A�̕`��
void SetRankingTexture(void);			// �����L���O�̃e�N�X�`���̏���
void SetRanking(int nScore);			// �X�R�A��ݒ�(�X�R�A����n��)
void ResetRanking(void);				// �����L���O���擾
void WriteScore(void);					// �����o��

#endif