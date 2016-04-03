/**
* Copyright (C) 2011 MK124
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef __AMX_HELPER_H__
#define __AMX_HELPER_H__

#include <string.h>

#include "plugin.h"
#include "sampgdk.h"

typedef int AMXAPI(*amx_Exec_t)(AMX *amx, cell *retval, int index);

typedef int AMXAPI(*amx_Register_t)(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number);

typedef int AMXAPI(*amx_FindPublic_t)(AMX *amx, const char *name, int *retval);

typedef int AMXAPI(*amx_Callback_t)(AMX *amx, cell index, cell *result, cell *params);

extern void *pAMXFunctions;

inline AMX_NATIVE amx_FindNative(AMX *amx, const char *func)
{
    if (!amx) return NULL;

    int index;
    amx_FindNative(amx, func, &index);
    if (index == 0x7FFFFFFF) return NULL;

    AMX_HEADER *hdr = (AMX_HEADER *) amx->base;
    AMX_FUNCSTUB *funcstub = (AMX_FUNCSTUB *) ((char *) (hdr) + hdr->natives + hdr->defsize * index);

    return (AMX_NATIVE) funcstub->address;
}

inline cell amx_Allot(AMX *amx, int len, cell **phys)
{
    cell amx_str;
    amx_Allot(amx, len, &amx_str, phys);

    return amx_str;
}

inline char *amx_GetString(AMX *amx, const cell str, char *dest, int size)
{
    cell *phys;

    amx_GetAddr(amx, str, &phys);
    amx_GetString(dest, phys, 0, (size_t) size);

    return dest;
}

inline cell amx_NewString(AMX *amx, const char *str, int len = -1)
{
    cell amx_str, *amx_str_phys;
    if (len < 0) len = (int) (strlen(str) + 1);

    amx_Allot(amx, len, &amx_str, &amx_str_phys);
    amx_SetString(amx_str_phys, str, 0, 0, (size_t) len);

    return amx_str;
}

inline bool amx_SetNativeAddress(AMX *amx, const char *func, void *addr)
{
    if (!amx) return false;

    int index;
    amx_FindNative(amx, func, &index);
    if (index == 0x7FFFFFFF) return false;

    AMX_HEADER *hdr = (AMX_HEADER *) amx->base;
    AMX_FUNCSTUB *funcstub = (AMX_FUNCSTUB *) ((char *) (hdr) + hdr->natives + hdr->defsize * index);
    funcstub->address = (ucell) addr;
    return true;
}

#endif