#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//マクロ定義
#define MAX_ENEMY (128) //敵の最大数
#define NUM_ENEMY (7) //敵の種類

#define E_SIZE_SMALL (10.0f)
#define E_SIZE_NORMAL (50.0f)
#define E_SIZE_BIG (150.0f)

typedef enum {
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum {
	SMALL = 0,
	NORMAL,
	BIG,
	MAX
}ENEMYSIZE;

//敵構造体の定義
typedef struct {
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 pos2;
	D3DXVECTOR3 rot;
	ENEMYSTATE state; //状態
	ENEMYSIZE size;
	float  fSize;        //敵の大きさ
	int nCounterState;//状態管理カウンター
	int nLife;        //体力
	int nMaxLife;     //最大体力
	int nPower;       //攻撃力
	int nType;        //種類
	int nCounterAnimEnemy; //アニメーションカウンター
	bool bUse;        //使用しているかどうか
	bool bRot;
	float fLengthEnemy;    //対角線の長さ
	float fAngleEnemy;     //対角線の角度
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, ENEMYSIZE size, int nLife, int nPower);
Enemy* GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif
