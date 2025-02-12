#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//マクロ定義
#define MAX_EFFECT (4096) //エフェクトの最大数

//エフェクト構造体の定義
typedef struct {
	D3DXVECTOR3 pos;  //位置
	D3DXCOLOR col;    //色
	float fRadius;    //半径(大きさ)
	int nLife;        //表示時間
	bool bUse;        //使用しているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, bool rainbow);
Effect* GetEffect(void);

#endif
