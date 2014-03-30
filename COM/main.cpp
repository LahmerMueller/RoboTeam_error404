#include <iostream>
#include <windows.h>
#include <stdio.h>


using namespace std;


short serialRead(HANDLE hwnd)
{
    signed short result;
    BYTE buffer1;
    BYTE buffer2;
    DWORD bytesRead;

    ReadFile(hwnd, &buffer1, 1, &bytesRead, NULL);
    ReadFile(hwnd, &buffer2, 1, &bytesRead, NULL);
    result = (((short)buffer1) << 8) | buffer2;

    return result;
}

int main()
{
    system("PAUSE");

    FILE *stream;

    stream = fopen("messwerte.dat", "w");

    HANDLE hwnd;
    DCB PortDCB;

    hwnd = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    cout << GetLastError() << endl;

    PortDCB.DCBlength = sizeof (DCB);
    GetCommState (hwnd, &PortDCB);

    PortDCB.BaudRate = 9600;
    PortDCB.fBinary = TRUE;
    PortDCB.fParity = TRUE;
    PortDCB.fOutxCtsFlow = FALSE;
    PortDCB.fOutxDsrFlow = FALSE;
    PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
    PortDCB.fDsrSensitivity = FALSE;
    PortDCB.fTXContinueOnXoff = TRUE;
    PortDCB.fOutX = FALSE;
    PortDCB.fInX = FALSE;
    PortDCB.fErrorChar = FALSE;
    PortDCB.fNull = FALSE;
    PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
    PortDCB.fAbortOnError = FALSE;
    PortDCB.ByteSize = 8;
    PortDCB.Parity = NOPARITY;
    PortDCB.StopBits = ONESTOPBIT;

    if (!SetCommState (hwnd, &PortDCB))
    {
        cout << "Couldn't set com settings: " << GetLastError() << endl;
        return 1;
    }

    BYTE buffer;
    DWORD bytesRead;
    int msg = 0;

    for(int i = 0; i < 5000; i++)
    {
        //ReadFile(hwnd, &buffer, 1, &bytesRead, NULL);
        //fprintf(stream, "%d %d\n", i, serialRead(hwnd));
        ReadFile(hwnd, &buffer, 1, &bytesRead, NULL);
        if(buffer == '\n')
        {
            fprintf(stream, "%d %d\n", i, serialRead(hwnd));
            Sleep(1);
        }
    }

    fclose(stream);
}
