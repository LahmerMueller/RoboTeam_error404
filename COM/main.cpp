#include <iostream>
#include <windows.h>
#include <irrlicht.h>

using namespace std;
using namespace irr;

int serialRead(HANDLE hwnd)
{
    int result;
    BYTE buffer;
    DWORD bytesRead;

    ReadFile(hwnd, &buffer, 1, &bytesRead, NULL);
    result = (int)buffer << 8;
    ReadFile(hwnd, &buffer, 1, &bytesRead, NULL);
    result = result | (int)buffer;

    return result;
}

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

    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
            core::dimension2d<u32>(600, 400), 16, false, false, false);

    video::IVideoDriver* driver = device->getVideoDriver();

    driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

    BYTE buffer;
    DWORD bytesRead;
    int msg = 0;

    while(true)
    {
        ReadFile(hwnd, &buffer, 1, &bytesRead, NULL);
        if(buffer == '\n')
        {
            cout << serialRead(hwnd) << endl;
        }


        driver->beginScene(true, true, video::SColor(255,255,255,255));

        driver->draw2DLine(core::position2d<s32>(96, 100), core::position2d<s32>(96, 205), video::SColor(255, 0, 0, 0));

        driver->endScene();
    }
    device->drop();
}
