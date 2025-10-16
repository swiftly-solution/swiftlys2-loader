#include "platform.h"
#include <dynlib.h>

void *GetGameDirPtr = nullptr;

const char *Plat_GetGameDirectory()
{
    if (!GetGameDirPtr)
    {
        GetGameDirPtr = get_export(load_library(WIN_LIN("tier0.dll", "libtier0.so")), "Plat_GetGameDirectory");
    }

    return reinterpret_cast<const char *(*)(int)>(GetGameDirPtr)(0);
}