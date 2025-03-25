//============================
//
// �}�E�X [ mouse.cpp ]
// Author:Asuma Nishio
//
//============================

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include "mouse.h"
#include "camera.h"
#include <stdio.h>
#include "mapedit.h"

//****************************
// �O���[�o���ϐ��錾
//****************************
#define MAX_MOUSE (32)

//****************************
// �O���[�o���ϐ��錾
//****************************
LPDIRECTINPUT8 g_pInputInterface = NULL;
LPDIRECTINPUTDEVICE8 g_pMouseDevice = NULL;
DIMOUSESTATE g_MouseState;
DIMOUSESTATE g_MouseOldState;
DIMOUSESTATE g_PrevMouseState;
DIMOUSESTATE g_CurrentMouseState;
D3DXMATRIX g_ProjMatrix;
D3DXMATRIX g_ViewMatrix;

D3DXVECTOR3 g_MousePos = { 0.0f,0.0f,0.0f };
D3DXVECTOR3 g_CalcMousePos = { 0.0f,0.0f,0.0f };
D3DXVECTOR3 g_NorMouseVec = { 0.0f,0.0f,0.0f };
D3DXVECTOR3 g_MouseFarPos = { 0.0f,0.0f,0.0f };
D3DXVECTOR3 g_ray = { 0.0f,0.0f,0.0f };

//****************************
// �v���g�^�C�v�錾
//****************************
D3DXVECTOR3 CalcScreenToWorldFar(float screenX, float screenY, float screenZ, D3DXMATRIX* View, D3DXMATRIX* Prj);

//============================
// �}�E�X�̏���������
//============================
HRESULT InitMouse(HINSTANCE hInstance,HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL)))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pInputInterface->CreateDevice(
		GUID_SysMouse,
		&g_pMouseDevice,
		NULL)))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(g_pMouseDevice->SetCooperativeLevel(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̃A�N�Z�X�����l��
	g_pMouseDevice->Acquire();

	return S_OK;
}
//============================
// �}�E�X�̏I������
//============================
void UninitMouse(void)
{
	//���̓f�o�C�X(�}�E�X)�̔j��
	if (g_pMouseDevice != NULL)
	{
		g_pMouseDevice->Unacquire();//�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pMouseDevice->Release();
		g_pMouseDevice = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̍U��
	if (g_pInputInterface != NULL)
	{
		g_pInputInterface->Release();
		g_pInputInterface = NULL;
	}
}
//============================
// �}�E�X�̍X�V����
//============================
void UpdateMouse(void)
{
	g_MouseOldState.lX = g_CurrentMouseState.lX;
	g_MouseOldState.lY = g_CurrentMouseState.lY;

	// �X�V�O�ɍŐV�}�E�X����ۑ�����
	g_PrevMouseState = g_CurrentMouseState;

	// �ŐV�̃}�E�X�̏�Ԃ��X�V
	HRESULT	hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);

	if (FAILED(hr))
	{
		g_pMouseDevice->Acquire();
	}

	// �}�E�X���W���擾����
	POINT p;
	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &p);

	g_CurrentMouseState.lX = p.x;
	g_CurrentMouseState.lY = p.y;

#if 1
	//--------------------------------------------
	// �[�x��1.0f�̎��̃��[���h���W
	//--------------------------------------------
	float screenX = (float)p.x;
	float screenY = (float)p.y;
	float screenZ = 1.0f; // �t�@�[�N���b�v�ʑ��̍��W (���C�L���X�g�Ȃǂł悭�g��)

	// ���[���h���W
	D3DXVECTOR3 farPos = CalcScreenToWorldFar(screenX, screenY, screenZ, &GetCamera()->mtxView, &GetCamera()->mtxProjection);
#endif
	//--------------------------------------------
	// �[�x��0.0f�̎��̃��[���h���W
	//--------------------------------------------
	float fX = (float)p.x;
	float fY = (float)p.y;
	float fZ = 0.0f; // �ŋߓ_���̍��W (���C�L���X�g�Ȃǂł悭�g��)

	// ���[���h���W
	D3DXVECTOR3 nearPos = CalcScreenToWorld(fX, fY, fZ, &GetCamera()->mtxView, &GetCamera()->mtxProjection);

	//// �x�N�g������
	//g_NorMouseVec = farPos - nearPos;

	//// ���K������
	//D3DXVec3Normalize(&g_NorMouseVec, &g_NorMouseVec);

	// ���C���������߂�
	D3DXVec3Subtract(&g_ray, &farPos, &nearPos);
	D3DXVec3Normalize(&g_ray, &g_ray);
}
//============================
//�N���b�N���ꂽ
//============================
bool OnMouseTriggerDown(int button_type)
{
	if (!(g_PrevMouseState.rgbButtons[button_type] & (0x80)) &&
		g_CurrentMouseState.rgbButtons[button_type] & (0x80))
	{
		return true;
	}
	return false;
}
//============================
// �N���b�N����߂�
//============================
bool OnMouseTriggerUp(int button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & (0x80) &&
		!(g_CurrentMouseState.rgbButtons[button_type] & (0x80)))
	{
		return true;
	}

	return false;
}
//============================
// �J�[�\���֌W
//============================
void SetCursorVisibility(bool visible)
{
	static int cursurCount = 0;

	if(visible)
	{
		//�J�[�\����\��
		while (cursurCount < 0)
		{
			ShowCursor(TRUE);
			cursurCount++;
		}
	}
	else
	{
		//�J�[�\�����\��
		while (cursurCount >= 0)
		{
			ShowCursor(FALSE);
			cursurCount--;
		}
	}
}
//============================
// �ړ��ʂ̎擾
//============================
D3DXVECTOR2 GetMouseVelocity()
{
	return D3DXVECTOR2((float)g_CurrentMouseState.lX, (float)g_CurrentMouseState.lY);
}
//============================
// �ߋ��̈ړ��ʂ̎擾
//============================
D3DXVECTOR2 GetMouseOldVelocity()
{
	return D3DXVECTOR2((float)g_MouseOldState.lX, (float)g_MouseOldState.lY);
}
//============================
// �ߋ��̈ړ��ʂ̎擾
//============================
bool OnMousePress(int button_type)
{
	return (g_CurrentMouseState.rgbButtons[button_type] & 0x80) ? true : false;
}
//============================
// �}�E�X�z�C�[���̎擾
//============================
BOOL IsMouseWheelPresent(void)
{
	return (GetSystemMetrics(SM_MOUSEWHEELPRESENT) != 0);
}
//=====================================================
//�}�E�X�̏�Ԏ擾
//=====================================================
LPDIRECTINPUTDEVICE8 GetMouseDevice(void)
{
	return g_pMouseDevice;
}
//=====================================================
//�}�E�X�̏��
//=====================================================
bool GetMouseState(DIMOUSESTATE* mouseState)
{
	// �}�E�X�f�o�C�X���擾
	LPDIRECTINPUTDEVICE8 pMouse = GetMouseDevice();

	if (pMouse == NULL)
	{
		return false;
	}

	// �}�E�X�̏�Ԃ��擾(��������������)
	HRESULT hr = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)mouseState);

	if (FAILED(hr))
	{
		// ���̓f�o�C�X�����Z�b�g����Ă���ꍇ�A�Ď擾�����݂�
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			pMouse->Acquire();

			// �Ď擾�����݂�
			hr = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)mouseState);
			if (FAILED(hr))
			{
				return false;
			}
		}
		else
		{
			//�G���[�̏ꍇ
			return false;
		}
	}

	return true; // ����Ɏ擾�ł����ꍇ
}
//=======================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=======================================
D3DXVECTOR3 CalcScreenToWorld(float screenX, float screenY, float screenZ, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �C���o�[�X�p�̃}�g���b�N�X��錾
	D3DXMATRIX InvView, InvPrj, InvViewport, transform;

	// �r���[�}�g���b�N�X,�v���W�F�N�V�����}�g���b�N�X�ɑ΂���C���o�[�X�̐ݒ�
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);

	// �r���[�|�[�g�ϊ����s��
	D3DXMatrixIdentity(&InvViewport);

	// �r���[�|�[�g�ɑ������
	InvViewport._11 = SCREEN_WIDTH / 2.0f;
	InvViewport._22 = -SCREEN_HEIGHT / 2.0f;
	InvViewport._41 = SCREEN_WIDTH / 2.0f;
	InvViewport._42 = SCREEN_HEIGHT / 2.0f;

	// �r���[�|�[�g�ɐݒ�
	D3DXMatrixInverse(&InvViewport, NULL, &InvViewport);

	// �}�g���b�N�X���
	transform = InvViewport * InvPrj * InvView;

	// �}�E�X���W�ɑ��
	g_MousePos.x = screenX;
	g_MousePos.y = screenY;
	g_MousePos.z = screenZ;

	// ���
	D3DXVECTOR3 ScreenPos = { g_MousePos.x ,g_MousePos.y ,g_MousePos.z };

	// ���W�ϊ�
	D3DXVec3TransformCoord(&g_CalcMousePos, &ScreenPos, &transform);

	// �ϊ���̍��W��Ԃ�
	return g_CalcMousePos;
}

