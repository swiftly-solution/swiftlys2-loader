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

#ifndef src_hooks_h
#define src_hooks_h

#include "dynlib.h"
#include "tier0.h"

#include <s2binlib/s2binlib.h>

#include <filesystem>
#include <string>

using CreateInterfaceFn = void *(*)(const char *name, int *retcode);

inline CreateInterfaceFn g_pCreateIFaceServer = nullptr;
inline CreateInterfaceFn g_pCreateIFaceEngine = nullptr;
inline S2BinLib004 *g_pS2BinLib = nullptr;
inline std::string g_sSwiftlyS2Path;
inline std::string g_sGame;

inline std::filesystem::path LibPath(const std::filesystem::path &subdir, const char *lib)
{
    return std::filesystem::path(Plat_GetGameDirectory()) / g_sGame / subdir / "bin" / WIN_LIN("win64", "linuxsteamrt64") / lib;
}

void HookServerConfig(void *iface);
void HookServer(void *iface);

void LoadSwiftlyS2();
void UnloadSwiftlyS2();

#endif
