
class CMyControl
{
	HWND			m_hWnd;

protected:
	CMyControl(CMyDialog * pDialog, UINT ctlId);
	LRESULT			dSendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
};

class CMyListView: public CMyControl
{
	UINT			m_iColumn;

public:
	CMyListView(CMyDialog * pDialog, UINT ctlId);

	UINT			Count();
	UINT			GetCurSel();
	VOID			ExtendedStyle(UINT uExStyle);
	UINT			ExtendedStyle();
	VOID			InsertColumn(const LPTSTR pszColumn, UINT cx = -1);
	UINT			InsertItem(const LPTSTR pszString, LPARAM lParam = 0, UINT uPos = -1);
	VOID			SetItem(UINT uItemId, LPTSTR pszString, UINT uColumn);
	UINT			InsertItem(DWORD uValue, LPARAM lParam = 0, UINT uPos = -1);
	VOID			SetItem(UINT uItemId, DWORD uValue, UINT uColumn);
	VOID			DeleteItem(UINT uItemId);
	VOID			Param(UINT uItemId, LPARAM lParam);
	LPARAM			Param(UINT uItemId);
};