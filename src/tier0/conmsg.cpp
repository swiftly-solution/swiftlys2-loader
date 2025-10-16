#include "conmsg.h"

#include <dynlib.h>

void *msgPtr = nullptr;

void Msg(const char *pMsg)
{
    if (!msgPtr)
    {
        msgPtr = get_export(load_library(WIN_LIN("tier0.dll", "libtier0.so")), "Msg");
    }

    return reinterpret_cast<void (*)(const char *)>(msgPtr)(pMsg);
}