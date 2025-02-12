#include "score.h"

#define DIGIT (8) //表示する桁数

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_posScore; //スコアの位置
int g_nScore;           //スコアの値

void InitScore(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(970.0f, 15.0f, 0.0f);
	g_nScore = 0;                            

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < 8; nCntScore++) {
		//頂点座標の設定
		pVtx[0].pos.x = g_posScore.x + (nCntScore * 35.0f);
		pVtx[0].pos.y = g_posScore.y;
		pVtx[0].pos.z = g_posScore.z;
		pVtx[1].pos.x = (g_posScore.x + 35.0f) + (nCntScore * 35.0f);
		pVtx[1].pos.y = g_posScore.y;
		pVtx[1].pos.z = g_posScore.z;
		pVtx[2].pos.x = g_posScore.x + (nCntScore * 35.0f);
		pVtx[2].pos.y = g_posScore.y + 70.0f;
		pVtx[2].pos.z = g_posScore.z;
		pVtx[3].pos.x = (g_posScore.x + 35.0f) + (nCntScore * 35.0f);
		pVtx[3].pos.y = g_posScore.y + 70.0f;
		pVtx[3].pos.z = g_posScore.z;

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

void UninitScore(void) {
	//テクスチャの破棄
	if (g_pTextureScore != NULL) {
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL) {
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

void UpdateScore(void) {
	float speed = ((rand() % 9 + 1) / 500.0f) + 1.0f;
	int Rcol = rand() % 256;
	int Gcol = rand() % 256;
	int Bcol = rand() % 256;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < DIGIT; nCntScore++) {
		pVtx[0].pos.x = (g_posScore.x + (nCntScore * 35.0f)) * speed;
		pVtx[0].pos.y = g_posScore.y * speed;
		pVtx[0].pos.z = g_posScore.z;
		pVtx[1].pos.x = ((g_posScore.x + 35.0f) + (nCntScore * 35.0f)) * speed;
		pVtx[1].pos.y = g_posScore.y * speed;
		pVtx[1].pos.z = g_posScore.z;
		pVtx[2].pos.x = (g_posScore.x + (nCntScore * 35.0f)) * speed;
		pVtx[2].pos.y = (g_posScore.y + 70.0f) * speed;
		pVtx[2].pos.z = g_posScore.z;
		pVtx[3].pos.x = ((g_posScore.x + 35.0f) + (nCntScore * 35.0f)) * speed;
		pVtx[3].pos.y = (g_posScore.y + 70.0f) * speed;
		pVtx[3].pos.z = g_posScore.z;

		pVtx[0].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
		pVtx[1].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
		pVtx[2].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
		pVtx[3].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

void DrawScore(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < 8; nCntScore++) {
		//スコアの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

void SetScore(int nScore) {
	int aPosTexU[DIGIT]; //各桁の数字を格納

	g_nScore = nScore;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < DIGIT; nCntScore++) {
		aPosTexU[nCntScore] = g_nScore % (int)pow(10, DIGIT - nCntScore) / (int)pow(10, DIGIT - (nCntScore + 1));

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntScore] * 0.1f), 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

void AddScore(int nValue) {
	int aPosTexU[DIGIT];

	UpdateScore();

	g_nScore += nValue;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < DIGIT; nCntScore++) {
		aPosTexU[nCntScore] = g_nScore % (int)pow(10, DIGIT - nCntScore) / (int)pow(10, DIGIT - (nCntScore + 1));

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntScore] * 0.1f), 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める

	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

void ResetCol(void) {
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < 8; nCntScore++) {
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}

	g_pVtxBuffScore->Unlock();
}

int GetScore(void) {
	return g_nScore;
}