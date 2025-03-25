//===========================
//
// �|�[�Y���� [ pause.h ]
// Author:Asuma Nishio
//
//===========================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************
// �C���N���[�h�t�@�C���錾
//*****************************
#include "main.h"

//*****************************
// �|�[�Y���j���[�̎�ނ̗񋓌^
//*****************************
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,// �Q�[���ɖ߂�
	PAUSE_MENU_RETRY,       // �Q�[������蒼��
	PAUSE_MENU_QUIT,        // �^�C�g���ɖ߂�
	PAUSE_MENU_MAX
}PAUSE_MENU;

//*********************************
// �|�[�Y���j���[�̃e�N�X�`���錾
//*********************************
static const char* PAUSE_TEX[PAUSE_MENU_MAX] =
{
	"data\\TEXTURE\\pause000.png", // continue
	"data\\TEXTURE\\pause001.png", // retry
	"data\\TEXTURE\\pause001.png", // quit
};

//*****************************
// �v���g�^�C�v�錾
//*****************************
void InitPause(void);		// �|�[�Y��ʂ̏�����
void UninitPause(void);		// �|�[�Y��ʂ̏I��
void UpdatePause(void);		// �|�[�Y��ʂ̍X�V
void DrawPause(void);		// �|�[�Y��ʂ̕`��

#endif