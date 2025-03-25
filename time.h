//=============================================================================
//
// タイム処理 [time.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//マクロ定義
#define MAX_TIMEDIGIT (2)//桁の最大数

//タイム構造体
typedef struct
{
	bool bUse;
}Time;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);

int GetTimeMinutes(void);
int GetTimeSeconds(void);

#endif

