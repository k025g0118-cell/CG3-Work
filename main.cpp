#include <Windows.h>
#include<cstdint>
#include<string>
#include<format>
//ファイルやディレクトリに関する操作を行うライブラリ
#include<filesystem>
//ファイルに書いたり読んだりするライブラリ
#include<fstream>
//時間を扱うライブラリ
#include<chrono>

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dbghelp.h>
#include<strsafe.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"Dbghelp.lib")

//--------------------------------
// 関数
//--------------------------------

// string -> wstring
std::wstring ConvertString(const std::string& str) {

	if (str.empty()) {
		return std::wstring();
	}

	int sizeNeeded = MultiByteToWideChar(CP_UTF8,0,str.c_str(),-1,nullptr,0);

	if (sizeNeeded == 0) {
		return std::wstring();
	}

	std::wstring result(sizeNeeded, 0);

	MultiByteToWideChar(CP_UTF8,0,str.c_str(),-1,&result[0],sizeNeeded);

	return result;
}

// wstring -> string
std::string ConvertString(const std::wstring& str) {

	if (str.empty()) {
		return std::string();
	}

	int sizeNeeded = WideCharToMultiByte(CP_UTF8,0,str.c_str(),-1,nullptr,0,nullptr,nullptr);

	if (sizeNeeded == 0) {
		return std::string();
	}

	std::string result(sizeNeeded, 0);

	WideCharToMultiByte(CP_UTF8,0,str.c_str(),-1,&result[0],sizeNeeded,nullptr,nullptr);

	return result;
}

// string版ログ
void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

// wstring版ログ
void Log(const std::wstring& message) {
	Log(ConvertString(message));
}

//ログをファイルに書き出す
void Log(std::ostream& os, const std::string& message) {
	os << message << std::endl;
	OutputDebugStringA(message.c_str());
}

static LONG WINAPI ExportDump(EXCEPTION_POINTERS* exception) {

	// 時刻を取得して、時刻を名前に入れたファイルを作成。Dumpsディレクトリ以下に出力
	SYSTEMTIME time;
	GetLocalTime(&time);
	wchar_t filePath[MAX_PATH] = { 0 };
	CreateDirectory(L"./Dumps", nullptr);
	StringCchPrintfW(filePath, MAX_PATH, L"./Dumps/%04d-%02d%02d-%02d%02d.dmp",
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);

	HANDLE dumpFileHandle = CreateFile(
		filePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		0,
		0
	);

	// processId（このexeのID）とクラッシュ（例外）の発生したthreadIdを取得
	DWORD processId = GetCurrentProcessId();
	DWORD threadId = GetCurrentThreadId();

	// 設定情報を入力
	MINIDUMP_EXCEPTION_INFORMATION minidumpInformation{ 0 };
	minidumpInformation.ThreadId = threadId;
	minidumpInformation.ExceptionPointers = exception;
	minidumpInformation.ClientPointers = TRUE;

	// Dumpを出力。MiniDumpNormalは最低限の情報を出力するフラグ
	MiniDumpWriteDump(GetCurrentProcess(), processId, dumpFileHandle, MiniDumpNormal, &minidumpInformation, nullptr, nullptr);

	// 他に例外がいないなら例外ハンドラがあれば実行。通常はプロセスを終了する
	return EXCEPTION_EXECUTE_HANDLER;
}

//---------------------------------

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam) {
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {

		// ウィンドウが破壊された
	case WM_DESTROY:

		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);

		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//Windowsアプリへのインポート(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	//誰も捕捉しなかった場合に(Unhandled)、捕捉する関数を登録
	SetUnhandledExceptionFilter(ExportDump);

	//--------------------------------
	// ウィンドウ
	//--------------------------------

	WNDCLASS wc{};

	//ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;

	//ウィンドウクラス名
	wc.lpszClassName = L"CG2WindowClass";

	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);

	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc);

	//クライアント領域のサイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kClientWidth,kClientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	HWND hwnd = CreateWindow(
		wc.lpszClassName,      //利用するクラス名
		L"CG2",                //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,   //良く見るウィンドウスタイル
		CW_USEDEFAULT,         //表示X座標
		CW_USEDEFAULT,         //表示Y座標
		wrc.right - wrc.left,  //ウィンドウ横幅
		wrc.bottom - wrc.top,  //ウィンドウ縦幅
		nullptr,               //親ウィンドウハンドル
		nullptr,               //メニューハンドル
		wc.hInstance,          //インスタンスハンドル
		nullptr                //オプション
	);

#ifdef _DEBUG

	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();

		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif // DEBUG


	//ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	//--------------------------------
	//文字列を格納する
	std::string str0{ "STRING!!!" };

	//整列を文字列にする
	std::string str1{ std::to_string(10) };

	Log(std::format("ClientSize = {} x {}\n", kClientWidth, kClientHeight));

	MSG msg{};
	//ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {

		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			//--------------------------------
			//ゲームの処理
			//--------------------------------

		}
	}

	return 0;
}


