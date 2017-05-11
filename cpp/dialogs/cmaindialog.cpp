
#include "stdafx.h"

/* CMainDialog::CMainDialog																				*\
|	Конструктор главного окна программы.
|	Устанавливает якори для элементов управления.
|	В имени константы указывается, к какой стороне окна элемент управления должен привязаться.
\*																										*/
CMainDialog::CMainDialog(HINSTANCE hInstance) : CMyDialog(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG))
{
	AddAnchor(ID_OK, ANCHOR_RIGHT_BOTTOM);
	AddAnchor(IDCANCEL, ANCHOR_RIGHT_BOTTOM);
	AddAnchor(IDC_INSERTPOINT, ANCHOR_LEFT_BOTTOM);
	AddAnchor(IDC_DELPOINT, ANCHOR_LEFT_BOTTOM);
	AddAnchor(IDC_CURVESLIST, ANCHOR_LEFT);
	AddAnchor(IDC_CANVAS, ANCHOR_FULL);
	AddAnchor(IDC_STATUS, ANCHOR_BOTTOM);
}


/* CMainDialog::OnInit																					*\
|	Метод OnInit вызывается перед непосредственным отображением диалога на экране.
|	Для работы диалога потребуется инициализировать список (ListView)
|	и получить указатель на экземпляр CCanvas
|
\*																										*/
VOID CMainDialog::OnInit(LPARAM lParam)
{
	m_cDot.lpcv = NULL;

	m_plv = new CMyListView(this, IDC_CURVESLIST);
	m_plv->InsertColumn(TEXT("Coordinates"), 75);
	m_plv->InsertColumn(TEXT("Color"), 100);
	m_plv->ExtendedStyle(LVS_EX_FULLROWSELECT);

	m_pCanvas = (CCanvas*) GetWindowLong(GetCtl(IDC_CANVAS), GWL_CANVASINSTANCE);
}

/* CMainDialog::OnClose																					*\
|	Метод OnClose вызывается перед закрытием диалога.
|	Здесь уничтожаются объекты, которые были созданы между вызовами OnInit и OnClose, включая эти методы.
\*																										*/
int CMainDialog::OnClose()
{
	delete m_plv;
	for(vector<LPCURVEPOINT>::iterator v = m_vCurveList.begin(); v < m_vCurveList.end(); v++)
		delete (*v);

	return 0;
}

/* CMainDialog::OnNotify																					*\
|	В метод OnNotify поступают различного рода уведомления от элементов управления.
|	Нас интересует только уведомление о событиях от мыши, поступающих в ЭУ Canvas.
|	При нажатии на л.к.м. мы фиксируем точку, которую потребуется перемещать,
|	при перемещении мыши при фиксированной точке мы меняем координаты точки в соответствии с текущими координатами мыши,
|	при отпускании л.к.м. мы снимаем фиксацию точки.
\*																											*/
int CMainDialog::OnNotify(LPNMHDR lpnm)
{
	if ( IDC_CANVAS == lpnm->idFrom && CVN_MOUSE == lpnm->code )
	{
		PCVNTF lpnmcv = (PCVNTF) lpnm;
		
		switch(lpnmcv->uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				if ( !m_cDot.lpcv )
				{
					m_cDot.lpcv = HitTest((SHORT) (0xFFFF & lpnmcv->pt.x) , (SHORT) (0xFFFF & lpnmcv->pt.y));
					if ( m_cDot.lpcv )
					{
						SetCapture(GetCtl(IDC_CANVAS));
					}
				}
				break;
			}
		case WM_LBUTTONUP:
			{
				if ( m_cDot.lpcv )
				{
					ReleaseCapture();
					m_cDot.lpcv = NULL;
				}
				break;
			}
		case WM_MOUSEMOVE:
			{
				if ( m_cDot.lpcv )
				{
					m_cDot.lpcv->pt.x = lpnmcv->pt.x;
					m_cDot.lpcv->pt.y = lpnmcv->pt.y;
					UpdateCoordList();
					DrawCurves();
				}
				break;
			}
		}
	}
	return 0;
}

