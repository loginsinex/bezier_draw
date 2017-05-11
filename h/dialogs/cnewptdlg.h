
typedef enum
{
	cptInsertFirst,
	cptInsertAfterSelected,
	cptInsertLast
} cpt_insert;

class CNewPtDlg: public CMyDialog
{
	COLORREF		m_userColor;
	cpt_insert		m_insertMode;

	void			OnInit(LPARAM lParam);
	void			OnButtonClick(USHORT uId, HWND hCtl);
	void			OnOK();
	void			SetStaticColor();

public:
	CNewPtDlg(HINSTANCE hInstance);
	void			GetParameters(COLORREF * userColor, cpt_insert * insertMode);
};