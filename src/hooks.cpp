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

safetyhook::InlineHook g_Source2Server_Init_Hook;
safetyhook::InlineHook g_Source2Server_Shutdown_Hook;

void LoadSwiftlyS2();
void UnloadSwiftlyS2();

void *Source2Server_Init(void *pThis, const char *pInterfaceName)
{
    auto ret = g_Source2Server_Init_Hook.call<void *>(pThis, pInterfaceName);

    LoadSwiftlyS2();

    return ret;
}

void Source2Server_Shutdown(void *pThis)
{
    UnloadSwiftlyS2();

    g_Source2Server_Shutdown_Hook.call<void>(pThis);
}