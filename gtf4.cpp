#include <Windows.h>

#include <assert.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    constexpr int KeyID = 42;
    auto RegisterSuccess = RegisterHotKey(NULL, KeyID, MOD_CONTROL | MOD_ALT, VK_F4);
    assert(RegisterSuccess);
    MSG Message;
    while(GetMessage(&Message, NULL, 0, ~0))
    {
        if(Message.message == WM_HOTKEY)
        {
            HWND Window = GetForegroundWindow();
            if(!Window)
            {
                OutputDebugStringA("GetForegroundWindow() failed!\n");
                continue;
            }
            DWORD ProcessID = 0;
            GetWindowThreadProcessId(Window, &ProcessID);
            if(!ProcessID)
            {
                OutputDebugStringA("GetWindowThreadProcessId() failed!\n");
                continue;
            }
            HANDLE Process = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
            if(!Process)
            {
                OutputDebugStringA("OpenProcess() failed!\n");
                continue;
            }
            BOOL TerminationSuccess = TerminateProcess(Process, 1);
            if(!TerminationSuccess)
            {
                OutputDebugStringA("TerminateProcess() failed!\n");
                continue;
            }
        }
    }
    auto UnregisterSuccess = UnregisterHotKey(NULL, KeyID);
    assert(UnregisterSuccess);
    return 0;
}
