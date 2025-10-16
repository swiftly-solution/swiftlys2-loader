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

#include "dynlib.h"
#include "tier0/conmsg.h"
#include "tier0/platform.h"
#include <tier0/commandline.h>
#include "commandparser.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

char *str_format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0)
    {
        va_end(args);
        return NULL;
    }

    char *buf = (char *)malloc(len + 1);
    if (!buf)
    {
        va_end(args);
        return NULL;
    }

    vsnprintf(buf, len + 1, fmt, args);
    va_end(args);

    return buf;
}

void *libPtr = nullptr;

void LoadSwiftlyS2()
{
    const char *defaultPath = WIN_LIN("addons\\swiftlys2", "addons/swiftlys2");
    const char *cmdLine = CommandLine()->GetCmdLine();

    CommandParser parser;
    parser.parse(cmdLine);

    std::string path = parser.get("sw_path", defaultPath);
    std::string computedPath = (std::string(Plat_GetGameDirectory()) + "/csgo/" + path + WIN_LIN("/bin/win64/swiftlys2.dll", "/bin/linuxsteamrt64/swiftlys2.so"));

    libPtr = load_library(computedPath.c_str());
    void *startCorePtr = get_export(libPtr, "StartCore");
    if (!startCorePtr)
    {
        Msg("SwiftlyS2: Failed to load SwiftlyS2.\n");
        return;
    }

    reinterpret_cast<bool (*)()>(startCorePtr)();
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