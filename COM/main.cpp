#include <iostream>
#include <windows.h>
#include <irrlicht.h>

using namespace std;
using namespace irr;

int main()
{
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
    char msg[512];

    while(true)
    {

        for(int i = 0; ReadFile(hwnd, &buffer, 1, &bytesRead, NULL) && buffer != '\n'; i++)
        {
            msg[i] = buffer;
            i++;
        }
        cout << msg << endl;

    }
}