/* CMainDialog::OnButtonClick																				*\
|	В метода OnButtonClick мы фиксируем команды от пользователя (добавить, удалить точку),
|	когда тот нажимает на соответствующую кнопку.
\*																											*/
void CMainDialog::OnButtonClick(USHORT uId, HWND hCtl)
{
	switch(uId)
	{
	case IDC_INSERTPOINT:
		{
			CNewPtDlg dlgNewPt(Instance());
			if ( IDOK == dlgNewPt.Show(this, ( 0 == m_plv->Count() ) ) )
			{
				COLORREF clr; cpt_insert insMode;
				dlgNewPt.GetParameters(&clr, &insMode);
				InsertNewPoint(clr, insMode);
			}
			break;
		}
	case IDC_DELPOINT:
		{
			INT lv_id = m_plv->GetCurSel();
			if ( -1 == lv_id )
			{
				MessageBox(WindowHandle(), TEXT("Please, choose a point for removing."), NULL, MB_ICONERROR);
			}
			else if ( IDYES == MessageBox(WindowHandle(), TEXT("Are you sure that you want to remove this point?"), TEXT(""), MB_ICONQUESTION | MB_YESNO) )
			{
				LPCURVEPOINT lpcv = (LPCURVEPOINT) m_plv->Param(lv_id);
				for(vector<LPCURVEPOINT>::iterator v = m_vCurveList.begin(); v < m_vCurveList.end(); ++v)
				{
					if ( (*v) == lpcv )
					{
						m_vCurveList.erase(v);
						break;
					}
				}
				delete lpcv;
				m_plv->DeleteItem(lv_id);
				DrawCurves();
			}
			break;
		}
	}
}

/* CMainDialog::InsertNewPoint																				*\
|	Метод вызывается, когда пользователь принял все параметры в диалоге IDD_NEWPOINT и нажал ОК.
|	В метод передается выбранный пользователем цвет и метод вставки (в начало или в конец).
|
\*																											*/
void CMainDialog::InsertNewPoint(COLORREF clr, cpt_insert insertMode)
{
	LPCURVEPOINT lpcv = new CURVEPOINT;
	lpcv->col = clr;
	lpcv->pt.x = lpcv->pt.y = 10;

	int lv_id = 0;
	int lv_cursel = m_plv->GetCurSel();
	LPCURVEPOINT lpcv_cur = (LPCURVEPOINT) m_plv->Param(lv_cursel);

	// find selected element
	vector<LPCURVEPOINT>::iterator v_cur;
	for(v_cur = m_vCurveList.begin(); v_cur < m_vCurveList.end(); ++v_cur)
		if ( (*v_cur) == lpcv_cur ) break;

	switch(insertMode)
	{
	case cptInsertFirst:
		{
			lv_id = m_plv->InsertItem(TEXT(""), (LPARAM) lpcv, 0);
			m_vCurveList.insert(m_vCurveList.begin(), lpcv);
			break;
		}
	case cptInsertAfterSelected:
		{
			lv_id = m_plv->InsertItem(TEXT(""), (LPARAM) lpcv, lv_cursel);
			m_vCurveList.insert(v_cur, lpcv);
			break;
		}
	default:
		{
			lv_id = m_plv->InsertItem(TEXT(""), (LPARAM) lpcv);
			m_vCurveList.push_back(lpcv);
			break;
		}
	}
	m_plv->SetItem(lv_id, clr, 1);
	UpdateCoordList();
	DrawCurves();
}

/* CMainDialog::HitTest																						*\
|	Метод HitTest ищет точку по указанным координатам.
|	Вызывается, если пользователь нажал на л.к.м в пределах поля CCanvas
\*																											*/
LPCURVEPOINT CMainDialog::HitTest(SHORT x, SHORT y)
{
	for(vector<LPCURVEPOINT>::iterator v = m_vCurveList.begin(); v < m_vCurveList.end(); ++v)
	{
		LPCURVEPOINT lpcv = (*v);
		if ( ( x >= lpcv->pt.x - 3 && x <= lpcv->pt.x + 3 ) && ( y >= lpcv->pt.y - 3 && y <= lpcv->pt.y + 3 ) )
			return lpcv;
	}
	return NULL;
}


