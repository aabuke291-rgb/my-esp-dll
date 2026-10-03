#include <windows.h>

#define JVM_BASE 0x7FFAECD50000

extern "C" __declspec(dllexport) bool esp_project(
    double wx, double wy, double wz,
    double ox, double oy, double oz,
    const float mv[16], const float pr[16], const int vp[4],
    float *outX, float *outY) 
{
    double rx = wx - ox, ry = wy - oy, rz = wz - oz;
    double ex = mv[0]*rx + mv[4]*ry + mv[8] *rz + mv[12];
    double ey = mv[1]*rx + mv[5]*ry + mv[9] *rz + mv[13];
    double ez = mv[2]*rx + mv[6]*ry + mv[10]*rz + mv[14];
    double ew = mv[3]*rx + mv[7]*ry + mv[11]*rz + mv[15];
    double cx = pr[0]*ex + pr[4]*ey + pr[8] *ez + pr[12]*ew;
    double cy = pr[1]*ex + pr[5]*ey + pr[9] *ez + pr[13]*ew;
    double cw = pr[3]*ex + pr[7]*ey + pr[11]*ez + pr[15]*ew;
    if (cw < 0.1) return false;
    double ndcX = cx / cw, ndcY = cy / cw;
    double sx = vp[0] + (ndcX * 0.5 + 0.5) * vp[2];
    double sy = vp[1] + (ndcY * 0.5 + 0.5) * vp[3];
    *outX = (float)sx;
    *outY = (float)(vp[3] - sy);
    return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
