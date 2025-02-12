#include "bg.h"

#define NUM_BG (3)

//グローバル宣言
LPDIRECT3DTEXTURE9 g_apTextureBackground[NUM_BG] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL; //頂点バッファへのポインタ
float g_aPosTexV[NUM_BG]; //テクスチャ座標の開始位置(V値)

void InitBackground(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bgGame01.png",
		&g_apTextureBackground[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bgGame02.png",
		&g_apTextureBackground[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bgGame02.png",
		&g_apTextureBackground[2]);

	//テクスチャ座標の開始位置(V値)初期化
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++) {
		g_aPosTexV[nCntBG] = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBackground,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++) {

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG] + 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBackground->Unlock();
}

void UninitBackground(void) {
	//テクスチャの破棄
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++) {
		if (g_apTextureBackground[nCntBG] != NULL) {
			g_apTextureBackground[nCntBG]->Release();
			g_apTextureBackground[nCntBG] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBackground != NULL) {
		g_pVtxBuffBackground->Release();
		g_pVtxBuffBackground = NULL;
	}
}

void UpdateBackground(void) {
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++) {

		g_aPosTexV[nCntBG] -= 0.001f;


		VERTEX_2D* pVtx; //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCntBG * 4);

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPosTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPosTexV[nCntBG] + 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffBackground->Unlock();
	}
}

void DrawBackground(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBackground, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++) {

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBackground[nCntBG]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}