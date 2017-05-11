// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <xstring>
#include <vector>

using namespace std;
// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
#include "resource.h"
#include "ccanvas.h"
#include "csizeanchor.h"
#include "cmydialog.h"
#include "controls.h"

#include "dialogs\cnewptdlg.h"
#include "dialogs\cmaindialog.h"

