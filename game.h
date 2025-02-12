#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//ゲームの状態
typedef enum {
	GAMESTATE_NONE = 0, //何もしていない状態
	GAMESTATE_NORMAL,   //通常状態
	GAMESTATE_END,      //終了状態
	GAMESTATE_MAX
}GAMESTATE;

typedef struct {
	int g_nWave;
	bool g_bWaveMove;
}Wave;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
void SetEnablePause(bool bPause);
Wave* GetWave(void);

#endif