//=======================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=======================================
D3DXVECTOR3 CalcScreenToWorldFar(float screenX, float screenY, float screenZ, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �C���o�[�X�p�̃}�g���b�N�X��錾
	D3DXMATRIX InvView, InvPrj, InvViewport, transform;

	// �r���[�}�g���b�N�X,�v���W�F�N�V�����}�g���b�N�X�ɑ΂���C���o�[�X�̐ݒ�
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);

	// �r���[�|�[�g�ϊ����s��
	D3DXMatrixIdentity(&InvViewport);

	// �r���[�|�[�g�ɑ������
	InvViewport._11 = SCREEN_WIDTH / 2.0f;
	InvViewport._22 = -SCREEN_HEIGHT / 2.0f;
	InvViewport._41 = SCREEN_WIDTH / 2.0f;
	InvViewport._42 = SCREEN_HEIGHT / 2.0f;

	// �r���[�|�[�g�ɐݒ�
	D3DXMatrixInverse(&InvViewport, NULL, &InvViewport);

	// �}�g���b�N�X���
	transform = InvViewport * InvPrj * InvView;

	// �}�E�X���W�ɑ��
	g_MousePos.x = screenX;
	g_MousePos.y = screenY;
	g_MousePos.z = screenZ;

	// ���
	D3DXVECTOR3 ScreenPos = { g_MousePos.x ,g_MousePos.y ,g_MousePos.z };

	// ���W�ϊ�
	D3DXVec3TransformCoord(&g_MouseFarPos, &ScreenPos, &transform);

	// �ϊ���̍��W��Ԃ�
	return g_MouseFarPos;
}

D3DXVECTOR3 GetMousePosCur()
{
	return g_MousePos;
}
D3DXVECTOR3 GetWorldMousePosCur()
{
	return g_CalcMousePos;
}
D3DXVECTOR3 GetVecMousePosCur()
{
	return g_ray;
}
D3DXVECTOR3 GetFarMousePosCur()
{
	return g_MouseFarPos;
}

