
#include "stdafx.h"

void InitCanvas()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc = { 0 };
	if ( GetClassInfo(hInstance, Z_CANVAS_CLASS, &wc) ) return;

	wc.cbWndExtra = 8;
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = CCanvas::WndProc;
	wc.lpszClassName = Z_CANVAS_CLASS;
	wc.style = 0;
	RegisterClass(&wc);
}

CCanvas::CCanvas(LPCREATESTRUCT lpcs)
{
	CopyMemory(&m_cs, lpcs, sizeof(m_cs));
	m_tBuffer.hDesktopDC = GetDC(NULL);
	m_tBuffer.hBitmap = CreateCompatibleBitmap(m_tBuffer.hDesktopDC, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	m_tBuffer.hDC = CreateCompatibleDC(m_tBuffer.hDesktopDC);
	m_tBuffer.hOldBitmap = (HBITMAP) SelectObject(m_tBuffer.hDC, m_tBuffer.hBitmap);

	FloodFill(m_tBuffer.hDC, 0, 0, GetSysColor(COLOR_WINDOW));
}

CCanvas::~CCanvas()
{
	SelectObject(m_tBuffer.hDC, m_tBuffer.hOldBitmap);
	DeleteDC(m_tBuffer.hDC);
	DeleteObject(m_tBuffer.hBitmap);
	ReleaseDC(NULL, m_tBuffer.hDesktopDC);
}

LRESULT CALLBACK CCanvas::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CCanvas * This = (CCanvas*) GetWindowLong(hWnd, GWL_CANVASINSTANCE);

	switch(uMsg)
	{
	case WM_CREATE:
		{
			This = new CCanvas((LPCREATESTRUCT) lParam);
			This->m_hWnd = hWnd;
			SetWindowLong(hWnd, GWL_CANVASINSTANCE, (LONG) This);
			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			This->NotifyParent(uMsg, wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_ERASEBKGND:
		{
			This->OnRedraw((HDC) wParam);
			break;
		}
	case WM_DESTROY:
		{
			delete This;
			break;
		}
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

HDC CCanvas::GetBuffer()
{
	return m_tBuffer.hDC;
}

VOID CCanvas::Redraw()
{
	InvalidateRect(m_hWnd, NULL, TRUE);
}

VOID CCanvas::OnRedraw(HDC hDC)
{
	RECT rc = { 0 };
	GetClientRect(m_hWnd, &rc);
	BitBlt(hDC, 0, 0, rc.right, rc.bottom, m_tBuffer.hDC, 0, 0, SRCCOPY);
}

VOID CCanvas::NotifyParent(UINT uMsg, UINT fwKeys, SHORT x, SHORT y)
{
	CVNTF cvn = { 0 };
	cvn.nmh.code = CVN_MOUSE;
	cvn.nmh.hwndFrom = m_hWnd;
	cvn.nmh.idFrom = (UINT) m_cs.hMenu;
	cvn.pt.x = x;
	cvn.pt.y = y;
	cvn.uMsg = uMsg;
	cvn.fwKeys = fwKeys;
	SendMessage(m_cs.hwndParent, WM_NOTIFY, 0, (LPARAM) &cvn);
}