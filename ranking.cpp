//==============================
//
// �����L���O���� [ranking.cpp ]
// Author:Asuma Nishio
//
//==============================

//*************************************
// �C���N���[�h�t�@�C���錾
//*************************************
#include "ranking.h"
#include <stdio.h>
#include "rankscore.h"
#include "ranksetdata.h"
//#include "sound.h"

//================================
// �����L���O�̏���������
//================================
void InitRanking(void)
{
	InitRankingData();		// ���ʂ̏�����

	InitRankingScore();		// �����L���O�̃X�R�A�̏�����

	ResetRanking();			// �����L���O�̃��Z�b�g

	SetRankingTexture();	// �����L���O�̃e�N�X�`������

	// PlaySound(SOUND_LABEL_RANKING);

}
//================================
// �����L���O�̏I������
//================================
void UninitRanking(void)
{
	// ���y���~
	//StopSound();

	UninitRankingData();	 // ���ʂ̏I������

	UninitRankingScore();	 // �����L���O�̃X�R�A�̏I������
}
//================================
// �����L���O�̍X�V����
//================================
void UpdateRanking(void)
{
	UpdateRankingData();	// ���ʂ̍X�V����

	UpdateRankingScore();	// �����L���O�̃X�R�A�̍X�V����
}
//================================
// �����L���O�̕`�揈��
//================================
void DrawRanking(void)
{
	DrawRankingData();		// ���ʂ̕`�揈��

	//DrawRankingScore();		//�����L���O�̃X�R�A�̕`�揈��
}
