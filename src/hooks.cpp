/************************************************************************************************
 *  SwiftlyS2 is a scripting framework for Source2-based games.
 *  Copyright (C) 2025-2026 Swiftly Solution SRL via Sava Andrei-Sebastian and it's contributors
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

#include <safetyhook/safetyhook.hpp>

static safetyhook::InlineHook s_Source2_Initialize_Hook;
static safetyhook::InlineHook s_Source2Server_Shutdown_Hook;
static safetyhook::InlineHook s_Source2ServerConfig_Connect_Hook;

static void InstallOnce(safetyhook::InlineHook &hook, void *target, auto detour)
{
    if (!hook)
        hook = safetyhook::create_inline(target, reinterpret_cast<void *>(detour));
}

static void Source2Server_Shutdown(void *pThis)
{
    UnloadSwiftlyS2();
    g_pS2BinLib->Destroy();

    s_Source2Server_Shutdown_Hook.call<void>(pThis);
}

static bool Source2_Initialize(void *pThis)
{
    bool ret = s_Source2_Initialize_Hook.call<bool>(pThis);

    LoadSwiftlyS2();

    return ret;
}

static bool Source2ServerConfig_Connect(void *pThis, void *factory)
{
    bool ret = s_Source2ServerConfig_Connect_Hook.call<bool>(pThis, factory);
    g_pCreateIFaceEngine = reinterpret_cast<CreateInterfaceFn>(factory);

    if (!s_Source2_Initialize_Hook)
    {
        void *s2init_func = nullptr;
        g_pS2BinLib->FindFuncWithString("engine2", "Engine loading startup asset: '%s'\n", &s2init_func);

        InstallOnce(s_Source2_Initialize_Hook, s2init_func, Source2_Initialize);
    }

    return ret;
}

void HookServerConfig(void *iface)
{
    InstallOnce(s_Source2ServerConfig_Connect_Hook, static_cast<void ***>(iface)[0][0], Source2ServerConfig_Connect);
}

void HookServer(void *iface)
{
    InstallOnce(s_Source2Server_Shutdown_Hook, static_cast<void ***>(iface)[0][5], Source2Server_Shutdown);
}
