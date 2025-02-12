#include "explosion.h"

//マクロ定義
#define MAX_EXPLOSION (512) //爆発の最大数

//爆発構造体の定義
typedef struct {
	D3DXVECTOR3 pos;  //位置
	D3DXCOLOR col;    //色
	int nCounterAnim; //アニメーションカウンター
	int nPatternAnim; //アニメーションパターン
	bool bUse;        //使用しているかどうか
}Explosion;

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];                    //爆発の情報
float g_fLengthExplosion;    //対角線の長さ
float g_fAngleExplosion;     //対角線の角度

void InitExplosion(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	//爆発の情報の初期化
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++) {
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//対角線の長さを算出する
	g_fLengthExplosion = sqrtf(50.0f * 50.0f + 50.0f * 50.0f) / 2.0f;

	//対角線の角度を算出する
	g_fAngleExplosion = atan2f(50.0f, 50.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++) {
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

void UninitExplosion(void) {
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL) {
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL) {
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

void UpdateExplosion(void) {
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++) {
		if (g_aExplosion[nCntExplosion].bUse == true) { //爆発が使用されている

			g_aExplosion[nCntExplosion].nCounterAnim++; //カウンターを加算する

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 5) == 0) {

				VERTEX_2D* pVtx; //頂点情報へのポインタ

				//頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

				pVtx += (nCntExplosion * 4);

				//パターンNo.を更新する
				g_aExplosion[nCntExplosion].nPatternAnim = 1 + (g_aExplosion[nCntExplosion].nCounterAnim) / 5;

				if ((g_aExplosion[nCntExplosion].nPatternAnim) >= 8) {
					g_aExplosion[nCntExplosion].bUse = false; //使用してない状態にする
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim) * 0.125f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aExplosion[nCntExplosion].nPatternAnim) * 0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim) * 0.125f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aExplosion[nCntExplosion].nPatternAnim) * 0.125f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffExplosion->Unlock();
			}
		}
	}
}

void DrawExplosion(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++) {
		if (g_aExplosion[nCntExplosion].bUse == true) { //爆発が使用されている
			//爆発の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col) {

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++) {
		if (g_aExplosion[nCntExplosion].bUse == false) {

			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true; //使用している状況にする

			VERTEX_2D* pVtx; //頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntExplosion * 4);

			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

			//頂点座標の設定
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[0].pos.z = g_aExplosion[nCntExplosion].pos.z;
			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(-D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(-D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[1].pos.z = g_aExplosion[nCntExplosion].pos.z;
			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[2].pos.z = g_aExplosion[nCntExplosion].pos.z;
			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[3].pos.z = g_aExplosion[nCntExplosion].pos.z;

			//頂点カラーの設定
			pVtx[0].col = g_aExplosion[nCntExplosion].col;
			pVtx[1].col = g_aExplosion[nCntExplosion].col;
			pVtx[2].col = g_aExplosion[nCntExplosion].col;
			pVtx[3].col = g_aExplosion[nCntExplosion].col;

			//頂点バッファをアンロックする
			g_pVtxBuffExplosion->Unlock();

			break;
		}
	}
}
