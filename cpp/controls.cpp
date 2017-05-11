
#include "stdafx.h"

CMyControl::CMyControl(CMyDialog * pDialog, UINT ctlId)
{
	m_hWnd = GetDlgItem(pDialog->WindowHandle(), ctlId);
}

LRESULT CMyControl::dSendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(m_hWnd, uMsg, wParam, lParam);
}

CMyListView::CMyListView(CMyDialog * pDialog, UINT ctlId) : CMyControl(pDialog, ctlId)
{
	m_iColumn = 0;
}

VOID CMyListView::ExtendedStyle(UINT uExStyle)
{
	dSendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, uExStyle, uExStyle);
}

UINT CMyListView::ExtendedStyle()
{
	return dSendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
}

VOID CMyListView::InsertColumn(const LPTSTR pszColumn, UINT cx)
{
	LV_COLUMN lvc = { 0 };
	lvc.cx = cx;
	lvc.fmt = LVCFMT_LEFT;
	lvc.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	lvc.pszText = pszColumn;
	dSendMessage(LVM_INSERTCOLUMN, m_iColumn, (LPARAM) &lvc);
	m_iColumn++;
}

UINT CMyListView::InsertItem(const LPTSTR pszString, LPARAM lParam, UINT uPos)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.pszText = pszString;
	lvi.lParam = lParam;
	return dSendMessage(LVM_INSERTITEM, ( -1 == uPos ? Count() : uPos ), (LPARAM) &lvi);
}

UINT CMyListView::Count()
{
	return dSendMessage(LVM_GETITEMCOUNT);
}

UINT CMyListView::GetCurSel()
{
	return dSendMessage(LVM_GETNEXTITEM, -1, MAKELPARAM(LVIS_FOCUSED | LVIS_SELECTED, 0));
}

VOID CMyListView::SetItem(UINT uItemId, LPTSTR pszString, UINT uColumn)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT;
	lvi.iItem = uItemId;
	lvi.iSubItem = uColumn;
	lvi.pszText = pszString;
	dSendMessage(LVM_SETITEM, 0, (LPARAM) &lvi);
}

UINT CMyListView::InsertItem(DWORD uValue, LPARAM lParam, UINT uPos)
{
	TCHAR pszString[16];
	wsprintf(pszString, TEXT("%d"), uValue);
	return InsertItem(pszString, lParam, uPos);
}

VOID CMyListView::SetItem(UINT uItemId, DWORD uValue, UINT uColumn)
{
	TCHAR pszString[16];
	wsprintf(pszString, TEXT("%d"), uValue);
	return SetItem(uItemId, pszString, uColumn);
}

VOID CMyListView::DeleteItem(UINT uItemId)
{
	dSendMessage(LVM_DELETEITEM, uItemId);
}

VOID CMyListView::Param(UINT uItemId, LPARAM lParam)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	lvi.iItem = uItemId;
	lvi.lParam = lParam;
	dSendMessage(LVM_SETITEM, 0, (LPARAM) &lvi);
}

LPARAM CMyListView::Param(UINT uItemId)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	lvi.iItem = uItemId;
	dSendMessage(LVM_GETITEM, 0, (LPARAM) &lvi);
	return lvi.lParam;
}
