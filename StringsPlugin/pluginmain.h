// Copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef _PLUGINMAIN_H
#define _PLUGINMAIN_H

#include <windows.h>
#include "pluginsdk\_plugins.h"
#include "QtPlugin.h"
#include <QString>
#include "searchstringswidget.h"
#include "xprocess.h"
#include "xprocessdevice.h"
#include "spdialoginfo.h"

namespace Plugin
{
extern int handle;
extern HWND hwndDlg;
extern int hMenu;
extern int hMenuDisasm;
extern int hMenuDump;
extern int hMenuStack;
extern QString sCurrentFileName;
extern CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
extern IMAGEHLP_MODULE64 modInfo;
extern QString sDebugFileName;
extern PROCESS_INFORMATION fdProcessInfo;
extern SearchStringsWidget *pSearchWidget;
extern XProcessDevice *pDevice;
extern qint64 nAddress;
extern SearchStrings::OPTIONS options;

enum PLUGIN_MENU
{
    PLUGIN_MENU_ABOUT
};
} //Plugin

#endif //_PLUGINMAIN_H
