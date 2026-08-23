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

#include "dynlib.h"
#include "hooks.h"
#include "tier0/conmsg.h"
#include "tier0/platform.h"
#include <s2binlib/s2binlib.h>

#include <stdlib.h>
#include <string>

void *libPtr = nullptr;
extern std::string g_sSwiftlyS2Path;
extern S2BinLib004 *g_pS2BinLib;
extern std::string g_sGame;

void LoadSwiftlyS2()
{
    std::string computedPath = (std::string(Plat_GetGameDirectory()) + "/" + g_sGame + "/" + g_sSwiftlyS2Path + "/bin/" + WIN_LIN("win64/swiftlys2.dll", "linuxsteamrt64/swiftlys2.so"));

    libPtr = load_library(computedPath.c_str());
    void *startCorePtr = get_export(libPtr, "StartCore");
    if (!startCorePtr)
    {
        Msg("SwiftlyS2: Failed to load SwiftlyS2.\n");
        return;
    }

    reinterpret_cast<bool (*)(void *, void *, void *)>(startCorePtr)((void *)g_pCreateIFaceServer, (void *)g_pCreateIFaceEngine, (void *)g_pS2BinLib);
}

void UnloadSwiftlyS2()
{
    if (!libPtr)
        return;

    void *stopCorePtr = get_export(libPtr, "StopCore");
    if (!stopCorePtr)
    {
        Msg("SwiftlyS2: Failed to unload SwiftlyS2.\n");
        return;
    }

    reinterpret_cast<bool (*)()>(stopCorePtr)();
}