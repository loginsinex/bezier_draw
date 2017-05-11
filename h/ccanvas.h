
#define Z_CANVAS_CLASS			TEXT("ZCanvas")
#define GWL_CANVASINSTANCE		4
#define CVN_MOUSE				-50

typedef struct _tagCVNTF
{
	NMHDR		nmh;
	UINT		uMsg;
	UINT		fwKeys;
	POINT		pt;
} CVNTF, *PCVNTF;

void InitCanvas();

class CCanvas
{
	static			LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND			m_hWnd;
	CREATESTRUCT	m_cs;

	struct
	{
		HDC		hDesktopDC;
		HDC		hDC;
		HBITMAP	hBitmap;
		HBITMAP	hOldBitmap;
	} m_tBuffer;

	CCanvas(LPCREATESTRUCT lpcs);
	~CCanvas();

	VOID		OnRedraw(HDC hDC);
	VOID		NotifyParent(UINT uMsg, UINT fwKeys, SHORT x, SHORT y);

	friend		void InitCanvas();

public:
	HDC			GetBuffer();
	VOID		Redraw();
};