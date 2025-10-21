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

#include "shared.h"
#include "dynlib.h"
#include "hooks.h"
#include "tier0/platform.h"
#include "tier0/conmsg.h"

#include <string>
#include <safetyhook/safetyhook.hpp>
#include <s2binlib/s2binlib.h>

typedef void *(*CreateInterfaceFn)(const char *name, int *retcode);

CreateInterfaceFn g_pCreateIFaceServer = nullptr;

bool s2bin_init = false;

SW_API void *CreateInterface(const char *name, int *retcode)
{
    if (!g_pCreateIFaceServer)
    {
        std::string computedPath = std::string(Plat_GetGameDirectory()) + "/csgo/bin/" + WIN_LIN("win64/server.dll", "linuxsteamrt64/libserver.so");
        g_pCreateIFaceServer = (CreateInterfaceFn)get_export(load_library(computedPath.c_str()), "CreateInterface");
    }

    if (!s2bin_init)
    {
        s2binlib_initialize(Plat_GetGameDirectory(), "csgo");
        s2bin_init = true;
    }

    if (std::string(name).find("Source2ServerConfig") != std::string::npos)
    {
        auto ret = g_pCreateIFaceServer(name, retcode);

        if ((bool)g_Source2ServerConfig_Connect_Hook == false)
        {
            g_Source2ServerConfig_Connect_Hook = safetyhook::create_inline(((void ***)ret)[0][0], (void *)Source2ServerConfig_Connect);
        }

        if (retcode)
            *retcode = 0;

        return ret;
    }
    else if (std::string(name).find("Source2Server") != std::string::npos)
    {
        auto ret = g_pCreateIFaceServer(name, retcode);

        if ((bool)g_Source2Server_Shutdown_Hook == false)
        {
            g_Source2Server_Shutdown_Hook = safetyhook::create_inline(((void ***)ret)[0][5], (void *)Source2Server_Shutdown);
        }

        if (retcode)
            *retcode = 0;

        return ret;
    }

    return g_pCreateIFaceServer(name, retcode);
}