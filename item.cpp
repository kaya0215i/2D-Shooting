#include "item.h"
#include "player.h"

#define MAX_ITEM (10)
#define ALL_ITEM (3)

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureItem[ALL_ITEM] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL; //頂点バッファへのポインタ
Item g_item[MAX_ITEM];

float g_fLengthItem;
float g_fAngleItem;

void InitItem(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&g_pTextureItem[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item001.png",
		&g_pTextureItem[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item002.png",
		&g_pTextureItem[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	srand((unsigned int)time(NULL));

	g_fLengthItem = sqrtf(50.0f * 50.0f + 50.0f * 50.0f) / 2.0f;
	g_fAngleItem = atan2f(50.0f, 50.0f);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++) {
		g_item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_item[nCntItem].type = ITEM_MAX;
		g_item[nCntItem].nType = 0;
		g_item[nCntItem].bUse = false;
	}

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++) {
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

void UninitItem(void) {
	//テクスチャの破棄
	for (int nCntItem = 0; nCntItem < ALL_ITEM; nCntItem++) {
		if (g_pTextureItem[nCntItem] != NULL) {
			g_pTextureItem[nCntItem]->Release();
			g_pTextureItem[nCntItem] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL) {
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

void UpdateItem(void) {
	Player* pPlayer;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++) {
		if (g_item[nCntItem].bUse == true) {
			g_item[nCntItem].pos += g_item[nCntItem].move;

			VERTEX_2D* pVtx;
			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntItem * 4);

			pVtx[0].pos.x = g_item[nCntItem].pos.x + sinf(D3DX_PI + g_fAngleItem) * g_fLengthItem;
			pVtx[0].pos.y = g_item[nCntItem].pos.y + cosf(D3DX_PI + g_fAngleItem) * g_fLengthItem;
			pVtx[0].pos.z = g_item[nCntItem].pos.z;
			pVtx[1].pos.x = g_item[nCntItem].pos.x + sinf(-D3DX_PI - g_fAngleItem) * g_fLengthItem;
			pVtx[1].pos.y = g_item[nCntItem].pos.y + cosf(-D3DX_PI - g_fAngleItem) * g_fLengthItem;
			pVtx[1].pos.z = g_item[nCntItem].pos.z;
			pVtx[2].pos.x = g_item[nCntItem].pos.x + sinf(-g_fAngleItem) * g_fLengthItem;
			pVtx[2].pos.y = g_item[nCntItem].pos.y + cosf(-g_fAngleItem) * g_fLengthItem;
			pVtx[2].pos.z = g_item[nCntItem].pos.z;
			pVtx[3].pos.x = g_item[nCntItem].pos.x + sinf(g_fAngleItem) * g_fLengthItem;
			pVtx[3].pos.y = g_item[nCntItem].pos.y + cosf(g_fAngleItem) * g_fLengthItem;
			pVtx[3].pos.z = g_item[nCntItem].pos.z;

			g_pVtxBuffItem->Unlock();

			pPlayer = GetPlayer();

			if (pPlayer->pos.x - SIZE_X / 4 <= g_item[nCntItem].pos.x && g_item[nCntItem].pos.x <= pPlayer->pos.x + SIZE_X / 4 &&
				pPlayer->pos.y - SIZE_Y / 4 <= g_item[nCntItem].pos.y && g_item[nCntItem].pos.y <= pPlayer->pos.y + SIZE_Y / 4) {

				HitItem(g_item[nCntItem].nType);

				g_item[nCntItem].bUse = false;
			}

			//画面外に出た
			if ((g_item[nCntItem].pos.x <= 0.0f) || (1280.0f <= g_item[nCntItem].pos.x) ||
				(g_item[nCntItem].pos.y <= 0.0f) || (720.0f <= g_item[nCntItem].pos.y)) {
				g_item[nCntItem].bUse = false;
			}
		}
	}
}

void DrawItem(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	int nCnt = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++) {
		if (g_item[nCntItem].bUse == true) {
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem[g_item[nCntItem].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}
}

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type, bool bRondomItem) {
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++) {
		if (g_item[nCntItem].bUse == false) {
			g_item[nCntItem].pos = pos;
			g_item[nCntItem].move = move;
			
			if (bRondomItem == true) {
				int nRandom = rand() % 100 + 1;

				if (0 < nRandom && nRandom < 60) {
					g_item[nCntItem].nType = 0;
				}
				else if (60 <= nRandom && nRandom < 90) {
					g_item[nCntItem].nType = 1;
				}
				else if (90 <= nRandom && nRandom <= 100) {
					g_item[nCntItem].nType = 2;
				}
			}
			else if (bRondomItem == false) {
				switch (type) {
				case ITEM_HEAL:
					g_item[nCntItem].nType = 0;
					break;

				case ITEM_POWER:
					g_item[nCntItem].nType = 1;
					break;

				case ITEM_RAPID:
					g_item[nCntItem].nType = 2;
					break;
				}
			}

			VERTEX_2D* pVtx;

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntItem * 4);

			pVtx[0].pos.x = g_item[nCntItem].pos.x + sinf(D3DX_PI + g_fAngleItem) * g_fLengthItem;
			pVtx[0].pos.y = g_item[nCntItem].pos.y + cosf(D3DX_PI + g_fAngleItem) * g_fLengthItem;
			pVtx[0].pos.z = g_item[nCntItem].pos.z;
			pVtx[1].pos.x = g_item[nCntItem].pos.x + sinf(-D3DX_PI - g_fAngleItem) * g_fLengthItem;
			pVtx[1].pos.y = g_item[nCntItem].pos.y + cosf(-D3DX_PI - g_fAngleItem) * g_fLengthItem;
			pVtx[1].pos.z = g_item[nCntItem].pos.z;
			pVtx[2].pos.x = g_item[nCntItem].pos.x + sinf(-g_fAngleItem) * g_fLengthItem;
			pVtx[2].pos.y = g_item[nCntItem].pos.y + cosf(-g_fAngleItem) * g_fLengthItem;
			pVtx[2].pos.z = g_item[nCntItem].pos.z;
			pVtx[3].pos.x = g_item[nCntItem].pos.x + sinf(g_fAngleItem) * g_fLengthItem;
			pVtx[3].pos.y = g_item[nCntItem].pos.y + cosf(g_fAngleItem) * g_fLengthItem;
			pVtx[3].pos.z = g_item[nCntItem].pos.z;

			g_pVtxBuffItem->Unlock();

			g_item[nCntItem].bUse = true;

			break;
		}
	}
}

void HitItem(int nType) {
	Player* pPlayer = GetPlayer();

	if (nType == 0) {
		pPlayer->nLife += 30;
		if (pPlayer->nLife > 100) {
			pPlayer->nLife = 100;
		}
	}
	else if (nType == 1) {
		pPlayer->nPowerUp = 2;
		pPlayer->nPowerUpTime = 100;
	}
	else if (nType == 2) {
		pPlayer->nUpgrade++;
		if (pPlayer->nUpgrade > 3) {
			pPlayer->nUpgrade = 3;
		}
	}
}
