
#include "stdafx.h"

CMyDialog::CMyDialog(HINSTANCE hInstance, LPTSTR lpszResource)
{
	m_sRes.hInstance = hInstance;
	m_sRes.lpszResource = lpszResource;
}

CMyDialog::~CMyDialog()
{
	vector<CSizeAnchor*>::iterator v;
	for(v=m_vAncList.begin();v<m_vAncList.end();v++)
		delete (*v);

	m_vAncList.clear();
}

VOID CMyDialog::AddAnchor(UINT uId, UINT uAnchorType)
{
	switch(uAnchorType)
	{
	case ANCHOR_LEFT_TOP: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop); break;
	case ANCHOR_LEFT_BOTTOM: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom); break;
	case ANCHOR_RIGHT_TOP: AddCustomAnchor(uId, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop); break;
	case ANCHOR_RIGHT_BOTTOM: AddCustomAnchor(uId, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	case ANCHOR_LEFT: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom); break;
	case ANCHOR_RIGHT: AddCustomAnchor(uId, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	case ANCHOR_TOP: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop); break;
	case ANCHOR_BOTTOM: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	case ANCHOR_FULL: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	}
}

VOID CMyDialog::AddCustomAnchor(UINT uId, AnchorType left, AnchorType top, AnchorType right, AnchorType bottom)
{
	CSizeAnchor * anchor = new CSizeAnchor(uId, left, top, right, bottom);
	m_vAncList.push_back(anchor);
}


INT_PTR WINAPI CMyDialog::DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMyDialog * This = (CMyDialog*) GetWindowLong(hDlg, DWL_USER);

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			This = (CMyDialog*) lParam;
			This->m_hWnd = hDlg;
			SetWindowLong(hDlg, DWL_USER, lParam);
			This->InitAnchors();
			This->OnInit(This->m_lUserParam);
			break;
		}
	case WM_COMMAND:
		{
			This->InternalOnCommand(LOWORD(wParam), HIWORD(wParam), (HWND) lParam);
			break;
		}
	case WM_NOTIFY:
		{
			int result = This->OnNotify((LPNMHDR) lParam);
			SetWindowLong(hDlg, DWL_MSGRESULT, result);
			return result;
		}
	case WM_SIZE: 
		{
			This->InternalOnSize(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_SIZING:
		{
			This->InternalOnSizing(wParam, LPRECT(lParam));
			break;
		}
	case WM_CLOSE:
		{
			int result = This->OnClose();
			EndDialog(hDlg, result);
			break;
		}
	default: return false;
	}
	return true;
}

void CMyDialog::InternalOnCommand(USHORT uId, USHORT uCmd, HWND hCtl)
{
	switch(uCmd)
	{
	case BN_CLICKED:
		{
			switch(uId)
			{
			case IDOK: OnOK(); break;
			case IDCANCEL: OnCancel(); break;
			default: OnButtonClick(uId, hCtl); break;
			}
			break;
		}
	default: OnCommand(uId, uCmd, hCtl); break;
	}
}

VOID CMyDialog::InitAnchors()
{
	vector<CSizeAnchor*>::iterator v;

	for(v=m_vAncList.begin(); v < m_vAncList.end(); v++)
		(*v)->Initialize(m_hWnd);
}

VOID CMyDialog::InternalOnSize(UINT fwSizeType, SHORT cx, SHORT cy)
{
	HDWP hdwp = BeginDeferWindowPos(m_vAncList.size());
	vector<CSizeAnchor*>::iterator v;

	for(v=m_vAncList.begin(); v < m_vAncList.end(); v++)
		(*v)->ResizeByDefer(hdwp);

	EndDeferWindowPos(hdwp);
}

VOID CMyDialog::InternalOnSizing(UINT fwSizeType, LPRECT lprc)
{
	InternalOnSize(fwSizeType, (USHORT) (0xFFFF & lprc->right) , (USHORT) (0xFFFF & lprc->bottom) );	
}

HWND CMyDialog::WindowHandle()
{
	return m_hWnd;
}

HINSTANCE CMyDialog::Instance()
{
	return m_sRes.hInstance;
}

HWND CMyDialog::GetCtl(USHORT uId)
{
	return GetDlgItem(m_hWnd, uId);
}

int CMyDialog::Show(CMyDialog * pParent, LPARAM lParam)
{
	m_lUserParam = lParam;
	m_pDlgParent = pParent;

	return DialogBoxParam(m_sRes.hInstance, m_sRes.lpszResource, ( pParent ? pParent->m_hWnd : NULL ), CMyDialog::DlgProc, (LPARAM) this);
}

LRESULT CMyDialog::dSendMessage(USHORT uId, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return SendDlgItemMessage(m_hWnd, uId, uMsg, wParam, lParam);
}

void CMyDialog::EnableCtl(USHORT uId, BOOL bEnable)
{
	EnableWindow(GetCtl(uId), bEnable);
}

void CMyDialog::Close(INT ret)
{
	EndDialog(m_hWnd, ret);
}

void CMyDialog::OnInit(LPARAM lParam)
{
	return;
}

void CMyDialog::OnOK()
{
	return;
}

void CMyDialog::OnCancel()
{
	EndDialog(m_hWnd, IDCANCEL);
	return;
}

void CMyDialog::OnButtonClick(USHORT uId, HWND hCtl)
{
	return;
}

void CMyDialog::OnCommand(USHORT uId, USHORT uCmd, HWND hCtl)
{
	return;
}

int	CMyDialog::OnNotify(LPNMHDR lpnm)
{
	return 0;
}

int CMyDialog::OnClose()
{
	return 0;
}