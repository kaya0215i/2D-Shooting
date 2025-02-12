#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include <time.h>
#include "d3dx9.h"  //描画処理に必要
#include "dinput.h" //入力処理に必要
#include "Xinput.h" //ジョイパッド処理に必要
#include "xaudio2.h"

//ライブラリのリンク
#pragma comment(lib, "d3d9.lib")   //描画処理に必要
#pragma comment(lib, "d3dx9.lib")  //[d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib") //DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")  //システム時刻取得に必要
#pragma comment(lib, "dinput8.lib") //入力処理に必要
#pragma comment(lib, "xinput.lib") //ジョイパッド処理に必要

//マクロ定義
#define SCREEN_WIDTH (1280)              //ウィンドウの幅
#define SCREEN_HEIGHT (720)              //ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define CLASS_NAME "WindowClass"         //ウィンドウクラスの名前
#define WINDOW_NAME "EMOJI SHOOTING" //ウィンドウの名前
#define ID_BUTTON_FINISH (215)           //終了ボタンのID
#define DIRECTINPUT__VERSION (0x0800) //ビルド時の警告対処用マクロ

//画面(モード)の種類
typedef enum {
	MODE_TITLE = 0, //タイトル画面
	MODE_GAME,      //ゲーム画面
	MODE_RESULT,    //リザルト画面
	MODE_MAX
}MODE;

//頂点情報[2D]の構造体の定義
typedef struct {
	D3DXVECTOR3 pos; //頂点座標
	float rhw;       //座標返還用係数(1.0f で固定)
	D3DCOLOR col;    //頂点カラー
	D3DXVECTOR2 tex;    //テクスチャ座標
}VERTEX_2D;

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lPalam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif