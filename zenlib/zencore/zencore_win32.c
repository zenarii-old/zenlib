#include <windows.h>

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int ShowCMD) {
    MessageBox(0, "Run and link with windows", "Test", MB_ICONINFORMATION | MB_OK);
    
    return 0;
}