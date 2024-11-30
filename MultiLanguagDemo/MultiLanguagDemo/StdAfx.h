#pragma once


#define _CRT_SECURE_NO_DEPRECATE
#include <WinSock2.h>
#include <windows.h>

#include "../../include/UIlib.h"
using namespace DuiLib;
//#pragma comment(lib, "../../lib/Duilib_u.lib")
#ifdef _DEBUG
#pragma comment(lib, "../../x64/Debug/DuiLib.lib")
#else
#pragma comment(lib, "../../x64/Release/DuiLib.lib")
#endif // _DEBUG

#include "../../Util/StringTools.h"
#include "../../Util/ToolsFunc.h"

//JSON
#include "json/json.h"
//#pragma comment(lib,"../json/lib/jsoncpp.lib")

extern wstring g_strAppPath;
//∂‡”Ô—‘¥¶¿Ì
void LoadMultiLanguageString(const wstring strLanguageJsonFile);
