#include <Windows.h>
#include <d3d9.h>
#include <MinHook.h>
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

            auto screen = pDevice->WorldToScreen(entity->vec_Origin);
            
            if (screen.z > 0 and entity->m_iHealth > 0)
            {
                auto sreen_size = pDevice->GetWindowSize();
                pDevice->DrawLine(ImVec3(sreen_size.x / 2, sreen_size.y, 0), screen, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
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