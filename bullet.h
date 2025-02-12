#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//マクロ定義
#define MAX_BULLET (512) //弾の最大数

typedef enum {
	//BULLETTYPE_PLAYER = 0,
	//BULLETTYPE_ENEMY,
	BULLETTYPE_NORMAL = 0,
	BULLETTYPE_HOMING,
	BULLETTYPE_MAX
}BULLETTYPE;

//弾構造体の定義
typedef struct {
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nLife;        //痔命
	BULLETTYPE type;
	bool bUse;        //使用しているかどうか
	bool isPlayer;    //プレイヤーの弾かどうか
	float fLengthBullet; //対角線の長さ
	float fAngleBullet;  //対角線の角度
	int nDamage;
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, float fLength, int nDamage, BULLETTYPE type, bool isPlayer);
Bullet* GetBullet(void);

#endif