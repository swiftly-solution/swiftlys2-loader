//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose:
//
//===========================================================================//

#ifndef src_tier0_commandline_h
#define src_tier0_commandline_h

#include <tier1/cutlstringtoken.h>

class ICommandLine
{
public:
    virtual void CreateCmdLine(const char *commandline) = 0;
    virtual void CreateCmdLine(int argc, char **argv) = 0;
    virtual void CreateCmdLinePrependAppName(const char *commandline) = 0;

    // Check whether a particular parameter exists
    virtual const char *CheckParm(CUtlStringToken param, const char **ppszValue = 0) const = 0;
    virtual bool HasParm(CUtlStringToken param) const = 0;

    // Gets at particular parameters
    virtual int ParmCount() const = 0;
    virtual int FindParm(CUtlStringToken param) const = 0; // Returns 0 if not found.
    virtual const char *GetParm(int nIndex) const = 0;

    // Returns the argument after the one specified, or the default if not found
    virtual const char *ParmValue(CUtlStringToken param, const char *pDefaultVal = 0) const = 0;
    virtual int ParmValue(CUtlStringToken param, int nDefaultVal) const = 0;
    virtual float ParmValue(CUtlStringToken param, float flDefaultVal) const = 0;
    virtual void unk() = 0;

    virtual const char **GetParms() const = 0;
    virtual const char *GetCmdLine(void) const = 0;
    virtual void AppendParm(CUtlStringToken param, const char *pszValues) = 0;

    // Returns true if there's atleast one parm available
    virtual bool HasParms(void) const = 0;

    virtual const char *GetUnkString() = 0;
};

ICommandLine *CommandLine();

#endif