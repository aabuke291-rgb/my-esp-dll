#include <windows.h>
#include <iostream>

#define JVM_BASE 0x7FFAECD50000

// Структуры для удобной передачи данных
struct Vector3 {
    double x, y, z;
};

struct Vector2 {
    float x, y;
};

// 1. Ваша математическая функция WorldToScreen
extern "C" __declspec(dllexport) bool esp_project(
    double wx, double wy, double wz,
    double ox, double oy, double oz,
    const float mv[16], const float pr[16], const int vp[4],
    float *outX, float *outY) 
{
    double rx = wx - ox, ry = wy - oy, rz = wz - oz;
    
    // Перевод в систему координат камеры (ModelView)
    double ex = mv[0]*rx + mv[4]*ry + mv[8] *rz + mv[12];
    double ey = mv[1]*rx + mv[5]*ry + mv[9] *rz + mv[13];
    double ez = mv[2]*rx + mv[6]*ry + mv[10]*rz + mv[14];
    double ew = mv[3]*rx + mv[7]*ry + mv[11]*rz + mv[15];
    
    // Умножение на матрицу проекции (Projection)
    double cx = pr[0]*ex + pr[4]*ey + pr[8] *ez + pr[12]*ew;
    double cy = pr[1]*ex + pr[5]*ey + pr[9] *ez + pr[13]*ew;
    double cw = pr[3]*ex + pr[7]*ey + pr[11]*ez + pr[15]*ew;
    
    // Проверка: находится ли объект перед камерой
    if (cw < 0.1) return false;
    
    // Нормализация (NDC)
    double ndcX = cx / cw;
    double ndcY = cy / cw;
    
    // Перевод в пиксельные координаты экрана (Screen Space)
    double sx = vp[0] + (ndcX * 0.5 + 0.5) * vp[2];
    double sy = vp[1] + (ndcY * 0.5 + 0.5) * vp[3];
    
    *outX = (float)sx;
    *outY = (float)(vp[3] - sy); // Инверсия Y для экрана
    return true;
}

// 2. Вспомогательная функция-экспорт для внешнего вызова (например, из C# или Python)
extern "C" __declspec(dllexport) bool GetScreenPos(
    Vector3 targetPos, Vector3 cameraPos,
    const float* mvMatrix, const float* projMatrix, const int* viewport,
    Vector2* outScreenPos) 
{
    return esp_project(
        targetPos.x, targetPos.y, targetPos.z,
        cameraPos.x, cameraPos.y, cameraPos.z,
        mvMatrix, projMatrix, viewport,
        &outScreenPos->x, &outScreenPos->y
    );
}

// 3. Основной рабочий поток внутри внедренной DLL
DWORD WINAPI MainThread(LPVOID lpParam) {
    // Здесь создается консоль для отладки (по желанию)
    /*
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    std::cout << "[ESP DLL] Loaded successfully at base: " << std::hex << JVM_BASE << std::endl;
    */

    // Основной цикл работы DLL (выполняется, пока не нажат END)
    while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
        // Здесь обычно вызываются функции чтения памяти игры и пересчета координат
        Sleep(10); 
    }

    // Завершение работы и выгрузка DLL
    // FreeConsole();
    // FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

// 4. Точка входа DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
