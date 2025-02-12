#include "ranking.h"
#include "input.h"
#include "score.h"

#define DIGIT (8) //表示する桁数

//ランキングスコア構造体
typedef struct {
	D3DXVECTOR3 pos;
	int nScore;
}RankScore;

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureRank = NULL;      //テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL; //頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL; //頂点バッファへのポインタ

RankScore g_aRankScore[MAX_RANK]; //ランキングスコア情報
int g_nRankUpdate;                //更新ランクNo.
int g_nTimerRanking;              //ランキング画面表示タイマー

int Rankin;

bool isInit = false;

void InitRanking(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking_rank.png",
		&g_pTextureRank);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureRankScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 40,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankScore,
		NULL);

	//スコア関連情報の初期化
	g_nTimerRanking = 0;
	g_nRankUpdate = -1;
	Rankin = 0;

	if (isInit == false) {
		ResetRanking();
		isInit = true;
	}

	SetRanking(GetScore());

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	//ランク表示用
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++) {
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(350.0f, 15.0f + (nCntRank * 130.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(510.0f, 15.0f + (nCntRank * 130.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(350.0f, 135.0f + (nCntRank * 130.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(510.0f, 135.0f + (nCntRank * 130.0f), 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (nCntRank * 0.2));
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (nCntRank * 0.2));
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + (nCntRank * 0.2));
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + (nCntRank * 0.2));

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffRank->Unlock();

	//スコア表示用

	int aPosTexU[DIGIT];


	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++) {
		for (int nCntScore = 0; nCntScore < DIGIT; nCntScore++) {

			aPosTexU[nCntScore] = g_aRankScore[nCntRank].nScore % (int)pow(10, DIGIT - nCntScore) / (int)pow(10, DIGIT - (nCntScore + 1));

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(670.0f + (nCntScore * 40.0f), 15.0f + (nCntRank * 130.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(705.0f + (nCntScore * 40.0f), 15.0f + (nCntRank * 130.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(670.0f + (nCntScore * 40.0f), 135.0f + (nCntRank * 130.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(705.0f + (nCntScore * 40.0f), 135.0f + (nCntRank * 130.0f), 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntScore] * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntScore] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntScore] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntScore] * 0.1f), 1.0f);

			pVtx += 4;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffRankScore->Unlock();

}

void UninitRanking(void) {
	//テクスチャの破棄
	if (g_pTextureRank != NULL) {
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}
	if (g_pTextureRankScore != NULL) {
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL) {
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
	if (g_pVtxBuffRankScore != NULL) {
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}
}

void UpdateRanking(void) {
	g_nTimerRanking++; //表示タイマーを加算

	if (g_nRankUpdate != -1) {
		int Rcol = rand() % 256;
		int Gcol = rand() % 256;
		int Bcol = rand() % 256;

		VERTEX_2D* pVtx;

		g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (4 - Rankin) * 32;

		for (int nCntRank = 0; nCntRank < DIGIT; nCntRank++) {
			pVtx[0].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
			pVtx[1].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
			pVtx[2].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
			pVtx[3].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);

			pVtx += 4;
		}

		g_pVtxBuffRankScore->Unlock();
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true) || (GetPadTap(XINPUT_GAMEPAD_A) == true)) {

	}
}

void DrawRanking(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//ランク用

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRank);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++) {
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
	}

	//スコア用

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankScore);

	for (int nCntRank = 0; nCntRank < 40; nCntRank++) {
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
	}
}

void ResetRanking(void) {
	//ランキングスコア情報の初期化設定
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++) {
		//スコアの左上座標の設定
		g_aRankScore[nCntRank].pos = D3DXVECTOR3(400.0f, 15.0f + (nCntRank * 130.0f), 0.0f);;

		//初期スコアの設定
		g_aRankScore[nCntRank].nScore = 0;
	}
}

void SetRanking(int nScore) {
	int nTemp;

	if (nScore > g_aRankScore[4].nScore) {
		g_aRankScore[4].nScore = nScore;

		for (int nCntRank = 0; nCntRank < 4; nCntRank++) {
			if (g_aRankScore[4 - nCntRank].nScore > g_aRankScore[4 - (nCntRank + 1)].nScore) {
				nTemp = g_aRankScore[4 - (nCntRank + 1)].nScore;
				g_aRankScore[4 - (nCntRank + 1)].nScore = g_aRankScore[4 - nCntRank].nScore;
				g_aRankScore[4 - nCntRank].nScore = nTemp;

				Rankin++;
			}
			else {
				break;
			}
		}
		g_nRankUpdate = 0;
	}
}
