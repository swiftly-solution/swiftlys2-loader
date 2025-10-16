#include "commandline.h"
#include <dynlib.h>

void *cliPtr = nullptr;

ICommandLine *CommandLine()
{
    if (!cliPtr)
    {
        cliPtr = get_export(load_library(WIN_LIN("tier0.dll", "libtier0.so")), "CommandLine");
    }

    return reinterpret_cast<ICommandLine *(*)()>(cliPtr)();
}
