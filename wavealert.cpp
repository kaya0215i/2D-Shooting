#include "wavealert.h"
#include "sound.h"
#include "game.h"

#define NUM_WAVE (5)

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureWaveAlert[NUM_WAVE] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWaveAlert = NULL; //頂点バッファへのポインタ

float Acol;
bool Up;

void InitWaveAlert(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wave001.png",
		&g_pTextureWaveAlert[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wave002.png",
		&g_pTextureWaveAlert[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wave003.png",
		&g_pTextureWaveAlert[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wave004.png",
		&g_pTextureWaveAlert[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wave005.png",
		&g_pTextureWaveAlert[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_WAVE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWaveAlert,
		NULL);

	Acol = 0.0f;
	Up = true;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWaveAlert->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < NUM_WAVE; nCntWave++) {
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
	g_pVtxBuffWaveAlert->Unlock();
}

void UninitWaveAlert(void) {
	//テクスチャの破棄
	for (int nCntWave = 0; nCntWave < NUM_WAVE; nCntWave++) {
		if (g_pTextureWaveAlert[nCntWave] != NULL) {
			g_pTextureWaveAlert[nCntWave]->Release();
			g_pTextureWaveAlert[nCntWave] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWaveAlert != NULL) {
		g_pVtxBuffWaveAlert->Release();
		g_pVtxBuffWaveAlert = NULL;
	}

	StopSound();
}

void UpdateWaveAlert(void) {
	Wave* pWave = GetWave();

	for (int nCntWave = 1; nCntWave < NUM_WAVE + 1; nCntWave++) {
		if (nCntWave == pWave->g_nWave) {

			if (Up == true) {
				Acol += 0.1f;
				if (Acol >= 1.0f) {
					Acol = 1.0f;
					Up = false;
				}
			}
			else if (Up == false) {
				Acol -= 0.1f;
				if (Acol <= 0.0f) {
					Acol = 0.0f;
					Up = true;
				}
			}

			VERTEX_2D* pVtx; //頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffWaveAlert->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += ((nCntWave -1) * 4);

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Acol);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Acol);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Acol);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Acol);

			//頂点バッファをアンロックする
			g_pVtxBuffWaveAlert->Unlock();

			break;
		}
	}
}

void DrawWaveAlert(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWaveAlert, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntWave = 0; nCntWave < NUM_WAVE; nCntWave++) {
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWaveAlert[nCntWave]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWave * 4, 2);
	}
}

void ColorReset(void) {
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWaveAlert->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < NUM_WAVE; nCntWave++) {
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWaveAlert->Unlock();
}
