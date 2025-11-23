/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  hud_update.cpp
//

#include "hud.h"
#include "cl_util.h"
#include <stdlib.h>
#include <memory.h>

extern cvar_t* cl_smooth_zoom;

int CL_ButtonBits(bool);
void CL_ResetButtonBits(int bits);

extern float v_idlescale;
float in_fov;
extern void HUD_SetCmdBits(int bits);

bool CHud::UpdateClientData(client_data_t* cdata, float time)
{
	memcpy(m_vecOrigin, cdata->origin, sizeof(Vector));
	memcpy(m_vecAngles, cdata->viewangles, sizeof(Vector));

	m_iKeyBits = CL_ButtonBits(false);
	//Handled in MsgFunc_Weapons now.
	//m_iWeaponBits = cdata->iWeaponBits;

	in_fov = cdata->fov;

	Think();

	float speed = 0.0f;

	switch ((int)cl_smooth_zoom->value)
	{
	case 0: speed = 0.0f; break;  // Disabled
	case 1: speed = 10.0f; break; // Slow
	case 2: speed = 20.0f; break; // Average
	case 3: speed = 30.0f; break; // Fast
	}

	if (speed > 0.0f)
	{
		float flInterp = 1.0f - expf(-speed * (float)m_flTimeDelta);

		cdata->fov = cdata->fov + (m_iFOV - cdata->fov) * flInterp;
	}
	else
	{
		cdata->fov = m_iFOV;
	}

	v_idlescale = m_iConcussionEffect;

	CL_ResetButtonBits(m_iKeyBits);

	// return 1 if in anything in the client_data struct has been changed, 0 otherwise
	return true;
}
