#include "input.h"
#include "bullet.h"
#include "enemy.h"

//マクロ定義
#define NUM_KEY_MAX (256) //キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_plnput = NULL;             //DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL; //入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];              //キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];       //キーボードのトリガー情報
XINPUT_STATE g_joyKeyState; //ジョイパッドのプレス情報
XINPUT_STATE g_joyTapState;
XINPUT_VIBRATION vibration;
bool g_Lsticktri;
int nCntVibration;

//キーボードの初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd) {
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_plnput, NULL))) {
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_plnput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL))) {
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//キーボードの終了処理
void UninitKeyboard(void) {
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL) {
		g_pDevKeyboard->Unacquire(); //キーボードへのアクセス権を破棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_plnput != NULL) {
		g_plnput->Release();
		g_plnput = NULL;
	}
}

//キーボードの更新処理
void UpdateKeyboard(void) {
	BYTE aKeyState[NUM_KEY_MAX]; //キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0]))) {
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++) {
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードプレス情報を保存
		}
	}
	else {
		g_pDevKeyboard->Acquire(); //キーボードへのアクセス権を獲得
	}
}

//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey) {
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey) {
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//ジョイパッドの初期化処理
HRESULT InitJoypad(void) {
	//メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joyTapState, 0, sizeof(XINPUT_STATE));
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//ジョイパッドの終了処理
void UninitJoypad(void) {
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//ジョイパッドの更新処理
void UpdateJoypad(void) {
	XINPUT_STATE joyKeyState;
	DWORD dwResult = XInputGetState(0, &joyKeyState);

	if (dwResult == ERROR_SUCCESS)
	{
		// Controller is connected
		g_joyTapState.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons) & joyKeyState.Gamepad.wButtons;
		g_joyKeyState.Gamepad = joyKeyState.Gamepad;

		// Zero value if thumbsticks are within the dead zone
		if ((g_joyKeyState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			g_joyKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(g_joyKeyState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				g_joyKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			g_joyKeyState.Gamepad.sThumbLX = 0;
			g_joyKeyState.Gamepad.sThumbLY = 0;
		}

		if (nCntVibration > 0) {
			nCntVibration--;
		}
		if (nCntVibration == 0) {
			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;
			XInputSetState(0, &vibration);
		}
	}
	else
	{
		// Controller is not connected
	}

	
}

bool GetPadPress(int pad) {
	return (g_joyKeyState.Gamepad.wButtons & pad) ? true : false;
}

bool GetPadTap(int pad) {
	return (g_joyTapState.Gamepad.wButtons & pad) ? true : false;
}

bool GetPadLstickX(int pad) {
	return (g_joyKeyState.Gamepad.sThumbLX <= pad) ? true : false;
}

bool GetPadLstickY(int pad) {
	return (g_joyKeyState.Gamepad.sThumbLY <= pad) ? true : false;
}

bool GetPadRstickX(int pad) {
	return (g_joyKeyState.Gamepad.sThumbRX <= pad) ? true : false;
}

bool GetPadRstickY(int pad) {
	return (g_joyKeyState.Gamepad.sThumbRY <= pad) ? true : false;
}

bool GetPadLtrigger(void) {
	return (g_joyKeyState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}

bool GetPadRtrigger(void) {
	return (g_joyKeyState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}
void OnPadVibration(void) {
	nCntVibration = 3;

	vibration.wLeftMotorSpeed = 65535;
	vibration.wRightMotorSpeed = 65535;
	XInputSetState(0, &vibration);
}