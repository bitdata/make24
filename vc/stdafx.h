// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define _SEMIAUTOMATIC_
#define _FRACTIONAL_

// Change these values to use different versions
#define WINVER		0x0500
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0500
//#define _RICHEDIT_VER	0x0100
#define _CRT_RAND_S
#define _ATL_NO_COM_SUPPORT

#include <atlbase.h>
#include <atlapp.h>
#if defined(_APP_MODULE_)
extern CAppModule _Module;
#endif
#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcoll.h>

#include <strsafe.h>
#pragma warning(disable:4995)
#include <algorithm>
#pragma warning(disable:4995)
using namespace std;

#include "cards.h"