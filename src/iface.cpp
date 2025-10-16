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

#include <string>
#include <safetyhook/safetyhook.hpp>

typedef void *(*CreateInterfaceFn)(const char *name, int *retcode);

CreateInterfaceFn g_pCreateIFaceServer = nullptr;

SW_API void *CreateInterface(const char *name, int *retcode)
{
    if (!g_pCreateIFaceServer)
    {
        g_pCreateIFaceServer = (CreateInterfaceFn)get_export(load_library(WIN_LIN("../../csgo/bin/win64/server.dll", "../../csgo/bin/linuxsteamrt64/libserver.so")), "CreateInterface");
    }

    if (std::string(name).find("Source2Server") != std::string::npos)
    {
        auto ret = g_pCreateIFaceServer(name, retcode);

        if ((bool)g_Source2Server_Init_Hook == false)
        {
            g_Source2Server_Init_Hook = safetyhook::create_inline(((void ***)ret)[0][3], (void *)Source2Server_Init);
            g_Source2Server_Shutdown_Hook = safetyhook::create_inline(((void ***)ret)[0][5], (void *)Source2Server_Shutdown);
        }

        if (retcode)
            *retcode = 0;

        return ret;
    }

    return g_pCreateIFaceServer(name, retcode);
}