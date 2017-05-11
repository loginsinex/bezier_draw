
#include "stdafx.h"

CNewPtDlg::CNewPtDlg(HINSTANCE hInstance) : CMyDialog(hInstance, MAKEINTRESOURCE(IDD_NEWPOINT))
{
	m_userColor = GetSysColor(COLOR_WINDOWTEXT);
	m_insertMode = cptInsertLast;
}

void CNewPtDlg::OnInit(LPARAM lParam)
{
	SetStaticColor();
	dSendMessage(IDC_RADIO3, BM_SETCHECK, TRUE);

	if ( lParam )
	{
		EnableCtl(IDC_RADIO1, FALSE);
		EnableCtl(IDC_RADIO2, FALSE);
		EnableCtl(IDC_RADIO3, FALSE);
	}
}

void CNewPtDlg::SetStaticColor()
{
	RECT rc = { 0 };
	GetClientRect(GetCtl(IDC_COLOR), &rc);
	CCanvas * pCanvas = (CCanvas*) GetWindowLong(GetCtl(IDC_COLOR), GWL_CANVASINSTANCE);
	HBRUSH hSysBrush, hCurBrush;
	hCurBrush = CreateSolidBrush(m_userColor);
	hSysBrush = (HBRUSH) SelectObject(pCanvas->GetBuffer(), hCurBrush);
	Rectangle(pCanvas->GetBuffer(), 0, 0, rc.right, rc.bottom);
	SelectObject(pCanvas->GetBuffer(), hSysBrush);
	DeleteObject(hCurBrush);
	pCanvas->Redraw();
}

void CNewPtDlg::OnButtonClick(USHORT uId, HWND hCtl)
{
	switch(uId)
	{
	case IDC_SETCOLOR:
		{
			CHOOSECOLOR cc = { 0 };
			COLORREF acrCustClr[16] = { 0 };
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = WindowHandle();
			cc.Flags = CC_ANYCOLOR | CC_RGBINIT;
			cc.lpCustColors = acrCustClr;
			cc.rgbResult = m_userColor;
			if ( ChooseColor(&cc) )
			{
				m_userColor = cc.rgbResult;
				SetStaticColor();
			}
			break;
		}
	}
}

void CNewPtDlg::OnOK()
{
	if ( dSendMessage(IDC_RADIO1, BM_GETCHECK) ) m_insertMode = cptInsertFirst;
	else if ( dSendMessage(IDC_RADIO2, BM_GETCHECK) ) m_insertMode = cptInsertAfterSelected;
	else if ( dSendMessage(IDC_RADIO3, BM_GETCHECK) ) m_insertMode = cptInsertLast;
	Close(IDOK);
}

void CNewPtDlg::GetParameters(COLORREF * userColor, cpt_insert * insertMode)
{
	if ( userColor ) *userColor = m_userColor;
	if ( insertMode ) *insertMode = m_insertMode;
}