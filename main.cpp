#include <windows.h>

#pragma comment(lib, "user32.lib")

// Переменные состояния
bool g_feature1 = false;
bool g_feature2 = false;

// Обработка событий окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        // Нажатие на кнопки/галочки
        if (LOWORD(wParam) == 1) { // Чекбокс 1
            g_feature1 = !g_feature1;
            CheckDlgButton(hwnd, 1, g_feature1 ? BST_CHECKED : BST_CHECKED);
        }
        else if (LOWORD(wParam) == 2) { // Чекбокс 2
            g_feature2 = !g_feature2;
            CheckDlgButton(hwnd, 2, g_feature2 ? BST_CHECKED : BST_CHECKED);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Поток для создания графического окна
DWORD WINAPI UIThread(LPVOID lpParam) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CustomWindow32";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Создаем стандартное окно
    HWND hwnd = CreateWindowA(
        "CustomWindow32", "Control Panel",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 300, 200,
        NULL, NULL, hInstance, NULL
    );

    // Добавляем элементы управления (кнопки/галочки)
    CreateWindowA("BUTTON", "Включить визуализацию 1", 
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
        20, 20, 240, 30, hwnd, (HMENU)1, hInstance, NULL);

    CreateWindowA("BUTTON", "Включить визуализацию 2", 
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
        20, 60, 240, 30, hwnd, (HMENU)2, hInstance, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
