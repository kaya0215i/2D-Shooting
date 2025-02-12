#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

#define NUM_TEXTURE (2)

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TEXTURE] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL; //頂点バッファへのポインタ

void InitTitle(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bgTitle.png",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/start.png",
		&g_pTextureTitle[1]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < NUM_TEXTURE; nCntTitle++) {
		if (nCntTitle == 0) {
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
		}
		else {
			float temp = nCntTitle * 100.0f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(440.0f, 450.0f + temp, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(840.0f, 450.0f + temp, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(440.0f, 530.0f + temp, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(840.0f, 530.0f + temp, 0.0f);
		}

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
	g_pVtxBuffTitle->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM7);
}

void UninitTitle(void) {
	//テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < NUM_TEXTURE; nCntTitle++) {
		if (g_pTextureTitle[nCntTitle] != NULL) {
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL) {
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
	//サウンドの停止
	StopSound();
}

void UpdateTitle(void) {
	// Enter
	if ((GetKeyboardTrigger(DIK_RETURN) == true) || (GetPadTap(XINPUT_GAMEPAD_A) == true)) {
		//モード設定
		SetFade(MODE_GAME);

		PlaySound(SOUND_LABEL_SE_ENTER);
	}
}

void DrawTitle(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TEXTURE; nCntTitle++) {
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}