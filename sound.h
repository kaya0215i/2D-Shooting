//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_ENTER,			// エンター押下音
	SOUND_LABEL_SE_SHOT_PLAYER,		// プレイヤー弾発射音
	SOUND_LABEL_SE_SHOT_ENEMY,		// 敵弾発射音
//	SOUND_LABEL_SE_DAMAGE_PLAYER,	// プレイヤーダメージ音
//	SOUND_LABEL_SE_DAMAGE_ENEMY,	// 敵ダメージ音
	SOUND_LABEL_BGM1,				// BGM1 (タイトル画面)
	SOUND_LABEL_BGM2,				// BGM2 (ゲーム画面)
	SOUND_LABEL_BGM3,				// BGM3 (クリア画面)
	SOUND_LABEL_BGM4,				// BGM4 (ゲームオーバー画面)
	SOUND_LABEL_BGM5,
	SOUND_LABEL_BGM6,
	SOUND_LABEL_BGM7,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
