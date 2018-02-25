#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd , UINT msg , WPARAM wp , LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	RECT rect;
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			hdc = BeginPaint(hWnd , &ps);
			GetClientRect(hWnd , &rect);
			SetTextAlign(hdc , TA_CENTER | TA_BASELINE);
			GetTextMetrics(hdc , &tm);
			TextOut(hdc,rect.right/2-5,rect.bottom/3,TEXT("進捗"), lstrlen(TEXT("進捗")));
			TextOut(hdc ,rect.right/2-5,rect.bottom/3+tm.tmHeight, TEXT("どうですか"), lstrlen(TEXT("どうですか")));
			EndPaint(hWnd , &ps);
			return 0;
		case WM_KEYDOWN:
			if (wp == 27)	WndProc(hWnd, WM_DESTROY, 0, 0);
			return 0;
	}
	return DefWindowProc(hWnd , msg , wp , lp);
}

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance ,LPSTR lpCmdLine , int nCmdShow ) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= NULL;
	winc.hCursor		= NULL;
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("KITTY");

	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
			TEXT("KITTY") , TEXT("　　　　　　　　　　グラ担当さん") ,
			WS_VISIBLE ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			320 , 240 ,
			NULL , NULL , hInstance , NULL
	);

	CreateWindow(
		TEXT("BUTTON") , TEXT("OKです") ,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		75 , 170 , 80 , 25 ,
		hwnd , NULL , hInstance , NULL
	);

	CreateWindow(
		TEXT("BUTTON") , TEXT("ダメです!") ,
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON ,
		155 , 170 , 80 , 25 ,
		hwnd , NULL , hInstance , NULL
	);

	if (hwnd == NULL) return -1;

	while(GetMessage(&msg , NULL , 0 , 0)) DispatchMessage(&msg);
	return msg.wParam;
}