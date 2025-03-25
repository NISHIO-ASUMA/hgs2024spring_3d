//=================================
//
// �X�R�A���� [ score.h ]
// Author:NISHIO ASUMA
//
//==================================
#ifndef  _SCORE_H_
#define  _SCORE_H_

//***************************************
// �C���N���[�h�t�@�C���錾
//***************************************
#include "main.h"

//***************************************
// �}�N����`
//***************************************
#define MAX_NUM_SCORE (8)		// �\�����錅�̍ő吔
#define NUM_DIGITS_8 (10000000) // ����ő包��(8��)
#define NUM_DIGITS_7 (1000000)  // ���錅��(7��)

//***************************************
// �v���g�^�C�v�錾
//***************************************
void InitScore(void);				// �X�R�A�̏�����
void UninitScore(void);				// �X�R�A�̏I��
void UpdateScore(void);				// �X�R�A�̍X�V
void DrawScore(void);				// �X�R�A�̕`��
void SetScore(int nScore);			// �X�R�A�̐ݒ�
void AddScore(int nValue);			// �X�R�A�̉��Z
int ScoreCounter(int nScore);		// �X�R�A��0���J�E���g
int GetScore(void);					// �X�R�A���擾
void SaveScore(void);				// �X�R�A�̏����o��

#endif // ! _SCORE_H_
