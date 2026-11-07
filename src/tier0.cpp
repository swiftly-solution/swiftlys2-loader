#include "tier0.h"
#include "dynlib.h"

ICommandLine *CommandLine()
{
    static const auto fn = get_tier0_export<ICommandLine *(*)()>("CommandLine");
    return fn();
}

void Msg(const char *pMsg)
{
    static const auto fn = get_tier0_export<void (*)(const char *, ...)>("Msg");
    fn("%s", pMsg);
}

const char *Plat_GetGameDirectory()
{
    static const auto fn = get_tier0_export<const char *(*)(int)>("Plat_GetGameDirectory");
    return fn(0);
}
