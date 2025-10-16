#include "cutlstringtoken.h"
#include "../dynlib.h"

void *registerPtr = nullptr;

void RegisterStringToken(uint32_t hashCode, const char *start, const char *end, bool extra)
{
    if (!registerPtr)
    {
        registerPtr = get_export(load_library(WIN_LIN("tier0.dll", "libtier0.so")), "RegisterStringToken");
    }

    reinterpret_cast<void (*)(uint32_t, const char *, const char *, bool)>(registerPtr)(hashCode, start, end, extra);
}