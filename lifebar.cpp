#include "lifebar.h"
#include "player.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureLifebar[2] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLifebar = NULL; //頂点バッファへのポインタ

bool DrawLife;

void InitLifebar(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/LifeFrame.png",
		&g_pTextureLifebar[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/LifeBar.png",
		&g_pTextureLifebar[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLifebar,
		NULL);

	DrawLife = true;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLifeBar = 0; nCntLifeBar < 2; nCntLifeBar++) {
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(100.0f, 15.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(600.0f, 15.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(100.0f, 85.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600.0f, 85.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLifebar->Unlock();
}

void UninitLifebar(void) {
	//テクスチャの破棄
	for (int nCntLifeBar = 0; nCntLifeBar < 2; nCntLifeBar++) {
		if (g_pTextureLifebar[nCntLifeBar] != NULL) {
			g_pTextureLifebar[nCntLifeBar]->Release();
			g_pTextureLifebar[nCntLifeBar] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLifebar != NULL) {
		g_pVtxBuffLifebar->Release();
		g_pVtxBuffLifebar = NULL;
	}
}

void UpdateLifebar(void) {
	Player* pPlayer;
	pPlayer = GetPlayer();

	if (pPlayer->nLife <= 0) {
		DrawLife = false;
	}

	float fPlayerLifePos = (100 - GetPlayerLife()) * 1.0f;
	float fPlayerLifeTex = (100 - GetPlayerLife()) * 0.01f;

	float LifeposX = 50.0f;
	float LifeposY = 50.0f;

	if (fPlayerLifePos >= 100.0f) {
		fPlayerLifePos = 100.0f;
	}
	if (fPlayerLifeTex >= 1.0f) {
		fPlayerLifeTex = 1.0f;
	}

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLifebar->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = pPlayer->pos.x - LifeposX;
	pVtx[0].pos.y = pPlayer->pos.y + LifeposY;

	pVtx[1].pos.x = pPlayer->pos.x + 100.0f - LifeposX;
	pVtx[1].pos.y = pPlayer->pos.y + LifeposY;

	pVtx[2].pos.x = pPlayer->pos.x - LifeposX;
	pVtx[2].pos.y = pPlayer->pos.y + 15.0f + LifeposY;

	pVtx[3].pos.x = pPlayer->pos.x + 100.0f - LifeposX;
	pVtx[3].pos.y = pPlayer->pos.y + 15.0f + LifeposY;


	pVtx[4].pos.x = pPlayer->pos.x + fPlayerLifePos - LifeposX;
	pVtx[4].pos.y = pPlayer->pos.y + LifeposY;

	pVtx[5].pos.x = pPlayer->pos.x + 100.0f - LifeposX;
	pVtx[5].pos.y = pPlayer->pos.y + LifeposY;

	pVtx[6].pos.x = pPlayer->pos.x + fPlayerLifePos - LifeposX;
	pVtx[6].pos.y = pPlayer->pos.y + 15.0f + LifeposY;

	pVtx[7].pos.x = pPlayer->pos.x + 100.0f - LifeposX;
	pVtx[7].pos.y = pPlayer->pos.y + 15.0f + LifeposY;

	//テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f + fPlayerLifeTex, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f + fPlayerLifeTex, 1.0f);
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
	g_pVtxBuffLifebar->Unlock();
}

void DrawLifebar(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLifebar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (DrawLife == true) {
		for (int nCntLifeBar = 0; nCntLifeBar < 2; nCntLifeBar++) {
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureLifebar[nCntLifeBar]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLifeBar * 4, 2);
		}
	}
}
