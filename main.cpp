 #include <windows.h>

#pragma comment(lib, "user32.lib")

bool g_show_window = false;
HWND g_hwnd = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // Действие для кнопки 1
        }
        break;
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        g_show_window = false;
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI UIThread(LPVOID lpParam) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CustomNavClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    // Создаем окно с широкими символами (wchar_t) для поддержки кириллицы
    g_hwnd = CreateWindowW(
        L"CustomNavClass", L"Меню управления",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        100, 100, 280, 180,
        NULL, NULL, hInstance, NULL
    );

    CreateWindowW(L"BUTTON", L"Визуализация 1", 
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
        20, 20, 220, 30, g_hwnd, (HMENU)1, hInstance, NULL);

    CreateWindowW(L"BUTTON", L"Визуализация 2", 
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
        20, 60, 220, 30, g_hwnd, (HMENU)2, hInstance, NULL);

    MSG msg;
    bool key_down = false;

    while (true) {
        // Цикл обработки Right Shift
        if (GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
            if (!key_down) {
                g_show_window = !g_show_window;
                ShowWindow(g_hwnd, g_show_window ? SW_SHOW : SW_HIDE);
                key_down = true;
            }
        } else {
            key_down = false;
        }

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        Sleep(10);
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, UIThread, NULL, 0, NULL);
    }
    return TRUE;
}
