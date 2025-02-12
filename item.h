#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

typedef enum {
	ITEM_HEAL = 0,
	ITEM_POWER,
	ITEM_RAPID,

	ITEM_MAX
}ITEMTYPE;

typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	ITEMTYPE type;
	int nType;
	bool bUse;
}Item;

//プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type, bool bRondomItem);
void HitItem(int nType);

#endif