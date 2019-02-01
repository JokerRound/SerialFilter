//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-28
// Description: 
//      The main function in here.
//
// Modify Log:
//      2019-01-28    Hoffman
//      Info: Moidfy Info
//******************************************************************************

#include <iostream>
#include <Windows.h>
#include <tchar.h>

#define SERIAL_NAME _T("COM1")

int _tmain(int argc, TCHAR* argv[])
{
    HANDLE hCom =
        CreateFile(
            SERIAL_NAME,
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

    if (INVALID_HANDLE_VALUE == hCom)
    {
        _tprintf(_T("Open com failed.\r\n"));
    }
    else
    {
        _tprintf(_T("Open com successful.\r\n"));
    }

    do
    {
        TCHAR tzBuffer[MAX_PATH + 1] = { 0 };
        _tprintf(_T("Input (. is quit): "));
        _tscanf(_T("%260s"), tzBuffer);

        if (_tcscmp(tzBuffer, _T(".")) == 0)
        {
            break;
        }

        DWORD dwWriteNumber = 0;
        BOOL bRet = WriteFile(hCom, 
                              tzBuffer, 
                              _tcslen(tzBuffer) * sizeof(tzBuffer[0]), 
                              &dwWriteNumber, 
                              NULL);
        if (!bRet)
        {
            _tprintf(_T("Wirte data failed.\r\n"));
        }
    } while (true);

    if (INVALID_HANDLE_VALUE != hCom)
    {
        CloseHandle(hCom);
        hCom = INVALID_HANDLE_VALUE;
    }

    return 0;
}
