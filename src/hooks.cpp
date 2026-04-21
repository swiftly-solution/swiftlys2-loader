/************************************************************************************************
 *  SwiftlyS2 is a scripting framework for Source2-based games.
 *  Copyright (C) 2025 Swiftly Solution SRL via Sava Andrei-Sebastian and it's contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ************************************************************************************************/

#include "hooks.h"

#include "dynlib.h"
#include <s2binlib/s2binlib.h>
#include "tier0/platform.h"

safetyhook::InlineHook g_Source2_Initialize_Hook;
safetyhook::InlineHook g_Source2Server_Shutdown_Hook;
safetyhook::InlineHook g_Source2ServerConfig_Connect_Hook;

void LoadSwiftlyS2();
void UnloadSwiftlyS2();

void Source2Server_Shutdown(void *pThis)
{
    UnloadSwiftlyS2();

    g_Source2Server_Shutdown_Hook.call<void>(pThis);
}

bool Source2_Initialize(void *pThis)
{
    bool ret = g_Source2_Initialize_Hook.call<bool>(pThis);

    LoadSwiftlyS2();

    return ret;
}

bool Source2ServerConfig_Connect(void *pThis, void *factory)
{
    bool ret = g_Source2ServerConfig_Connect_Hook.call<bool>(pThis, factory);

    if ((bool)g_Source2_Initialize_Hook == false)
    {
        // For both Windows and Linux, it can be the function before Engine_Start_Looping from Source2Main
        // The function also contains "Engine loading startup asset: '%s'\n"
        void *s2init_func = nullptr;
        s2binlib_pattern_scan(
            "engine2",
            WIN_LIN(
                "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 8B E1",
                "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC ? ? ? ? 48 89 BD ? ? ? ? 48 8B 3D ? ? ? ? 48 85 FF"),
            &s2init_func);

        g_Source2_Initialize_Hook = safetyhook::create_inline(s2init_func, (void *)Source2_Initialize);
    }

    return ret;
}