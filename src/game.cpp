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

#include "game.h"

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

static std::filesystem::path GetDLLPath()
{
#if defined(_WIN32)
    HMODULE hModule = nullptr;
    GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&GetDLLPath),
        &hModule);

    std::wstring path(MAX_PATH, L'\0');
    path.resize(GetModuleFileNameW(hModule, path.data(), static_cast<DWORD>(path.size())));
    return path;
#else
    Dl_info info;
    if (!dladdr(reinterpret_cast<void *>(&GetDLLPath), &info) || !info.dli_fname)
        return {};

    std::error_code ec;
    auto resolved = std::filesystem::canonical(info.dli_fname, ec);
    return ec ? std::filesystem::path(info.dli_fname) : resolved;
#endif
}

std::string GetGameFolder()
{
    return GameFolderFromPath(GetDLLPath());
}
