#include "enemylife.h"
#include "enemy.h"

#define MAX_LIFE (50)

typedef struct {
	D3DXVECTOR3 pos;  //位置
	int nLife;        //痔命
	bool bUse;        //使用しているかどうか
	int Enemy;
}Life;

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureEnemyLife[2] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyLife = NULL; //頂点バッファへのポインタ
Life g_life[MAX_LIFE];

void InitEnemyLife(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/LifeFrame.png",
		&g_pTextureEnemyLife[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemyLifeBar.png",
		&g_pTextureEnemyLife[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyLife,
		NULL);

	for (int nCntLifeBar = 0; nCntLifeBar < MAX_LIFE; nCntLifeBar++) {
		g_life[nCntLifeBar].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_life[nCntLifeBar].bUse = false;
		g_life[nCntLifeBar].Enemy = 0;
	}

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLifeBar = 0; nCntLifeBar < MAX_LIFE; nCntLifeBar++) {
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(250.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(250.0f, 35.0f, 0.0f);

		pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(250.0f, 0.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(250.0f, 35.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 8;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemyLife->Unlock();
}

void UninitEnemyLife(void) {
	//テクスチャの破棄
	for (int nCntLifeBar = 0; nCntLifeBar < 2; nCntLifeBar++) {
		if (g_pTextureEnemyLife[nCntLifeBar] != NULL) {
			g_pTextureEnemyLife[nCntLifeBar]->Release();
			g_pTextureEnemyLife[nCntLifeBar] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemyLife != NULL) {
		g_pVtxBuffEnemyLife->Release();
		g_pVtxBuffEnemyLife = NULL;
	}
}

void UpdateEnemyLife(void) {
	Enemy* pEnemy;

	for (int nCntLifeBar = 0; nCntLifeBar < MAX_LIFE; nCntLifeBar++) {
		if (g_life[nCntLifeBar].bUse == true) {
			pEnemy = GetEnemy();

			pEnemy += g_life[nCntLifeBar].Enemy;

			if (pEnemy->bUse == false) {
				g_life[nCntLifeBar].bUse = false;

				continue;
			}

			g_life[nCntLifeBar].pos = pEnemy->pos;
			g_life[nCntLifeBar].nLife = pEnemy->nLife;

			float LifeposX = 50.0f;
			float LifeposY = pEnemy->fSize;

			float fEnemyLifePos = (pEnemy->nMaxLife - g_life[nCntLifeBar].nLife) * (100.0f / pEnemy->nMaxLife);
			float fEnemyLifeTex = (pEnemy->nMaxLife - g_life[nCntLifeBar].nLife) * (1.0f / pEnemy->nMaxLife);

			if (fEnemyLifePos >= 100.0f) {
				fEnemyLifePos = 100.0f;
			}
			if (fEnemyLifeTex >= 1.0f) {
				fEnemyLifeTex = 1.0f;
			}

			VERTEX_2D* pVtx; //頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntLifeBar * 8;

			//頂点座標の設定
			pVtx[0].pos.x = g_life[nCntLifeBar].pos.x - LifeposX;
			pVtx[0].pos.y = g_life[nCntLifeBar].pos.y + LifeposY;

			pVtx[1].pos.x = g_life[nCntLifeBar].pos.x + 100.0f - LifeposX;
			pVtx[1].pos.y = g_life[nCntLifeBar].pos.y + LifeposY;

			pVtx[2].pos.x = g_life[nCntLifeBar].pos.x - LifeposX;
			pVtx[2].pos.y = g_life[nCntLifeBar].pos.y + 15.0f + LifeposY;

			pVtx[3].pos.x = g_life[nCntLifeBar].pos.x + 100.0f - LifeposX;
			pVtx[3].pos.y = g_life[nCntLifeBar].pos.y + 15.0f + LifeposY;


			pVtx[4].pos.x = g_life[nCntLifeBar].pos.x + fEnemyLifePos - LifeposX;
			pVtx[4].pos.y = g_life[nCntLifeBar].pos.y + LifeposY;

			pVtx[5].pos.x = g_life[nCntLifeBar].pos.x + 100.0f - LifeposX;
			pVtx[5].pos.y = g_life[nCntLifeBar].pos.y + LifeposY;

			pVtx[6].pos.x = g_life[nCntLifeBar].pos.x + fEnemyLifePos - LifeposX;
			pVtx[6].pos.y = g_life[nCntLifeBar].pos.y + 15.0f + LifeposY;

			pVtx[7].pos.x = g_life[nCntLifeBar].pos.x + 100.0f - LifeposX;
			pVtx[7].pos.y = g_life[nCntLifeBar].pos.y + 15.0f + LifeposY;

			//テクスチャ座標の設定
			pVtx[4].tex = D3DXVECTOR2(0.0f + fEnemyLifeTex, 0.0f);
			pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[6].tex = D3DXVECTOR2(0.0f + fEnemyLifeTex, 1.0f);
			pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//頂点バッファをアンロックする
			g_pVtxBuffEnemyLife->Unlock();
		}
	}
}

void DrawEnemyLife(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemyLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntLifeBar = 0; nCntLifeBar < MAX_LIFE; nCntLifeBar++) {
		if (g_life[nCntLifeBar].bUse == true) {
			pDevice->SetTexture(0, g_pTextureEnemyLife[0]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLifeBar * 8, 2);

			pDevice->SetTexture(0, g_pTextureEnemyLife[1]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntLifeBar + 1) * 8 ) - 4, 2);
		}
	}
}

void SetEnemyLife(int nNum) {
	for (int nCntLifeBar = 0; nCntLifeBar < MAX_LIFE; nCntLifeBar++) {
		if (g_life[nCntLifeBar].bUse == false) {
			g_life[nCntLifeBar].bUse = true;
			g_life[nCntLifeBar].Enemy = nNum;

			break;
		}
	}
}