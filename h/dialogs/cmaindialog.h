
#define		COLOR_BEZIER_CURVE			RGB(0, 200, 255)

typedef struct _tagCURVEPOINT
{
	POINT		pt;
	COLORREF	col;
} CURVEPOINT, *LPCURVEPOINT; 

typedef struct _tagDPOINT
{
	double		x;
	double		y;
} DPOINT, *PDPOINT;

class CMainDialog: public CMyDialog
{
	CCanvas					*m_pCanvas;
	CMyListView				*m_plv;
	vector<LPCURVEPOINT>	m_vCurveList;

	struct
	{
		LPCURVEPOINT		lpcv;
	} m_cDot;

	void			OnInit(LPARAM lParam);
	int				OnClose();
	void			OnButtonClick(USHORT uId, HWND hWnd);
	int				OnNotify(LPNMHDR lpnm);
	void			InsertNewPoint(COLORREF clr, cpt_insert insertMode);
	void			UpdateCoordList();
	void			DrawCurves();
	LPCURVEPOINT	HitTest(SHORT x, SHORT y);

	void			DrawBezierArc(HDC hDC);
	BOOL			FindBezierPt(DPOINT * pptList, UINT uSizeOfList, double t, DPOINT * pptOut);

public:
	CMainDialog(HINSTANCE hInstance);
};