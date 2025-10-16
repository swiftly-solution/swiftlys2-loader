//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose:
//
// $NoKeywords: $
//
//===========================================================================//

#ifndef src_tier1_cutlstringtoken_h
#define src_tier1_cutlstringtoken_h

#include <cstdint>
#include "murmurhash.h"

void RegisterStringToken(uint32_t hashCode, const char *start, const char *end, bool extra);

class CUtlStringToken
{
public:
    CUtlStringToken(const char *str)
    {
        m_nHashCode = MurmurHash2LowerCase((char *)str, 0x31415926);
        RegisterStringToken(m_nHashCode, str, 0, true);
    }

private:
    uint32_t m_nHashCode;
};

#endif