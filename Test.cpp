#include <windows.h>
#include "TgaImage.h"
#include <memory>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


RECT rec;

char szStr[200] = "please drag file";
POINT dropped_point;


char szClassName[] = "test";


int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
	LPSTR lpsCmdLine, int nCmdShow) {
	MSG msg;
	BOOL bRet;
	WNDCLASSEX wc;
	HWND hWnd;
	ATOM atom;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	//プロシージャ名
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hCurInst;//インスタンス
	wc.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)szClassName;
	wc.hIconSm = NULL;

	if ((atom = RegisterClassEx(&wc)) == 0)	return FALSE;

	hWnd = CreateWindow(MAKEINTATOM(atom), "DragDrop", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hCurInst, NULL);
	if (!hWnd) return FALSE;

	DragAcceptFiles(hWnd, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			break;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDROP hdrop;
	TCHAR filename[100];
	HBRUSH hBrush = 0;
	std::unique_ptr<TGAImage> ti = std::make_unique<TGAImage>();

	switch (msg) {
	case WM_PAINT:
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rec);  //ウインドウのサイズを取得
		hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, hBrush);
		PatBlt(hdc, 0, 0, rec.right, rec.bottom, PATCOPY);
		SetBkColor(hdc, RGB(10, 10, 10));  //文字の背景色を設定
		SetTextColor(hdc, RGB(200, 200, 200));  //文字の色を設定
		TextOut(hdc, 10, 10, (LPCTSTR)szStr, (int)strlen(szStr));
		EndPaint(hWnd, &ps);
		break;


	case WM_DROPFILES:
		hdrop = (HDROP)wp;
		DragQueryFile(hdrop, 0, filename, sizeof(filename) / sizeof(TCHAR));
		wsprintf(szStr, "path = %s", filename);
		InvalidateRect(hWnd, NULL, FALSE);
		
		
		ti->Load(filename);

		break;


	case WM_DESTROY:
		DeleteObject(hBrush);
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, msg, wp, lp));
	}
	return 0;
}