/* CMainDialog::UpdateCoordList																				*\
|	Метод обновляет координаты в списке точек при добавлении или при перемещении точек
\*																											*/
void CMainDialog::UpdateCoordList()
{
	INT c = m_plv->Count(), i = 0;
	for(i=0;i<c;++i)
	{
		LPCURVEPOINT lpcv = (LPCURVEPOINT) m_plv->Param(i);
		if ( lpcv )
		{
			TCHAR pszCoord[16];
			wsprintf(pszCoord, TEXT("%d:%d"), lpcv->pt.x, lpcv->pt.y);
			m_plv->SetItem(i, pszCoord, 0);
		}
	}
}

/* CMainDialog::DrawCurves																					*\
|	Метод обновляет поле CCanvas и отрисовывает кривую Безье по заданным точкам.
\*																											*/
void CMainDialog::DrawCurves()
{
	HDC hDC = m_pCanvas->GetBuffer();
	RECT rc = { 0 , 0 , GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	Rectangle(hDC, -1, -1, rc.right + 1, rc.bottom + 1);
	HPEN hPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_WINDOW)), hSysPen = (HPEN) SelectObject(hDC, hPen);
	HPEN hCurvePen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	for(vector<LPCURVEPOINT>::iterator v = m_vCurveList.begin(); v < m_vCurveList.end(); ++v)
	{
		LPCURVEPOINT lpcv = (*v);
		HBRUSH hBrush = CreateSolidBrush(lpcv->col), hSysBrush = (HBRUSH) SelectObject(hDC, hBrush);
		Ellipse(hDC, lpcv->pt.x - 3, lpcv->pt.y - 3, lpcv->pt.x + 3, lpcv->pt.y + 3); 
		SelectObject(hDC, hSysBrush);
		DeleteObject(hBrush);
	}

	SelectObject(hDC, hCurvePen);
	DrawBezierArc(hDC);
	SelectObject(hDC, hSysPen);
	DeleteObject(hPen);
	DeleteObject(hCurvePen);

	m_pCanvas->Redraw();
}

/* CMainDialog::DrawBezierArc																				*\
|	Непосредственно отрисовывающий кривую Безье на заданном контексте HDC
\*																											*/
void CMainDialog::DrawBezierArc(HDC hDC)
{
	UINT uSizeOfList = m_vCurveList.size();
	DPOINT ptList[32];
	if ( uSizeOfList > 32 ) return;

	int i = 0;
	for(vector<LPCURVEPOINT>::iterator v = m_vCurveList.begin(); v < m_vCurveList.end(); v++)
	{
		ptList[i].x = (*v)->pt.x;
		ptList[i].y = (*v)->pt.y;
		++i;
	}

	DPOINT pCur;

	for(i=0; i<=1000; i+=5)
	{
		FindBezierPt(ptList, uSizeOfList, (double) ((double) i)/1000.0, &pCur);
		if ( i > 0 )
			LineTo(hDC, (int) pCur.x, (int) pCur.y);
		else
			MoveToEx(hDC, (int) pCur.x, (int) pCur.y, NULL);
	}
}

/* CMainDialog::FindBezierPt																				*\
|	Метод находит точку на кривой Безье для заданного параметра t.
|	В метод передаются все базовые и вспомогательные точки кривой.
\*																											*/
BOOL CMainDialog::FindBezierPt(DPOINT * pptList, UINT uSizeOfList, double t, DPOINT * pptOut)
{
	if ( uSizeOfList < 2 || uSizeOfList > 32 ) return FALSE;
	DPOINT FloorList[32];
	DPOINT * pFloorList = NULL;
	if ( uSizeOfList == 2 ) pFloorList = pptOut;
	else pFloorList = (PDPOINT) &FloorList;
	for(UINT i=0; i < uSizeOfList - 1; ++i)
	{
		pFloorList[i].x = (pptList[i].x + t*(pptList[i+1].x-pptList[i].x));
		pFloorList[i].y = (pptList[i].y + t*(pptList[i+1].y-pptList[i].y));
	}

	if ( 2 == uSizeOfList ) return TRUE;
	BOOL bResult = FindBezierPt(pFloorList, uSizeOfList - 1, t, pptOut);
	return bResult;
}