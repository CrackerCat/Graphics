// d3d11.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "d3d11.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    theApp = new DXViewerApp(hInstance);
    if (theApp->Init()) {
        theApp->Run();
    }

    return 0;
}
