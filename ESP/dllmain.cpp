#include <Windows.h>
#include <d3d9.h>
#include "MinHook.h"
#include <d3dx9core.h>
#include "vec3.h"
#include "CustomDirectX9Device.h"
#include "CBaseEntity.h"

LPVOID o_end_scene;
LPVOID o_on_kill;
LPVOID oPresent;
CustomDirectX9Device* g_pDevice = nullptr;

int __cdecl hOnKill(int a1, int a2)
{
    PlaySound("killsound.wav", NULL, SND_ASYNC);

    typedef int(__cdecl* OnKillT)(int,int);
    return reinterpret_cast<OnKillT>(o_on_kill)(a1, a2);

}

int __fastcall PresentMain(void* pThis,void* edx, const struct tagRECT* a2, const struct tagRECT* a3, HWND a4, const struct _RGNDATA* a5, unsigned int a6)
{
    if (not g_pDevice)
    {
        typedef long(__thiscall* Present)(void* pThis, const struct tagRECT* a2, const struct tagRECT* a3, HWND a4, const struct _RGNDATA* a5, unsigned int a6);
        return reinterpret_cast<Present>(oPresent)(pThis, a2, a3, a4, a5, a6);
    }
    for (byte i = 1; i < 32; i++)
    {
        __try
        {
            CBaseEntity* entity = *reinterpret_cast<CBaseEntity**>(0x1BFBBF8 + i * 0x8c);

            if (!entity)
                continue;

            auto top = g_pDevice->WorldToScreen(entity->vec_Origin);

            if (top.z > 0 and entity->m_iHealth > 0)
            {
                auto bottom = g_pDevice->WorldToScreen(entity->vec_Origin + ImVec3(0, 0, 70));

                int height = abs(static_cast<int>(top.y - bottom.y));

                ImVec2 bottom_left_corner = bottom;
                ImVec2 bottom_right_corner = bottom;

                bottom_left_corner.x -= height / 4.f;
                bottom_right_corner.x += height / 4.f;

                ImVec2 top_left_coner = bottom_left_corner;
                ImVec2 top_right_coner = bottom_right_corner;

                top_left_coner.y = top.y;
                top_right_coner.y = top.y;

                g_pDevice->DrawLine(bottom_left_corner, bottom_right_corner, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
                g_pDevice->DrawLine(top_left_coner, top_right_coner, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
                g_pDevice->DrawLine(top_left_coner, bottom_left_corner, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
                g_pDevice->DrawLine(bottom_right_corner, top_right_coner, 2, D3DCOLOR_ARGB(255, 255, 255, 255));

                g_pDevice->DrawLine(bottom_right_corner + ImVec2(5, 0), top_right_coner + ImVec2(5, 0), 5, D3DCOLOR_ARGB(255, 0, 0, 0));

                g_pDevice->DrawLine(top_right_coner + ImVec2(5, -(height * entity->GetHealthPercent())), top_right_coner + ImVec2(5, 0), 5, entity->GetColorBasedOnHealth());
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {

        }
    }
    typedef long(__thiscall* Present)(void* pThis,const struct tagRECT* a2, const struct tagRECT* a3, HWND a4, const struct _RGNDATA* a5, unsigned int a6);
    return reinterpret_cast<Present>(oPresent)(pThis, a2, a3, a4, a5, a6);
}

long __stdcall hkEndScene(CustomDirectX9Device* pDevice)
{
    g_pDevice = pDevice;

    typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
    return reinterpret_cast<EndScene>(o_end_scene)(pDevice);
}


DWORD WINAPI EntryPoint(HMODULE hModule)
{
    MH_Initialize();
    DWORD end_scene_addr = (DWORD)(GetModuleHandle("d3d9.dll")) + 0x68510;
    DWORD PresentAddr = (DWORD)(GetModuleHandle("d3d9.dll")) + 0x4f353;
    DWORD on_kill_addr   = (DWORD)(GetModuleHandle(NULL))       + 0xc55d0;

    MH_CreateHook((LPVOID)end_scene_addr, &hkEndScene, &o_end_scene);
    MH_CreateHook((LPVOID)PresentAddr,    &PresentMain, &oPresent);
    MH_CreateHook((LPVOID)on_kill_addr,   &hOnKill,    &o_on_kill);
    MH_EnableHook(MH_ALL_HOOKS);

    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(100);
    }

    MH_DisableHook(MH_ALL_HOOKS);
    Sleep(100);
    MH_RemoveHook(MH_ALL_HOOKS);
    
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