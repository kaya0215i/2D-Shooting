#include "pause.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

#define NUM_TEXTURE (4)

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_TEXTURE] = {};      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL; //頂点バッファへのポインタ
int g_pauseMenu;

void InitPause(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	g_pauseMenu = 1;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pausemenu.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/continue.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/retry.png",
		&g_pTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/retire.png",
		&g_pTexturePause[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < NUM_TEXTURE; nCntPause++) {
		if (nCntPause == 0) {
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
		}
		else {
			float temp = nCntPause * 100.0f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(540.0f, 170.0f + temp, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(740.0f, 170.0f + temp, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(540.0f, 250.0f + temp, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(740.0f, 250.0f + temp, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

void UninitPause(void) {
	for (int nCntPause = 0; nCntPause < NUM_TEXTURE; nCntPause++) {
		//テクスチャの破棄
		if (g_pTexturePause[nCntPause] != NULL) {
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL) {
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	StopSound();
}

void UpdatePause(void) {

	if ((GetKeyboardTrigger(DIK_W) == true) || (GetPadTap(XINPUT_GAMEPAD_DPAD_UP) == true)) {
		g_pauseMenu--;
		if (g_pauseMenu < 1) {
			g_pauseMenu = 3;
		}
	}
	else if ((GetKeyboardTrigger(DIK_S) == true) || (GetPadTap(XINPUT_GAMEPAD_DPAD_DOWN) == true)) {
		g_pauseMenu++;
		if (g_pauseMenu > 3) {
			g_pauseMenu = 1;
		}
	}

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < NUM_TEXTURE; nCntPause++) {
		//頂点カラーの設定
		if (nCntPause == 0) {
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}


		
		pVtx += 4;
	}

	pVtx -= 4 * NUM_TEXTURE;;

	pVtx += g_pauseMenu * 4;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	// Enter
	if ((GetKeyboardTrigger(DIK_RETURN) == true) || (GetPadTap(XINPUT_GAMEPAD_A) == true)) {
		switch (g_pauseMenu) {
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY:
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:
			SetGameState(GAMESTATE_END);
			SetFade(MODE_TITLE);
			SetEnablePause(false);
			break;
		}

		PlaySound(SOUND_LABEL_SE_ENTER);
	}
}

void DrawPause(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < NUM_TEXTURE; nCntPause++) {
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}