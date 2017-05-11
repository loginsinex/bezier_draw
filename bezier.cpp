// bezier.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "bezier.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	InitCommonControls();
	InitCanvas();
	CMainDialog dMain(hInstance);
	dMain.Show(NULL);
	return 0;
}