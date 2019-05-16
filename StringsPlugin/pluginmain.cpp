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
#include "pluginmain.h"

int Plugin::handle;
HWND Plugin::hwndDlg;
int Plugin::hMenu;
int Plugin::hMenuDisasm;
int Plugin::hMenuDump;
int Plugin::hMenuStack;
QString Plugin::sCurrentFileName;
CREATE_PROCESS_DEBUG_INFO Plugin::CreateProcessInfo;
IMAGEHLP_MODULE64 Plugin::modInfo;
QString Plugin::sDebugFileName;
PROCESS_INFORMATION Plugin::fdProcessInfo;
SearchStringsWidget *Plugin::pSearchWidget=nullptr;
XProcessDevice *Plugin::pDevice=nullptr;
qint64 Plugin::nAddress=0;
SearchStrings::OPTIONS Plugin::options;

extern "C" __declspec(dllexport) bool pluginit(PLUG_INITSTRUCT* initStruct)
{
    initStruct->pluginVersion = plugin_version;
    initStruct->sdkVersion = PLUG_SDKVERSION;
    strcpy_s(initStruct->pluginName, plugin_name);

    Plugin::handle = initStruct->pluginHandle;
    QtPlugin::Init();
    return true;
}

extern "C" __declspec(dllexport) void plugsetup(PLUG_SETUPSTRUCT* setupStruct)
{
    Plugin::hwndDlg = setupStruct->hwndDlg;
    Plugin::hMenu = setupStruct->hMenu;
    Plugin::hMenuDisasm = setupStruct->hMenuDisasm;
    Plugin::hMenuDump = setupStruct->hMenuDump;
    Plugin::hMenuStack = setupStruct->hMenuStack;
    GuiExecuteOnGuiThread(QtPlugin::Setup);

    _plugin_menuaddentry(Plugin::hMenu, 0, "&About...");
}

extern "C" __declspec(dllexport) bool plugstop()
{
    GuiExecuteOnGuiThread(QtPlugin::Stop);
    return true;
}

extern "C" __declspec(dllexport) void CBINITDEBUG(CBTYPE cbType, PLUG_CB_INITDEBUG* info)
{
    Plugin::sCurrentFileName=info->szFileName;
}

extern "C" __declspec(dllexport) void CBCREATEPROCESS(CBTYPE cbType, PLUG_CB_CREATEPROCESS* info)
{
    Plugin::CreateProcessInfo=*(info->CreateProcessInfo);
    Plugin::modInfo=*(info->modInfo);
    Plugin::sDebugFileName=info->DebugFileName;
    Plugin::fdProcessInfo=*(info->fdProcessInfo);

    if(Plugin::pSearchWidget)
    {
        if(Plugin::pDevice)
        {
            if(Plugin::pDevice->isOpen())
            {
                Plugin::pDevice->close();
            }

            delete Plugin::pDevice;
            Plugin::pDevice=0;
        }

        qint64 nImageAddress=(qint64)Plugin::CreateProcessInfo.lpBaseOfImage;
        qint64 nImageSize=XProcess::getImageSize(Plugin::CreateProcessInfo.hProcess,nImageAddress);

        Plugin::pDevice=new XProcessDevice;

        if(Plugin::pDevice->openHandle(Plugin::CreateProcessInfo.hProcess,nImageAddress,nImageSize,QIODevice::ReadOnly))
        {
            Plugin::options.nBaseAddress=nImageAddress;
            Plugin::pSearchWidget->show();
            Plugin::pSearchWidget->setData(Plugin::pDevice,&(Plugin::options),false);
        }
    }
}

extern "C" __declspec(dllexport) void CBSTOPDEBUG(CBTYPE cbType, PLUG_CB_STOPDEBUG* info)
{
    if(Plugin::pDevice)
    {
        if(Plugin::pDevice->isOpen())
        {
            Plugin::pDevice->close();
        }

        delete Plugin::pDevice;
        Plugin::pDevice=0;
    }

    Plugin::pSearchWidget->hide();
}

extern "C" __declspec(dllexport) void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
    switch(info->hEntry)
    {
    case Plugin::PLUGIN_MENU_ABOUT:
        if(Plugin::pSearchWidget)
        {
            SPDialogInfo di(Plugin::pSearchWidget);

            di.exec();
        }
        break;
    default:
        break;
    }
}
