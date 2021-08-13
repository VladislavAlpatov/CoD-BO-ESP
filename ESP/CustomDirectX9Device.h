#pragma once
#include <d3d9.h>
#include <d3dx9core.h>
#include "vec3.h"

class viewmatrix
{
public:
    float* operator[](int index)
    {
        return this->data[index];
    }

private:
    float data[4][4];
};

class CustomDirectX9Device : public IDirect3DDevice9
{
public:
    void DrawLine(ImVec2 pos1, ImVec2 pos2, int thickness, D3DCOLOR color)
    {
        ID3DXLine* LineL;
        D3DXCreateLine(this, &LineL);

        D3DXVECTOR2 Line[2];
        Line[0] = D3DXVECTOR2(pos1.x, pos1.y);
        Line[1] = D3DXVECTOR2(pos2.x, pos2.y);
        LineL->SetWidth(thickness);
        LineL->Draw(Line, 2, color);
        LineL->Release();
    }
    void DrawMessage(LPD3DXFONT font, ImVec2 pos, LPCSTR Message)
    {
        D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 0, 0);
        RECT rct;
        rct.left = pos.x;
        rct.right = 1680;
        rct.top = pos.y;
        rct.bottom = rct.top + 200;
        font->DrawTextA(NULL, Message, -1, &rct, 0, fontColor);
    }
    ImVec3 WorldToScreen(ImVec3 pos)
    {
        auto dwViewmatrix = *(viewmatrix*)0xBA6970;
        float _x = dwViewmatrix[0][0] * pos.x + dwViewmatrix[0][1] * pos.y + dwViewmatrix[0][2] * pos.z + dwViewmatrix[0][3];
        float _y = dwViewmatrix[1][0] * pos.x + dwViewmatrix[1][1] * pos.y + dwViewmatrix[1][2] * pos.z + dwViewmatrix[1][3];
        float _z = dwViewmatrix[2][0] * pos.x + dwViewmatrix[2][1] * pos.y + dwViewmatrix[2][2] * pos.z + dwViewmatrix[2][3];
        float  w = dwViewmatrix[3][0] * pos.x + dwViewmatrix[3][1] * pos.y + dwViewmatrix[3][2] * pos.z + dwViewmatrix[3][3];

        ImVec2 window_size = this->GetWindowSize();

        float x = (window_size.x / 2 * (_x / w)) + ((_x / w) + window_size.x / 2);

        float y = -(window_size.y / 2 * (_y / w)) + ((_y / w) + window_size.y / 2);

        return ImVec3(x, y, w);
    }
    ImVec2 GetWindowSize()
    {
        D3DVIEWPORT9 viewport;
        this->GetViewport(&viewport);

        return ImVec2(viewport.Width, viewport.Height);
    }
};

