#pragma once
#include "vec3.h"
class CBaseEntity
{
public:
	char pad_0000[284]; //0x0000
	ImVec3 vec_Origin; //0x011C
	char pad_0128[92]; //0x0128
	int m_iHealth; //0x0184
	char pad_0188[1728]; //0x0188
}; //Size: 0x0848