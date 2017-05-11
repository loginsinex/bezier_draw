

class CMyDialog
{
	static		INT_PTR WINAPI DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID		InternalOnSize(UINT fwSizeType, SHORT cx, SHORT cy);
	VOID		InternalOnSizing(UINT fwSizeType, LPRECT lprc);
	VOID		InternalOnCommand(USHORT uId, USHORT uCmd, HWND hCtl);
	VOID		InitAnchors();

	HWND					m_hWnd;
	LPARAM					m_lUserParam;
	CMyDialog				*m_pDlgParent;
	vector<CSizeAnchor*>	m_vAncList;

	struct
	{
		HINSTANCE		hInstance;
		LPTSTR			lpszResource;
	} m_sRes;

protected:
	CMyDialog(HINSTANCE hInstance, LPTSTR lpszResource);
	~CMyDialog();

public:
	LRESULT				dSendMessage(USHORT uId, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	int					Show(CMyDialog * pParent = NULL, LPARAM lParam = 0);
	void				Close(INT ret);
	CMyDialog			*Parent();
	VOID				AddAnchor(UINT uId, UINT uAnchorType);
	VOID				AddCustomAnchor(UINT uId, AnchorType left, AnchorType top, AnchorType right, AnchorType bottom);
	HWND				GetCtl(USHORT uId);
	HWND				WindowHandle();
	HINSTANCE			Instance();
	void				EnableCtl(USHORT uId, BOOL bEnable);

	virtual void		OnInit(LPARAM lParam);
	virtual void		OnOK();
	virtual void		OnCancel();
	virtual void		OnButtonClick(USHORT uId, HWND hCtl);
	virtual void		OnCommand(USHORT uId, USHORT uCmd, HWND hCtl);
	virtual int			OnNotify(LPNMHDR lpnm);
	virtual int			OnClose();
};