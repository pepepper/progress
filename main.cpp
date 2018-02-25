#include <windows.h>
#include <stdio.h>
LPCTSTR dataname[3]={TEXT("title"),TEXT("WhenOK"),TEXT("WhenNG")};
int timeout[2];
wchar_t text[3][128];
VOID ReadReg(){
	FILE *file;
	if(file=fopen("textset.ini","r")){
		fclose(file);
		for(int i=0;i<3;i++){

			GetPrivateProfileString(TEXT("KeyConfig"),dataname[i],TEXT("NULL"),text[i],2048,TEXT(".\\textset.ini"));
		}
		timeout[0]=GetPrivateProfileInt(TEXT("KeyConfig"),TEXT("OKtime"),0,TEXT(".\\textset.ini"));
		timeout[1]=GetPrivateProfileInt(TEXT("KeyConfig"),TEXT("NGtime"),0,TEXT(".\\textset.ini"));
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	RECT rect;
	switch(msg){
		case WM_TIMER:
			ShowWindow(hWnd,SW_SHOW);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			hdc=BeginPaint(hWnd,&ps);
			GetClientRect(hWnd,&rect);
			SetTextAlign(hdc,TA_CENTER|TA_BASELINE);
			GetTextMetrics(hdc,&tm);
			TextOut(hdc,rect.right/2-5,rect.bottom/3,TEXT("進捗"),lstrlen(TEXT("進捗")));
			TextOut(hdc,rect.right/2-5,rect.bottom/3+tm.tmHeight,TEXT("どうですか"),lstrlen(TEXT("どうですか")));
			EndPaint(hWnd,&ps);
			return 0;
		case WM_KEYDOWN:
			if(wp==27)	WndProc(hWnd,WM_DESTROY,0,0);
			return 0;
		case WM_COMMAND:
			switch(LOWORD(wp)){//さっきのボタンの子ウィンドウIDなら
				case 1:
					MessageBox(NULL,text[1],TEXT("進捗"),MB_OK|MB_ICONINFORMATION);
					SetTimer(hWnd,1,timeout[0],NULL);
					ShowWindow(hWnd,SW_HIDE);
					return 0;
				case 2:
					MessageBox(NULL,text[2],TEXT("進捗"),MB_OK|MB_ICONWARNING);
					SetTimer(hWnd,1,timeout[1],NULL);
					ShowWindow(hWnd,SW_HIDE);
					return 0;
			}
	}
	return DefWindowProc(hWnd,msg,wp,lp);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;
	wchar_t windowtitle[256];
	ReadReg();
	winc.style=CS_HREDRAW|CS_VREDRAW;
	winc.lpfnWndProc=WndProc;
	winc.cbClsExtra=winc.cbWndExtra=0;
	winc.hInstance=hInstance;
	winc.hIcon=NULL;
	winc.hCursor=NULL;
	winc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName=NULL;
	winc.lpszClassName=TEXT("KITTY");

	if(!RegisterClass(&winc)) return -1;
	wsprintf(windowtitle,TEXT("%s"),text[0]);
	hwnd=CreateWindow(
		TEXT("KITTY"),windowtitle,
		WS_VISIBLE,
		CW_USEDEFAULT,CW_USEDEFAULT,
		320,240,
		NULL,NULL,hInstance,NULL
	);

	CreateWindow(TEXT("BUTTON"),TEXT("OKです"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,75,170,80,25,hwnd,(HMENU)1,hInstance,NULL);

	CreateWindow(TEXT("BUTTON"),TEXT("ダメです!"),WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,155,170,80,25,hwnd,(HMENU)2,hInstance,NULL);

	if(hwnd==NULL) return -1;

	while(GetMessage(&msg,NULL,0,0)) DispatchMessage(&msg);
	return msg.wParam;
}