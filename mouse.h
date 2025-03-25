//============================
//
// マウス [mouse.h]
// Author:Asuma Nishio
//
//============================
#ifndef _MOUSE_H_
#define _MOUSE_H_

//****************************
// インクルードファイル
//****************************
#include "main.h"

//****************************
// マクロ定義
//****************************
#define LEFT_MOUSE (0)  // マウスの左クリック
#define RIGHT_MOUSE (1) // マウスの右クリック
#define MOUSE_WHEEL (2) // マウスホイールクリック

//****************************
// プロトタイプ宣言
//****************************
HRESULT InitMouse(HINSTANCE hInstance,HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool OnMouseTriggerDown(int button_type);
bool OnMouseTriggerUp(int button_type);
D3DXVECTOR2 GetMouseVelocity();
D3DXVECTOR2 GetMouseOldVelocity();
bool OnMousePress(int button_type);
BOOL IsMouseWheelPresent(void);
void SetCursorVisibility(bool visible);
LPDIRECTINPUTDEVICE8 GetMouseDevice(void);
bool GetMouseState(DIMOUSESTATE* mouseState);
D3DXVECTOR3 CalcScreenToWorld(float screenX, float screenY, float screenZ, D3DXMATRIX* View, D3DXMATRIX* Prj);
D3DXVECTOR3 GetMousePosCur();
D3DXVECTOR3 GetWorldMousePosCur();
D3DXVECTOR3 GetVecMousePosCur();
D3DXVECTOR3 GetFarMousePosCur();

#endif