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

#include <format>

static void *libPtr = nullptr;

void LoadSwiftlyS2()
{
    libPtr = load_library(LibPath(g_sSwiftlyS2Path, WIN_LIN("swiftlys2.dll", "swiftlys2.so")));
    if (!libPtr)
    {
        Msg(std::format("SwiftlyS2: Failed to load SwiftlyS2: {}\n", get_library_error()).c_str());
        return;
    }

    const auto startCore = get_export_as<bool (*)(void *, void *, void *, const char *)>(libPtr, "StartCore");
    if (!startCore)
    {
        Msg("SwiftlyS2: Failed to load SwiftlyS2.\n");
        return;
    }

    startCore(reinterpret_cast<void *>(g_pCreateIFaceServer), reinterpret_cast<void *>(g_pCreateIFaceEngine), g_pS2BinLib, g_sGame.c_str());
}

void UnloadSwiftlyS2()
{
    if (!libPtr)
        return;

    const auto stopCore = get_export_as<bool (*)()>(libPtr, "StopCore");
    if (!stopCore)
    {
        Msg("SwiftlyS2: Failed to unload SwiftlyS2.\n");
        return;
    }

    stopCore();
}
