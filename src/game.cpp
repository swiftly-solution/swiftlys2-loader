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

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <climits>
#include <cstdlib>
#endif

std::string GetDLLPath()
{
#if defined(_WIN32)
    char path[MAX_PATH];
    HMODULE hModule = NULL;
    GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCSTR)&GetDLLPath,
        &hModule);
    GetModuleFileNameA(hModule, path, MAX_PATH);
    return std::string(path);
#elif defined(__linux__) || defined(__APPLE__)
    Dl_info info;
    if (dladdr((void *)&GetDLLPath, &info) && info.dli_fname)
    {
        char resolved[PATH_MAX];
        if (realpath(info.dli_fname, resolved))
        {
            return std::string(resolved);
        }
        return std::string(info.dli_fname);
    }
    return "";
#endif
}

std::string GetGameFolder()
{
    std::filesystem::path dll_path = GetDLLPath();
    std::filesystem::path currentPath = dll_path.parent_path();
    while (!currentPath.empty() && currentPath.has_parent_path())
    {
        if (currentPath.filename() == "addons")
        {
            return currentPath.parent_path().filename();
        }
        currentPath = currentPath.parent_path();
    }
    return "";
}