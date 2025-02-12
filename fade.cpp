#include "fade.h"


//グローバル宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL; //頂点バッファへのポインタ
FADE g_fade;          //フェードの状態
MODE g_modeNext;      //次の画面
D3DXCOLOR g_colorFade; //ポリゴン(フェード)の色

void InitFade(MODE modeNext) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	g_fade = FADE_IN; //フェードイン状態
	g_modeNext = modeNext; //次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); //黒いポリゴン(不透明)にしておく

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

void UninitFade(void) {
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL) {
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

void UpdateFade(void) {
	if (g_fade != FADE_NONE) {
		if (g_fade == FADE_IN) {
			//フェードイン状態
			g_colorFade.a -= 0.02f; //ポリゴンを透明にしていく
			if (g_colorFade.a <= 0.0f) {
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE; //何もしていない状態に
			}
		}
		else if (g_fade == FADE_OUT) {
			//フェードアウト状態
			g_colorFade.a += 0.02f; //ポリゴンを不透明にしていく
			if (g_colorFade.a >= 1.0f) {
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN; //フェードイン状態に
				
				//モード設定(次の画面に移行)
				SetMode(g_modeNext);
			}
		}

		VERTEX_2D* pVtx; //頂点情報へのポインタ

	    //頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

void DrawFade(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//フェードの設定処理
void SetFade(MODE modeNext) {
	g_fade = FADE_OUT; //フェードアウト状態に
	g_modeNext = modeNext; //次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); //黒いポリゴン(透明)にしておく
}

//フェードの取得処理
FADE GetFade(void) {
	return g_fade;
}