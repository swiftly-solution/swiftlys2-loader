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

#include "shared.h"
#include "cmdline.h"
#include "game.h"
#include "hooks.h"

#include <string_view>

SW_API void *CreateInterface(const char *name, int *retcode)
{
    g_sGame = GetGameFolder();
    if (!g_pCreateIFaceServer)
        g_pCreateIFaceServer = get_export_as<CreateInterfaceFn>(load_library(LibPath("", WIN_LIN("server.dll", "libserver.so"))), "CreateInterface");

    if (!g_pS2BinLib)
    {
        g_sSwiftlyS2Path = GetCmdArg(CommandLine()->GetCmdLine(), "-sw_path", WIN_LIN("addons\\swiftlys2", "addons/swiftlys2"));

        g_pS2BinLib = get_export_as<S2CreateInterfaceFn>(load_library(LibPath(g_sSwiftlyS2Path, WIN_LIN("s2binlib.dll", "s2binlib.so"))), "S2BinLib_CreateInterface")(S2BINLIB_INTERFACE_NAME);
        g_pS2BinLib->Initialize(Plat_GetGameDirectory(), g_sGame.c_str());
        g_pS2BinLib->DumpXrefs("engine2");
    }

    void *ret = g_pCreateIFaceServer(name, retcode);
    const std::string_view ifaceName{name};

    if (ifaceName.contains("Source2ServerConfig"))
        HookServerConfig(ret);
    else if (ifaceName.contains("Source2Server"))
        HookServer(ret);
    else
        return ret;

    if (retcode)
        *retcode = 0;

    return ret;
}
