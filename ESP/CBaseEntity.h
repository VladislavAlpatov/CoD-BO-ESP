#pragma once
#include "vec3.h"
class CBaseEntity
{
public:
	char   pad_0000[284]; //0x0000
	ImVec3 vec_Origin; //0x011C
	char   pad_0128[92]; //0x0128
	int    m_iHealth; //0x0184
	int    m_iMaxHealth;

	float GetHealthPercent()
	{
		return (float)this->m_iHealth / (float)this->m_iMaxHealth;
	}

	D3DCOLOR GetColorBasedOnHealth()
	{
		float health_percent = this->GetHealthPercent();

		if (health_percent >= 0.65f)
			return D3DCOLOR_ARGB(255, 0, 255, 0);
		else if (health_percent >= 0.3f)
			return D3DCOLOR_ARGB(255, 255, 255, 0);
		else if (health_percent >= 0)
			return D3DCOLOR_ARGB(255, 255, 0, 0);
		else
			return D3DCOLOR_ARGB(255, 0, 0, 0);
	}
};