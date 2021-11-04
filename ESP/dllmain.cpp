#include <Windows.h>
#include <d3d9.h>
#include "MinHook.h"
#include <d3dx9core.h>
#include "vec3.h"
#include "CustomDirectX9Device.h"
#include "CBaseEntity.h"

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

DWORD o_end_scene;
long __stdcall hkEndScene(CustomDirectX9Device* pDevice)
{
    for (byte i = 1; i < 32; i++)
    {
        __try
        {
            CBaseEntity* entity = *(CBaseEntity**)(0x1BFBBF8 + i * 0x8c);

            if (!entity)
                continue;

            auto up = pDevice->WorldToScreen(entity->vec_Origin);
            
            if (up.z > 0 and entity->m_iHealth > 0 and entity->m_iHealth < 500)
            {
                auto bottom = pDevice->WorldToScreen(entity->vec_Origin + ImVec3(0, 0, 70));
                int height = abs((int)(up.y - bottom.y));

                ImVec2 x1 = bottom; x1.x -= height / 4.f;
                ImVec2 x2 = bottom; x2.x += height / 4.f;

                ImVec2 x3 = x1;
                ImVec2 x4 = x2;

                auto health_bar_start = x4;

                x3.y = up.y;
                x4.y = up.y;
                pDevice->DrawLine(x1, x2, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
                pDevice->DrawLine(x3, x4, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
                pDevice->DrawLine(x3, x1, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
                pDevice->DrawLine(x2, x4, 2, D3DCOLOR_ARGB(255, 255, 255, 255));

                pDevice->DrawLine(x2 + ImVec2(5, 0), x4 + ImVec2(5, 0), 5, D3DCOLOR_ARGB(255, 0, 0, 0));

                pDevice->DrawLine(x4 + ImVec2(5, -(height * (entity->m_iHealth / (float)*(int*)(0x2AB5EF64)))), x4 + ImVec2(5, 0), 5, D3DCOLOR_ARGB(255, 0, 255, 0));
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {

        }
    }

    typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
    return reinterpret_cast<EndScene>(o_end_scene)(pDevice);
}


DWORD WINAPI EntryPoint(HMODULE hModule)
{
    MH_Initialize();
    DWORD end_scene_addr = (DWORD)(GetModuleHandle("d3d9.dll")) + 0x63130;

    MH_CreateHook((LPVOID)end_scene_addr, &hkEndScene, reinterpret_cast<LPVOID*>(&o_end_scene));
    MH_EnableHook((LPVOID)end_scene_addr);
    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(100);
    }
    MH_DisableHook((LPVOID)end_scene_addr);
    Sleep(100);
    MH_RemoveHook((LPVOID)end_scene_addr);
    
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call,  LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)EntryPoint, hModule, 0, nullptr);
    }

    return TRUE;

}