#ifndef _PLAYER_H_
#define _PLAYER_H_

#define SIZE_X (100)
#define SIZE_Y (100)

#include "main.h"

typedef enum {
	PLAYERSTATE_APPEAR = 0, //出現状態
	PLAYERSTATE_NORMAL,     //通常状態
	PLAYERSTATE_DAMAGE,     //ダメージ状態
	PLAYERSTATE_DEATH,      //死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	int nXsize;
	int nYsize;
	PLAYERSTATE state; //プレイヤーの状態
	int nCounterState; //状態管理カウンター
	int nLife;         //体力
	int nPower;        //攻撃力
	int nPowerUp;      //攻撃アップ倍率
	int nPowerUpTime;  //攻撃アップ時間
	int nUpgrade;
	bool bDisp;        //表示するかしないか
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
int Godnum(void);
void Command(void);
bool GetGod(void);
void HitPlayer(int nDamage);
int GetPlayerLife(void);

#endif